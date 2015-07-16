#pragma once

#include <QMainWindow>
#include <QSharedPointer>
#include <QTimer>
#include <QList>
#include <QProcess>
#include <QString>
#include "ui_window.h"
#include "panomizer.h"

class Window : public QMainWindow {
	Q_OBJECT

public:
	Window( QWidget *parent = 0 );
	~Window();

private:
	Ui::MainWindow ui;
	QTimer timer;
	QString compressName, sound1, sound2, sound3, sound4;
	qint32 numCompletedAudio;
	int recordState;
	void removeSound();
	void addThread();
	int		get( QLineEdit & lineEdit );
	void	readSettings();
	QString animateString( QString s );
	int		screenWidth;

	void	getSound( double maxFPS );
	void	init();
	void	initOffset();
	void	addOne( QLineEdit* );
	void	removeOne( QLineEdit* );
	void	pushButton1Clicked();
	void	pushButton2Clicked();
	void	pushButton3Clicked();
	void	pushButton4Clicked();
	void	pushButton11Clicked();
	void	pushButton12Clicked();
	void	pushButton13Clicked();
	void	pushButton14Clicked();
	void	pushButton15Clicked();
	void	pushButton16Clicked();
	void	pushButton17Clicked();
	void	pushButton18Clicked();
	void	pushButton19Clicked();
	void	pushButton20Clicked();
	//void	deleteFile();
	//void	newFile();
	void	saveFile();
	void	preview();
	void	setPercent( double );
	void	setFrameCount( double );
	void	setError( QString );
	void	afterCreated();
	void	setStatus( const QString & text );
	void	sendSplit();
	void	compressButton();
	void	enableButtons();
	void	disableButtons();
	void	disableAll();
	void	enableAll();
	void	removeTempFiles();
	void	reset();
	void	stopButton();
	void	compress( QString in, QString out );
	void	convert( QString in, QString out );
	void	compressState( QProcess::ProcessState s );
	void	convertState( QProcess::ProcessState s );

	auto	getFrameCount() -> int;
	auto	getSplitValue() -> float;
	auto	isHighQuality() -> bool;
	auto	isAutoSelectCodec() -> bool;
	auto	isAddFiftyFrame() -> bool;
	auto	isAutoCompress() -> bool;
	auto	isSoundEnabled() -> bool;

	void	addFiftyFrameState( int state );
	void	splitState( int state );
	void	qualityState( bool checked );

	void	help();
	void	create( const QString & filename );
	void	audioState( QProcess::ProcessState state );
	void	animate();
	auto	getAudio( QString in, QString out, double offset = 0.0, double time = 0.0 ) -> QString;
signals:
	void	initNames( QList< QString > names );
	void	initOffsets( int globalOffset, QList< int > offsets );

	void	setAddFiftyFrame( bool newFiftyFrame );
	void	setSplit( float newSplit );
	void	setQuality( Quality newQuality );

	void	createVideo( QString filename, double frameCount, bool autoFindCodec );
	void	stop();
	void	stopCreating();
	void	kill();
	void	getFPS();
};