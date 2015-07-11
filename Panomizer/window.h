#pragma once

#include <QMainWindow>
#include "ui_window.h"
#include "panomizer.h"
#include <QThread>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QProcess>
#include <QSharedPointer>
#include <QTimer>
#include <QVector>
#include <QList>

typedef QSharedPointer<QWidget> QWidgetPtr;

class Window : public QMainWindow {
	Q_OBJECT

public:
	Window( QWidget *parent = 0 );
	~Window();

private:
	Ui::MainWindow ui;
	QWidgetPtr previewWidget;
	QLabel* previewLabel;
	QTimer timer;
	QString compressName, sound1, sound2, sound3, sound4;
	qint32 numCompletedAudio;
	int recordState;
	void removeSound();
	void addThread();
	int		get( QLineEdit & lineEdit );
	void	readSettings();
	QString animateString( QString s );

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
	void	saveFile();
	void	preview();
	void	setPercent( double );
	void	setImage( QPixmap );
	void	setError( QString );
	void	setStatus( QString e );
	void	compressButton();
	void	compress( QString in, QString out );
	void	convert( QString in, QString out );
	void	compressState( QProcess::ProcessState s );
	void	convertState( QProcess::ProcessState s );
	void	audioState( QProcess::ProcessState s );
	void	animate();
	QString getAudio( QString in, QString out, double offset = 0.0, double time = 0.0 );
signals:
	void initNames( QList< QString > names );
	void initOffsets( int globalOffset, QList< int > offsets );

	void createVideo( QString filename, double frameCount, bool highResolution, bool preview );
	void stop();
	void stopCreating();
	void kill();
	void getFPS();
};