#pragma once
#include <opencv2\opencv.hpp>

class VideoReader : public cv::VideoCapture {

public:
	VideoReader( const std::string & filename = "" );

	static auto	getSyncOffset( double fps, double maxFPS, double offset ) -> double;

	void	setOffset( int offset );
	void	setGlobalOffset( double globalOffset );
	void	setCurrentOffset( double currentOffset );
	void	setNeedFPS( double deltaFPS );

	auto	fps() -> double;
	auto	frameCount() -> double;
	auto	codec() -> int;
	auto	codecName()->std::string;
	auto	frameSize() -> double;
	auto	filename() -> const std::string;
	auto	offset() -> const int;
	auto	currentOffset() -> const double;
	auto	needFPS() -> const double;

	void	reset();
	auto	operator >> ( cv::Mat & mat ) -> VideoReader &;

private:
	double			mOffset, mGlobalOffset;
	double			mCurrentDelta, mCurrentOffset;
	double			mNeedFPS;
	double			mCurrentFrame;
	std::string		mFilename;
	cv::Mat			lastFrame;

};

