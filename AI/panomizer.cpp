#include "panomizer.h"
#include "asmopencv.h"
#include <QSettings>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrentRun>	

auto	operator "" _q( const char* str, size_t size ) -> QString {
	return str;
}

Panomizer::Panomizer() : recordInProgress{ false }, needStopRecord{ false } {
	//connect( &watcher, &QFutureWatcher< cv::Mat >::finished, this, &Panomizer::writeWatcherFrame );
}

void	Panomizer::stop() {
	emit finished();
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

	if ( recordInProgress ) {
		return;
	}

	reset();

	auto frame = nextFrame( false ).result();
	if ( !frame.empty() ) {
		emit firstFrame( ASM::cvMatToQPixmap( frame ) );
	}
}

void	Panomizer::reset() {
	if ( recordInProgress ) {
		return;
	}

	for ( auto && reader : readers ) {
		reader.reset();
	}
}

auto	emptyMat() -> cv::Mat {
	return{};
}

auto	Panomizer::nextFrame( bool highResolution ) -> QFuture< cv::Mat > {//cv::Mat {
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

	mats.push_back( mats[ 0 ] );
	return QtConcurrent::run( this, &Panomizer::gorisontalSum, mats, highResolution );
}

auto	Panomizer::gorisontalSum( std::vector< cv::Mat > mats, bool highResolution ) -> cv::Mat {
	if ( mats.size() < 1 ) {
		return{};
	}
	
	auto rows = 0;
	auto cols = 0;
	auto type = 0;
	for ( auto & mat : mats ) {
		if ( !highResolution ) {
			cv::resize( mat, mat, { mat.cols / 4, mat.rows / 4 } );
		}
		rows = std::max( rows, mat.rows );
		type = mat.type();
		cols += mat.cols;
	}
	cv::Mat out( rows, cols, type );

	auto currentX = 0;
	for ( auto && mat : mats ) {
		mat.copyTo( out( { { currentX, 0 }, mat.size() } ) );
		currentX += mat.cols;
	}

	return out;
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

void	Panomizer::findOptimalVideoCodec( const std::string & name, double fps, const cv::Size & size, bool highResolution ) {
	if ( !highResolution ) 		writer.open( name, CV_FOURCC( 'M', 'P', 'G', '4' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'M', '4', 'S', '2' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'F', 'M', 'P', '4' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'W', 'M', 'V', '1' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'D', 'I', 'V', 'X' ), fps, size );
	if ( !writer.isOpened() )	writer.open( name, CV_FOURCC( 'X', '2', '6', '4' ), fps, size );
}
/*
void	Panomizer::writeWatcherFrame() {
	writer << watcher.result();
}

auto	Panomizer::write( const cv::Mat & mat ) -> bool { //const QFuture< cv::Mat > & future
	//if ( future.isFinished() ) {
	try {
		writer << mat;// future.result();
		return true;
	}
	catch (...) {}
	return false;
	//}
	//else {
	//	watcher.setFuture( future );
	//}
}
*/
void Panomizer::createVideo( QString filename, double frameCount, bool highResolution, bool preview ) {
	if ( recordInProgress ) {
		return;
	}

	if ( !preview ) {
		reset();
		auto frame = nextFrame( highResolution ).result();
		if ( frame.empty() ) {
			emit error( "input" );
			return;
		}

		findOptimalVideoCodec( filename.toStdString(), maxFPS, frame.size(), highResolution );
		if ( !writer.isOpened() ) {
			emit error( "name/codec" );
			return;
		}
	}
	reset();

	futures.clear();
	needStopRecord = false;
	recordInProgress = true;
	if ( frameCount <= 0.0 ) {
		frameCount = std::numeric_limits< double >::max();
	}
	for ( auto && reader : readers ) {
		frameCount = std::min( frameCount, reader.frameSize() );
	}

	try {
		auto futuresSize = QThread::idealThreadCount();
		for ( auto currentFrame = 0.0; currentFrame <= frameCount; ++currentFrame ) {
			for ( auto i = futures.size(); i < futuresSize && ( frameCount - currentFrame > i  ); ++i ) {
				futures.append( nextFrame( highResolution ) );
			}

			if ( futures.empty() ) {
				break;
			}
  
			auto frame = futures.takeFirst().result();
			if ( frame.empty() ) {
				break;
			}

			if ( preview ) {
				cv::imshow( "preview", frame );
			}
			else {
				writer << frame;
			}

			QCoreApplication::processEvents();
			if ( needStopRecord ) {
				break;
			}

			emit percent( 100 * currentFrame / frameCount );
		}
	}
	catch ( const cv::Exception & e ) {
		emit error( e.what() );
		needStopRecord = true;
	}
	writer.release();
	recordInProgress = false;

	if ( needStopRecord ) {
		emit status( "stopped" );
	}
	else {
		emit status( "created" );
	}
}