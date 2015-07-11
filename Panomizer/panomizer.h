#pragma once

#include <QObject>
#include <QPixmap>
#include <QString>
#include <QList>
#include <QVector>
#include <QFuture>
//#include <QFutureWatcher>
#include <opencv2\opencv.hpp>
#include "videoreader.h"

class Panomizer : public QObject {
	Q_OBJECT

public:
	Panomizer();

	void	initNames( QList< QString > names );
	void	initOffsets( int globalOffset, QList< int > offsets );

	void	createVideo( QString filename, double frameCount, bool highResolution, bool preview );
	void	stopCreating();

	void	sendMaxFPS();
	void	stop();

private:
	QVector< VideoReader >			readers;
	cv::VideoWriter					writer;

	double							maxFPS;
	bool							recordInProgress;
	bool							needStopRecord;
	QList< QFuture< cv::Mat > >		futures;
	//QFutureWatcher< cv::Mat >		watcher;

	void	reset();
	auto	gorisontalSum( std::vector< cv::Mat > mats, bool highResolution = true ) -> cv::Mat;
	auto	getMaxFPS() -> double;
	void	findOptimalVideoCodec( const std::string & name, double fps, const cv::Size & size, bool highResolution );
	//void	writeWatcherFrame();
	//auto	write( const cv::Mat & mat ) -> bool; //QFuture< cv::Mat >
	auto	nextFrame( bool highResolution ) -> QFuture< cv::Mat >;

signals:
	void finished();
	void firstFrame( QPixmap );
	void percent( double );
	void error( QString );
	void status( QString );
	void returnMaxFPS( double );

};
