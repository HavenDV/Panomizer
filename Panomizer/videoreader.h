#pragma once
#include <opencv2\opencv.hpp>

class VideoReader : public cv::VideoCapture {

public:
	VideoReader();
	VideoReader( const std::string & filename );

	auto	getSyncOffset( double fps, double maxFPS, double offset ) -> double;

	void	setGlobalOffset( double globalOffset );

	void	reset();

	auto	fps() -> double;

	auto	frameCount() -> double;

	auto	frameSize() -> double;

	auto	filename() -> const std::string;

	auto	offset() -> const int;
	auto	currentOffset() -> const double;

	auto	needFPS() -> const double;

	void	setOffset( int offset );
	void	setNeedFPS( double deltaFPS );
	void	setCurrentOffset( double currentOffset );

	auto	operator >> ( cv::Mat & mat ) -> VideoReader &;

private:
	double			mOffset, mGlobalOffset;
	double			mCurrentDelta, mCurrentOffset;
	double			mNeedFPS;
	double			mCurrentFrame;
	std::string		mFilename;
	cv::Mat			lastFrame;

};

