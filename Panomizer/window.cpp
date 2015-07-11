#include "window.h"
#include <QFileDialog>

Window::Window( QWidget *parent )
	: QMainWindow{ parent }, previewWidget{ new QWidget(), &QObject::deleteLater },
	timer{ this }, numCompletedAudio{ 0 } {
	ui.setupUi( this );
	connect( ui.pushButton, &QPushButton::clicked, this, &Window::pushButton1Clicked );
	connect( ui.pushButton_2, &QPushButton::clicked, this, &Window::pushButton2Clicked );
	connect( ui.pushButton_3, &QPushButton::clicked, this, &Window::pushButton3Clicked );
	connect( ui.pushButton_4, &QPushButton::clicked, this, &Window::compressButton );
	connect( ui.pushButton_7, &QPushButton::clicked, this, &Window::pushButton4Clicked );
	connect( ui.pushButton_5, &QPushButton::clicked, this, &Window::saveFile );
	connect( ui.pushButton_9, &QPushButton::clicked, this, &Window::kill );
	connect( ui.pushButton_10, &QPushButton::clicked, this, &Window::stopCreating );
	connect( ui.pushButton_11, &QPushButton::clicked, this, &Window::pushButton11Clicked );
	connect( ui.pushButton_12, &QPushButton::clicked, this, &Window::pushButton12Clicked );
	connect( ui.pushButton_13, &QPushButton::clicked, this, &Window::pushButton13Clicked );
	connect( ui.pushButton_14, &QPushButton::clicked, this, &Window::pushButton14Clicked );
	connect( ui.pushButton_15, &QPushButton::clicked, this, &Window::pushButton15Clicked );

	connect( ui.pushButton_16, &QPushButton::clicked, this, &Window::pushButton16Clicked );
	connect( ui.pushButton_17, &QPushButton::clicked, this, &Window::pushButton17Clicked );
	connect( ui.pushButton_18, &QPushButton::clicked, this, &Window::pushButton18Clicked );
	connect( ui.pushButton_19, &QPushButton::clicked, this, &Window::pushButton19Clicked );
	connect( ui.pushButton_20, &QPushButton::clicked, this, &Window::pushButton20Clicked );
	connect( ui.pushButton_21, &QPushButton::clicked, this, &Window::preview );

	connect( ui.lineEdit_7, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_2, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_3, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_5, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_11, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.pushButton_8, &QPushButton::clicked, this, &Window::initOffset );

	ui.progressBar->setVisible( false );

	previewWidget->setFixedWidth( QApplication::desktop()->width() );
	previewWidget->setFixedHeight( QApplication::desktop()->width() / 4 / 1.333 );
	previewLabel = new QLabel( previewWidget.data() );

	readSettings();
	addThread();
	
	connect( &timer, &QTimer::timeout, this, &Window::animate );
	timer.start( 500 );

	sound1 = "sound1.temp.mp3";
	sound2 = "sound2.temp.mp3";
	sound3 = "sound3.temp.mp3";
	sound4 = "sound4.temp.mp3";
}

void Window::removeSound() {
	QFile::remove( sound1 );
	QFile::remove( sound2 );
	QFile::remove( sound3 );
	QFile::remove( sound4 );
}

Window::~Window() {
	emit stop();
	emit kill();

	if ( compressName != "" ) {
		QFile::remove( compressName + "temp.avi" );
	}
	removeSound();
}

void Window::readSettings() {
	QSettings settings;

	settings.beginGroup( "names" );
	ui.lineEdit->setText( settings.value( "name1" ).toString() );
	ui.lineEdit_4->setText( settings.value( "name2" ).toString() );
	ui.lineEdit_6->setText( settings.value( "name3" ).toString() );
	ui.lineEdit_10->setText( settings.value( "name4" ).toString() );
	settings.endGroup();

	settings.beginGroup( "offsets" );
	ui.lineEdit_7->setText( settings.value( "globalOffset" ).toString() );
	ui.lineEdit_2->setText( settings.value( "offset1" ).toString() );
	ui.lineEdit_3->setText( settings.value( "offset2" ).toString() );
	ui.lineEdit_5->setText( settings.value( "offset3" ).toString() );
	ui.lineEdit_11->setText( settings.value( "offset4" ).toString() );
	settings.endGroup();
}

int Window::get( QLineEdit & lineEdit ) {
	if ( lineEdit.text().isEmpty() ) {
		lineEdit.setText( "0" );
	}
	return lineEdit.text().toInt();
}

void Window::init() {
	auto name1 = ui.lineEdit->text();
	auto name2 = ui.lineEdit_4->text();
	auto name3 = ui.lineEdit_6->text();
	auto name4 = ui.lineEdit_10->text();

	if ( name1.isEmpty() || name2.isEmpty() ||
		 name3.isEmpty() || name4.isEmpty() ) {
		return;
	}

	QSettings settings;
	settings.beginGroup( "names" );
	settings.setValue( "name1", name1 );
	settings.setValue( "name2", name2 );
	settings.setValue( "name3", name3 );
	settings.setValue( "name4", name4 );
	settings.endGroup();

	emit initNames( { name1, name2, name3, name4 } );
	initOffset();
}

void Window::initOffset() {
	auto globalOffset = get( *ui.lineEdit_7 );
	auto offset1 = get( *ui.lineEdit_2 );
	auto offset2 = get( *ui.lineEdit_3 );
	auto offset3 = get( *ui.lineEdit_5 );
	auto offset4 = get( *ui.lineEdit_11 );

	QSettings settings;
	settings.beginGroup( "offsets" );
	settings.setValue( "globalOffset", globalOffset );
	settings.setValue( "offset1", offset1 );
	settings.setValue( "offset2", offset2 );
	settings.setValue( "offset3", offset3 );
	settings.setValue( "offset4", offset4 );
	settings.endGroup();

	emit initOffsets( globalOffset, { offset1, offset2, offset3, offset4 } );
}

void Window::addThread() {
	auto thread = new QThread();
	auto object = new Panomizer();
	object->moveToThread( thread );

	qRegisterMetaType< QList< QString > >();
	qRegisterMetaType< QList< int > >();
	connect( this, &Window::initNames, object, &Panomizer::initNames );
	connect( this, &Window::initOffsets, object, &Panomizer::initOffsets );
	connect( object, &Panomizer::finished, thread, &QThread::quit );
	connect( object, &Panomizer::finished, object, &QObject::deleteLater );
	connect( thread, &QThread::finished, thread, &QThread::deleteLater );
	connect( this, &Window::createVideo, object, &Panomizer::createVideo );
	connect( this, &Window::stop, object, &Panomizer::stop );
	connect( this, &Window::stopCreating, object, &Panomizer::stopCreating );

	connect( this, &Window::getFPS, object, &Panomizer::sendMaxFPS );
	connect( object, &Panomizer::returnMaxFPS, this, &Window::getSound );

	connect( object, &Panomizer::firstFrame, this, &Window::setImage );
	connect( object, &Panomizer::percent, this, &Window::setPercent );
	connect( object, &Panomizer::error, this, &Window::setError );
	connect( object, &Panomizer::status, this, &Window::setStatus );
	thread->start();

	init();
}

void Window::setImage( QPixmap data ) {
	previewLabel->setPixmap( data.scaled( previewWidget->size() ) );
	previewWidget->show();
}

void Window::pushButton1Clicked() {
	auto name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit->text() );
	if ( !name.isEmpty() ) {
		ui.lineEdit->setText( name );
		init();
	}
}

void Window::pushButton2Clicked() {
	QString name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit_4->text() );
	if ( name != "" )
		ui.lineEdit_4->setText( name );
	init();
}

void Window::pushButton3Clicked() {
	QString name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit_6->text() );
	if ( name != "" )
		ui.lineEdit_6->setText( name );
	init();
}

void Window::pushButton4Clicked() {
	QString name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit_10->text() );
	if ( name != "" )
		ui.lineEdit_10->setText( name );
	init();
}

void Window::addOne( QLineEdit* lineEdit ) {
	int value = lineEdit->text().toInt();
	lineEdit->setText( QString( "%1" ).arg( value + 1 ) );
	initOffset();
}

void Window::removeOne( QLineEdit* lineEdit ) {
	int value = lineEdit->text().toInt();
	lineEdit->setText( QString( "%1" ).arg( value - 1 ) );
	initOffset();
}

void Window::pushButton11Clicked() {
	addOne( ui.lineEdit_2 );
}

void Window::pushButton12Clicked() {
	addOne( ui.lineEdit_3 );
}

void Window::pushButton13Clicked() {
	addOne( ui.lineEdit_5 );
}

void Window::pushButton14Clicked() {
	addOne( ui.lineEdit_11 );
}

void Window::pushButton15Clicked() {
	addOne( ui.lineEdit_7 );
}

void Window::pushButton16Clicked() {
	removeOne( ui.lineEdit_2 );
}

void Window::pushButton17Clicked() {
	removeOne( ui.lineEdit_3 );
}

void Window::pushButton18Clicked() {
	removeOne( ui.lineEdit_5 );
}

void Window::pushButton19Clicked() {
	removeOne( ui.lineEdit_11 );
}

void Window::pushButton20Clicked() {
	removeOne( ui.lineEdit_7 );
}

void Window::saveFile() {
	numCompletedAudio = 0;
	removeSound();
	previewWidget->close();
	QString name = QFileDialog::getSaveFileName( this, u8"Сохранить как",
												 "C:/untitled.mkv",
												 u8"Видео (*.mkv)" );
	if ( name == "" ) return;
	compressName = name;
	ui.pushButton_5->setEnabled( false );
	setStatus( u8"Файл создается. Пожалуйста, подождите..." );

	if ( ui.checkBox_2->isChecked() )
		emit createVideo( 
			name + "temp.avi", 
			ui.lineEdit_9->text().toInt(), 
			ui.radioButton_2->isChecked(), 
			false 
		);
	else {
		emit getFPS();
	}
}

void Window::preview() {
	previewWidget->close();

	ui.pushButton_5->setEnabled( false );
	setStatus( u8"Предпросмотр..." );

	emit createVideo(
		"", ui.lineEdit_9->text().toInt(),
		ui.radioButton_2->isChecked(), true
	);
}

void Window::getSound( double maxFPS ) {
	auto offset = get( *ui.lineEdit_7 );
	auto offset1 = ( offset + get( *ui.lineEdit_2 ) ) / maxFPS;
	auto offset2 = ( offset + get( *ui.lineEdit_3 ) ) / maxFPS;
	auto offset3 = ( offset + get( *ui.lineEdit_5 ) ) / maxFPS;
	auto offset4 = ( offset + get( *ui.lineEdit_11 ) ) / maxFPS;

	auto time = ui.lineEdit_9->text().toInt();
	auto time1 = time / maxFPS;
	auto time2 = time / maxFPS;
	auto time3 = time / maxFPS;
	auto time4 = time / maxFPS;

	getAudio( ui.lineEdit->text(), sound1, offset1, time1 );
	getAudio( ui.lineEdit_4->text(), sound2, offset2, time2 );
	getAudio( ui.lineEdit_6->text(), sound3, offset3, time3 );
	getAudio( ui.lineEdit_10->text(), sound4, offset4, time4 );
}

void Window::setPercent( double p ) {
	ui.progressBar->setVisible( true );
	ui.progressBar->setValue( p );
}

void Window::setError( QString e ) {
	if ( e == "offset" )	e = u8"Смещение превышает длительность файла.";
	else	if ( e == "input" )		e = u8"Один или несколько файлов не могут быть открыты. Выберите другие.";
	else	if ( e == "name/codec" ) e = u8"Ошибка с кодеками/проблема с именем. Установите кодеки/выберите другое имя.";
	ui.error->appendPlainText( e );
}

void Window::setStatus( QString e ) {
	if ( e != "compresse"&&e != "compressed" )
		ui.pushButton_5->setEnabled( true );
	if ( e == "created" ) {
		if ( ui.checkBox->isChecked() && ui.radioButton_2->isChecked() ) {
			ui.status->setText( u8"Видео создано. Происходит сжатие. Пожалуйста, подождите..." );
			compress( compressName + "temp.avi", compressName + "temp.mkv" );
			//convert(compressName+"temp.avi", compressName+"temp1.avi");
		}
		else {
			convert( compressName + "temp.avi", compressName );
		}
	}
	else	if ( e == "stopped" ) {
		ui.status->setText( u8"Отменено." );
		removeSound();
		setPercent( 0.0 );
	}
	else						ui.status->setText( e );
}

void Window::compressButton() {
	auto openFileName = QFileDialog::getOpenFileName( 
		this, u8"Выберите файл для сжатия", 0, 
		u8"Видео (*.avi *.mp4 *.mkv *.mov)" 
	);
	if ( openFileName != "" ) {
		auto saveFileName = QFileDialog::getSaveFileName( 
			this, u8"Сохранить как", "C:/untitled.mkv", 
			u8"Видео (*.avi *.mp4 *.mkv *.mov)" 
		);
		compress( openFileName, saveFileName );
	}
}

void Window::compress( QString in, QString out ) {
	if ( in.isEmpty() || out.isEmpty() ) {
		return;
	}

	QString program{ "x264.exe" };
	QStringList arguments;
	arguments << "-o" << out << in;
	auto proc = new QProcess{ this };
	connect( this, &Window::kill, proc, &QProcess::kill );
	connect( proc, &QProcess::stateChanged, this, &Window::compressState );
	proc->start( program, arguments );
}

void Window::compressState( QProcess::ProcessState s ) {
	if ( s == 0 ) {
		ui.pushButton_4->setEnabled( true );
		ui.pushButton_4->setText( u8"Открыть" );
		ui.cstatus->setText( u8"Сжатие завершено." );
		setPercent( 100.0 );
		//removeSound();
		QFile::remove( compressName + "temp.avi" );
		convert( compressName + "temp.mkv", compressName );
		//setStatus( u8"Видео создано." );
	}
	else if ( s == 2 ) {
		ui.pushButton_4->setEnabled( false );
		ui.pushButton_4->setText( u8"Сжатие..." );
		setPercent( 99.0 );
		setStatus( u8"Сжатие. Пожалуйста, подождите..." );
	}
}

void Window::convertState( QProcess::ProcessState s ) {
	if ( s == 0 ) {
		setStatus( u8"Видео создано." );
		setPercent( 100.0 );
		QFile::remove( compressName + "temp.avi" );
		QFile::remove( compressName + "temp.mkv" );
		removeSound();
	}
	else if ( s == 2 ) {
		setStatus( u8"Конвертация..." );
		setPercent( 99.0 );
	}
}

void Window::audioState( QProcess::ProcessState s ) {
	if ( s == 0 ) {
		++numCompletedAudio;
	}

	if ( numCompletedAudio > 3 ) {
		emit createVideo(
			compressName + "temp.avi", ui.lineEdit_9->text().toInt(),
			ui.radioButton_2->isChecked(),
			false
		);
	}
}

void Window::animate() {
	ui.status->setText( animateString( ui.status->text() ) );
	ui.cstatus->setText( animateString( ui.cstatus->text() ) );
}

QString Window::animateString( QString s ) {
	if ( s.size() < 3 ) return{};
	if ( s.at( s.size() - 3 ) == '.' ) {
		if ( s.at( s.size() - 2 ) == '.' ) {
			if ( s.at( s.size() - 1 ) == '.' ) {
				s.replace( s.size() - 1, 1, ' ' );
				s.replace( s.size() - 2, 1, ' ' );
				s.replace( s.size() - 3, 1, ' ' );
			}
			else if ( s.at( s.size() - 1 ) == ' ' ) s.replace( s.size() - 1, 1, '.' );
		}
		else if ( s.at( s.size() - 2 ) == ' ' ) s.replace( s.size() - 2, 1, '.' );
	}
	else if ( s.at( s.size() - 3 ) == ' ' ) s.replace( s.size() - 3, 1, '.' );
	return s;
}

void Window::convert( QString in, QString out ) {
	if ( in.isEmpty() || out.isEmpty() ) {
		return;
	}

	QString program = "mkvmerge.exe";
	QStringList arguments;
	arguments << "-o" << out << in;
	if ( !( ui.checkBox_2->isChecked() ) ) {
		arguments << sound1 << sound2 << sound3 << sound4;
	}

	auto process = new QProcess( this );
	connect( this, &Window::kill, process, &QProcess::kill );
	connect( process, &QProcess::stateChanged, this, &Window::convertState );
	process->start( program, arguments );
}

QString Window::getAudio( QString in, QString out, double offset, double time ) {
	if ( in.isEmpty() || out.isEmpty() ) {
		return{};
	}

	QString program = "ffmpeg.exe";
	QStringList arguments;
	arguments << "-i" << in;
	arguments << "-vn" << "-ar" << "44100" << "-ac" << "2" << "-ab" << "192" << "-f" << "mp3";
	arguments << "-ss" << QString( "%1" ).arg( offset );
	if ( time > 0.1 ) arguments << "-t" << QString( "%1" ).arg( time );
	arguments << out;

	auto process = new QProcess( this );
	connect( this, &Window::kill, process, &QProcess::kill );
	connect( process, &QProcess::stateChanged, this, &Window::audioState );
	process->start( program, arguments );

	return out;
}