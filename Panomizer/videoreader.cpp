#include "videoreader.h"

VideoReader::VideoReader( const std::string & filename ) :
	cv::VideoCapture( filename ),
	mOffset{ 0.0 }, mGlobalOffset{ 0.0 }, mNeedFPS{ 0.0 },
	mCurrentDelta{ 0.0 }, mCurrentOffset{ 0.0 },
	mFilename{ filename }, mCurrentFrame{ 0.0 } {
}

auto	VideoReader::getSyncOffset( double fps, double maxFPS, double offset ) -> double {
	return ( 2 * fps - maxFPS ) * offset * ( maxFPS - fps ) / ( fps * fps );
}

void	VideoReader::setOffset( int offset ) {
	mOffset = offset;
}

void	VideoReader::setGlobalOffset( double globalOffset ) {
	mGlobalOffset = globalOffset;
}

void	VideoReader::setNeedFPS( double needFPS ) {
	mNeedFPS = needFPS;
	//if ( needFPS - fps() > 0.0 ) {
	//emit error( QString( u8"FPS ����� %1 ������� �� ������������� FPS(%2, %3)." ).arg( i + 1 ).arg( currentFPS ).arg( maxFPS ) );
	//}
	reset();
}

void	VideoReader::setCurrentOffset( double currentOffset ) {
	mCurrentOffset = currentOffset;
}

void	VideoReader::reset() {
	auto syncOffset = mGlobalOffset + mOffset - getSyncOffset( fps(), mNeedFPS, mGlobalOffset + mOffset );
	if ( syncOffset < frameCount() ) {
		set( CV_CAP_PROP_POS_FRAMES, syncOffset );
	}
	else {
		//emit error( QString( u8"�������� ��� ����� \"%1\" ��������� ��� ������������ �� %2 ����" ).arg( mFilename ).arg( syncOffset - frameCount() + 1 ) );
		set( CV_CAP_PROP_POS_FRAMES, frameCount() - 1 );
	}
	mCurrentFrame = 0.0;
	mCurrentDelta = 0.0;
	mCurrentOffset = 0.0;
}

auto	VideoReader::fps() -> double {
	return get( CV_CAP_PROP_FPS );
}

auto	VideoReader::frameCount() -> double {
	return get( CV_CAP_PROP_FRAME_COUNT );
}

auto	VideoReader::codec() -> int {
	return static_cast< int >( get( CV_CAP_PROP_FOURCC ) );
}

auto	VideoReader::codecName() -> std::string {
	auto ex = codec();
	char name[ 5 ];
	for ( auto i = 0U; i < 4U; ++i ) {
		name[ i ] = static_cast< char >( ( ex & ( 0XFF << i * 8U ) ) >> i * 8U );
	}
	name[ 4 ] = '\0';
	return name;
}

auto	VideoReader::frameSize() -> double {
	return frameCount() - mOffset - mGlobalOffset;
}

auto	VideoReader::filename() -> const std::string {
	return mFilename;
}

auto	VideoReader::offset() -> const int {
	return mOffset;
}

auto	VideoReader::needFPS() -> const double {
	return mNeedFPS;
}

auto	VideoReader::currentOffset() -> const double {
	return mCurrentOffset;
}

auto	VideoReader::operator >> ( cv::Mat & mat ) -> VideoReader & {
	mCurrentDelta = getSyncOffset( fps(), mNeedFPS, mCurrentFrame - 3.0 * mCurrentOffset );
	if ( mCurrentDelta >= mCurrentOffset + 1.0 ) {
		++mCurrentOffset;// += 1.0;
		lastFrame.copyTo( mat );
	}
	else {
		cv::VideoCapture::operator>>( mat );
		lastFrame = mat;
	}
	++mCurrentFrame;
	return *this;
}
