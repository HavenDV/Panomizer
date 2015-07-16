#include "panomizer.h"
#include "asmopencv.h"
#include <QSettings>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrentRun>	

constexpr auto previewWindowName = "preview";

auto	operator "" _q( const char* str, size_t size ) -> QString {
	return str;
}

Panomizer::Panomizer( int _screenWidth ) : 
	recordInProgress{ false }, 
	needStopRecord{ false }, 
	addFiftyFrame{ true },
	quality{ Quality::High },
	split{ 0.0F },
	screenWidth{ _screenWidth } {
	&decltype( watcher )::finished;
	connect( &watcher, &QFutureWatcher< cv::Mat >::finished, this, &Panomizer::firstFrameFinished );
}

void	Panomizer::stop() {
	emit finished();
}

void Panomizer::setAddFiftyFrame( bool newAddFiftyFrame ) {
	addFiftyFrame = newAddFiftyFrame;
	sendFirstFrame();
}

void Panomizer::setSplit( float newSplit ) {
	split = newSplit;
	sendFirstFrame();
}

void Panomizer::setQuality( Quality newQuality ) {
	quality = newQuality;
	sendFirstFrame();
}

void	Panomizer::stopCreating() {
	needStopRecord = true;
}

void	Panomizer::initNames( QList< QString > names ) {
	readers.clear();
	
	auto i = 0;
	for ( auto && name : names ) {
		VideoReader reader{ name.toStdString() };
		if ( !reader.isOpened() ) {
			emit error( QString( u8"Файл №%1 (\"%2\") не может быть открыт. Выберите другой файл" ).arg( i + 1 ).arg( name ) );
			return;
		}
		readers.append( reader );
		++i;
	}

	maxFPS = getMaxFPS();
	for ( auto && reader : readers ) {
		reader.setNeedFPS( maxFPS );
	}
}

void Panomizer::initOffsets( int globalOffset, QList< int > offsets ) {
	auto offset = std::begin( offsets );
	for ( auto && reader : readers ) {
		reader.setOffset( *offset );
		reader.setGlobalOffset( globalOffset );
		++offset;
	}

	sendFirstFrame();
}

void	Panomizer::sendFirstFrame() {
	if ( recordInProgress ) {
		return;
	}

	reset();
	watcher.setFuture( nextFrame( quality, split ) );
}

void	Panomizer::firstFrameFinished() {
	showPreview( watcher.result() );
}

void	Panomizer::showPreview( const cv::Mat &frame ) {
	if ( frame.empty() ) {
		return;
	}

	auto k = static_cast< float >( screenWidth ) / frame.cols;
	auto height = static_cast< int >( frame.rows * k );
	cv::namedWindow( previewWindowName, cv::WINDOW_NORMAL );
	cv::moveWindow( previewWindowName, 0, 0 );
	cv::resizeWindow( previewWindowName, screenWidth, height );
	cv::imshow( previewWindowName, frame );
}

void Panomizer::closePreview() {
	cv::destroyWindow( previewWindowName );
}

void	Panomizer::reset() {
	if ( recordInProgress ) {
		return;
	}

	futures.clear();
	for ( auto && reader : readers ) {
		reader.reset();
	}
	writer.release();
}

auto	emptyMat() -> cv::Mat {
	return{};
}

auto	Panomizer::nextFrame( Quality quality, float splitValue ) -> QFuture< cv::Mat > {//cv::Mat {
	std::vector< cv::Mat > mats;
	
	for ( auto & reader : readers ) {
		if ( !reader.isOpened() ) {
			return QtConcurrent::run( &emptyMat );
		}

		cv::Mat mat;
		reader >> mat;

		if ( mat.empty() ) {
			return QtConcurrent::run( &emptyMat );
		}
		mats.push_back( mat );
	}
	
	if ( mats.empty() ) {
		return QtConcurrent::run( &emptyMat );
	}

	if ( addFiftyFrame ) {
		mats.push_back( mats[ 0 ] );
	}
	return QtConcurrent::run( this, &Panomizer::gorisontalSum, mats, quality, splitValue );
}

auto	toGray( const cv::Mat &in, bool normalise ) -> cv::Mat { //Tested in GPU. bugs and ~0.1x CPU speed
	if ( in.empty() ) {
		return in;
	}

	auto out = cv::Mat{};
	if ( in.channels() == 4 ) {
		cvtColor( in, out, cv::COLOR_BGRA2GRAY );
	}
	else if ( in.channels() == 3 ) {
		cvtColor( in, out, cv::COLOR_BGR2GRAY );
	}
	else {
		out = in; //.clone() MAY BE ERROR
	}

	if ( normalise && out.channels() == 1 ) {
		equalizeHist( out, out );
	}

	return out;
}

void drawOptFlowMap( const cv::Mat& flow, cv::Mat& cflowmap, int step,
					 double, const cv::Scalar& color ) {
	for ( int y = 0; y < cflowmap.rows; y += step ) {
		for ( int x = 0; x < cflowmap.cols; x += step ) {
			const cv::Point2f& fxy = flow.at<cv::Point2f>( y, x );
			line( cflowmap, { x, y }, { cvRound( x + fxy.x ), cvRound( y + fxy.y ) }, color );
			//circle( cflowmap, cv::Point( x, y ), 2, color, -1 );
		}
	}
}

auto	meanMat( const cv::Mat & mat1, const cv::Mat & mat2 ) -> cv::Mat {
	cv::Mat meanMat{ mat1.size(), mat1.type() };
	for ( auto i = 0; i < meanMat.cols; ++i ) {
		auto k = static_cast< float >( i ) / meanMat.cols;
		auto m = 1.0F - k;
		for ( auto j = 0; j < meanMat.rows; ++j ) {
			auto color1 = mat1.at< cv::Vec3b >( j, i );
			auto color2 = mat2.at< cv::Vec3b >( j, i );
			auto color3 = cv::Vec3b{
				static_cast< uchar >( m * color1[ 0 ] + k * color2[ 0 ] ),
				static_cast< uchar >( m * color1[ 1 ] + k * color2[ 1 ] ),
				static_cast< uchar >( m * color1[ 2 ] + k * color2[ 2 ] )
			};
			meanMat.at< cv::Vec3b >( j, i ) = color3;
		}
	}
	return meanMat;
}

auto	flowMean( const cv::Mat & mat1, const cv::Mat & mat2 ) -> cv::Mat {
	if ( mat1.empty() || mat2.empty() || mat1.size() != mat2.size() ) {
		return{};
	}

	cv::Mat matGray1 = toGray( mat1, false ),
		matGray2 = toGray( mat2, false ),
		flow;

	cv::calcOpticalFlowFarneback( matGray1, matGray2, flow, 0.5, 3, 5, 5, 5, 1.2, cv::OPTFLOW_FARNEBACK_GAUSSIAN );
	//auto centerPoint = getCenterPoint( matGray1 );
	//auto outPoint = getSFlowPoint( centerPoint, flow, centerPoint.y ) -
	//	getSFlowPoint( centerPoint, flow, 99999 );

	auto out = meanMat( mat1, mat2 );
	drawOptFlowMap( flow, out, 32, 2.0, { 0, 255, 0 } );
	cv::imshow( "dif", out );

	return out;
}

#include <opencv2\xfeatures2d.hpp>
auto	findPoints( const cv::Mat & mat1, const cv::Mat & mat2 ) -> void {
	static auto detector = cv::xfeatures2d::SURF::create();

	cv::imshow( "meanMat", meanMat( mat1, mat2 ) );

	std::vector< cv::KeyPoint > keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;

	detector->detectAndCompute( mat1, cv::noArray(), keypoints1, descriptors1 );
	detector->detectAndCompute( mat2, cv::noArray(), keypoints2, descriptors2 );
	
	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match( descriptors1, descriptors2, matches );

	auto minmax = std::minmax_element(
		std::begin( matches ), std::end( matches ),
		[] ( const cv::DMatch & a, const cv::DMatch & b ) -> bool {
			return a.distance < b.distance;
	} );
	auto minDistance = matches.empty() ? 0.0F : minmax.first->distance;
	auto maxDistance = matches.empty() ? 0.0F : minmax.second->distance;

	std::vector< cv::DMatch > goodMatches;

	for ( auto && match : matches ) {
		if ( match.distance < 0.3F * maxDistance ) {
			goodMatches.push_back( match );
		}
	}

	cv::Mat img_matches;
	cv::drawMatches( mat1, keypoints1, mat2, keypoints2,
					 goodMatches, img_matches, cv::Scalar::all( -1 ), cv::Scalar::all( -1 ),
					 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Localize the object
	std::vector<cv::Point2f> obj;
	std::vector<cv::Point2f> scene;

	for ( auto && goodMatch : goodMatches ) {
		obj.push_back( keypoints1[ goodMatch.queryIdx ].pt );
		scene.push_back( keypoints2[ goodMatch.trainIdx ].pt );
	}

	if ( obj.size() < 5 || scene.size() < 5 ) {
		cv::imshow( "matches", img_matches );
		return;
	}
	
	cv::Mat homography = findHomography( obj, scene, cv::RANSAC );

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector< cv::Point2f > obj_corners{ 4 };
	obj_corners[ 0 ] = cv::Point{ 0, 0 };
	obj_corners[ 1 ] = cv::Point{ mat1.cols, 0 };
	obj_corners[ 2 ] = cv::Point{ mat1.cols, mat1.rows };
	obj_corners[ 3 ] = cv::Point{ 0, mat1.rows };
	std::vector< cv::Point2f > scene_corners{ 4 };

	try {
		cv::perspectiveTransform( obj_corners, scene_corners, homography );
	}
	catch ( ... ) {
		return;
	}

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	cv::Point2f point = cv::Point( mat1.cols, 0 );
	cv::line( img_matches, scene_corners[ 0 ] + point, scene_corners[ 1 ] + point, { 0, 255, 0 }, 4 );
	cv::line( img_matches, scene_corners[ 1 ] + point, scene_corners[ 2 ] + point, { 0, 255, 0 }, 4 );
	cv::line( img_matches, scene_corners[ 2 ] + point, scene_corners[ 3 ] + point, { 0, 255, 0 }, 4 );
	cv::line( img_matches, scene_corners[ 3 ] + point, scene_corners[ 0 ] + point, { 0, 255, 0 }, 4 );

	cv::imshow( "matches", img_matches );
}

auto	Panomizer::resizeForH264( cv::Mat & mat ) -> decltype( mat ) {
	constexpr auto level = 36864 * 256;
	if ( mat.cols * mat.rows > level ) { // 9600 1080  600x68
		cv::resize( mat, mat, { 7680, 864 } );
	}
	return mat;
}

auto	Panomizer::gorisontalSum( std::vector< cv::Mat > mats, Quality quality, float splitValue ) noexcept -> cv::Mat {
	try {
		if ( mats.empty() ) {
			return{};
		}
		if ( mats.size() == 1 ) {
			return mats[ 0 ];
		}

		auto rows = 0;
		auto cols = 0;
		auto type = 0;
		auto size = static_cast< int >( mats.size() );
		for ( auto && mat : mats ) {
			if ( quality == Quality::Low ) {
				cv::resize( mat, mat, { mat.cols / size, mat.rows / size } );
			}
			else if ( quality == Quality::Medium ) {
				cv::resize( mat, mat, { 2 * mat.cols / size, 2 * mat.rows / size } );
			}
			rows = std::max( rows, mat.rows );
			type = mat.type();
			cols += mat.cols;// -width;
		}

		splitValue = std::min( std::max( splitValue, 0.0F ), 0.5F );
		auto width = static_cast< int >( splitValue * mats[ 0 ].cols );
		cols -= width * mats.size();

		cv::Mat out( rows, cols, type );

		
		//flowMean( mats[ 0 ]( { mats[ 0 ].cols - width, 0, width, mats[ 0 ].rows } ),
		//			mats[ 1 ]( { 0, 0, width, mats[ 1 ].rows } ) );

		auto currentX = 0;
		for ( auto && mat : mats ) {
			cv::Mat mean;
			auto meanRect = cv::Rect{ currentX - width, 0, width, out.rows };
			if ( currentX > 0 && width > 0 ) {
				mean = meanMat( out( meanRect ), mat( { 0, 0, width, out.rows } ) );
			}
			auto rect = cv::Rect{ width, 0, mat.cols - width, mat.rows };
			mat( rect ).copyTo( out( { { currentX, 0 }, rect.size() } ) );
			if ( !mean.empty() ) {
				mean.copyTo( out( meanRect ) );
			}
			currentX += mat.cols - width;
		}

		return resizeForH264( out );
	}
	catch ( const cv::Exception & exception ) {
		qDebug() << exception.what();
	}
	return{};
}

auto	Panomizer::getMaxFPS() -> double {
	auto maxFPS = 0.0;
	for ( auto && reader : readers ) {
		maxFPS = std::max( maxFPS, reader.fps() );
	}
	return maxFPS;
}

void	Panomizer::sendMaxFPS() {
	emit returnMaxFPS( maxFPS );
}

void	Panomizer::findOptimalVideoCodec( const std::string & name, double fps, const cv::Size & size, bool highResolution, bool autoFindCodec ) {
	if ( !autoFindCodec && !writer.isOpened() ) {
		writer.open( name, CV_FOURCC_PROMPT, fps, size );
	}
	
	auto fourcc = CV_FOURCC_PROMPT;
	for ( auto && reader : readers ) {
		fourcc = reader.codec();
	}
	if ( !writer.isOpened() )	writer.open( name, fourcc, fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'X', '2', '6', '4' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'H', '2', '6', '4' ), fps, size );
	if ( !writer.isOpened() && !highResolution ) {
								writer.open( name, CV_FOURCC( 'M', 'P', 'G', '4' ), fps, size );
	}
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'M', '4', 'S', '2' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'F', 'M', 'P', '4' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'W', 'M', 'V', '1' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'D', 'I', 'V', 'X' ), fps, size );
}

void Panomizer::createVideo( QString filename, double frameCount, bool autoFindCodec ) {
	if ( recordInProgress ) {
		return;
	}

	reset();
	auto frame = nextFrame( quality, split ).result();
	if ( frame.empty() ) {
		emit error( "input" );
		return;
	}
	reset();

	auto isPreview = filename.isEmpty();
	if ( !isPreview ) {
		closePreview();
		findOptimalVideoCodec( filename.toStdString(), maxFPS, frame.size(), quality == Quality::High, autoFindCodec );
		if ( !writer.isOpened() ) {
			emit error( "name/codec" );
			return;
		}
	}

	
	needStopRecord = false;
	recordInProgress = true;
	if ( frameCount <= 0.0 ) {
		frameCount = std::numeric_limits< double >::max();
	}
	for ( auto && reader : readers ) {
		frameCount = std::min( frameCount, reader.frameSize() );
	}
	emit returnFrameCount( frameCount );

	try {
		auto futuresSize = QThread::idealThreadCount();
		for ( auto currentFrame = 0.0; currentFrame <= frameCount; ++currentFrame ) {
			for ( auto i = futures.size(); i < futuresSize && ( frameCount - currentFrame > i  ); ++i ) {
				futures.append( nextFrame( quality, split ) );
			}

			if ( futures.empty() ) {
				break;
			}
  
			auto frame = futures.takeFirst().result();
			if ( frame.empty() ) {
				break;
			}

			if ( isPreview ) {		
				showPreview( frame );
			}
			else {
				writer << frame;
			}

			QCoreApplication::processEvents();
			if ( needStopRecord ) {
				break;
			}

			emit percent( 100.0 * currentFrame / frameCount );
		}
	}
	catch ( const cv::Exception & e ) {
		emit error( e.what() );
	}
	recordInProgress = false;
	reset();

	emit percent( 100.0 );
	emit created();
}