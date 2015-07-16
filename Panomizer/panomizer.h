#pragma once

#include <QObject>
#include <QPixmap>
#include <QString>
#include <QList>
#include <QVector>
#include <QFuture>
#include <QFutureWatcher>
#include <opencv2\opencv.hpp>
#include "videoreader.h"

enum Quality {
	Low,
	Medium,
	High
};
Q_DECLARE_METATYPE( Quality );

class Panomizer : public QObject {
	Q_OBJECT

public:
	Panomizer( int _screenWidth );

	void	initNames( QList< QString > names );
	void	initOffsets( int globalOffset, QList< int > offsets );

	void	sendFirstFrame();

	void	createVideo( QString filename, double frameCount, bool autoFindCodec );
	void	stopCreating();

	void	sendMaxFPS();
	void	stop();

	void	setAddFiftyFrame( bool newAddFiftyFrame );
	void	setSplit( float newSplit );
	void	setQuality( Quality newQuality );

private:
	QVector< VideoReader >			readers;
	cv::VideoWriter					writer;

	double							maxFPS;
	bool							recordInProgress;
	bool							needStopRecord;
	QList< QFuture< cv::Mat > >		futures;
	QFutureWatcher< cv::Mat >		watcher;
	bool							addFiftyFrame;
	Quality							quality;
	float							split;
	int								screenWidth;

	void	firstFrameFinished();
	void	showPreview( const cv::Mat &frame );
	void	closePreview();
	void	reset();
	auto	resizeForH264( cv::Mat & mat ) -> decltype( mat );
	auto	gorisontalSum( std::vector< cv::Mat > mats, Quality quality, float split ) noexcept -> cv::Mat;
	auto	getMaxFPS() -> double;
	void	findOptimalVideoCodec( const std::string & name, double fps, const cv::Size & size, bool highResolution, bool autoFindCodec );
	//void	sendWatcherFrame();
	//auto	write( const cv::Mat & mat ) -> bool; //QFuture< cv::Mat >
	auto	nextFrame( Quality quality, float splitValue ) -> QFuture< cv::Mat >;

signals:
	void finished();
	void percent( double );
	void error( QString );
	void status( QString );
	void returnMaxFPS( double );
	void created();
	void returnFrameCount( double );

};
