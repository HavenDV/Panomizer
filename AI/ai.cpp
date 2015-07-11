#include "ai.h"
#include <QFileDialog>

AI::AI( QWidget *parent )
	: QMainWindow{ parent }, previewWidget{ new QWidget(), &QObject::deleteLater },
	timer{ this }, numCompletedAudio{ 0 } {
	ui.setupUi( this );
	connect( ui.pushButton, &QPushButton::clicked, this, &AI::pushButton1Clicked );
	connect( ui.pushButton_2, &QPushButton::clicked, this, &AI::pushButton2Clicked );
	connect( ui.pushButton_3, &QPushButton::clicked, this, &AI::pushButton3Clicked );
	connect( ui.pushButton_4, &QPushButton::clicked, this, &AI::compressButton );
	connect( ui.pushButton_7, &QPushButton::clicked, this, &AI::pushButton4Clicked );
	connect( ui.pushButton_5, &QPushButton::clicked, this, &AI::saveFile );
	connect( ui.pushButton_9, &QPushButton::clicked, this, &AI::kill );
	connect( ui.pushButton_10, &QPushButton::clicked, this, &AI::stopCreating );
	connect( ui.pushButton_11, &QPushButton::clicked, this, &AI::pushButton11Clicked );
	connect( ui.pushButton_12, &QPushButton::clicked, this, &AI::pushButton12Clicked );
	connect( ui.pushButton_13, &QPushButton::clicked, this, &AI::pushButton13Clicked );
	connect( ui.pushButton_14, &QPushButton::clicked, this, &AI::pushButton14Clicked );
	connect( ui.pushButton_15, &QPushButton::clicked, this, &AI::pushButton15Clicked );

	connect( ui.pushButton_16, &QPushButton::clicked, this, &AI::pushButton16Clicked );
	connect( ui.pushButton_17, &QPushButton::clicked, this, &AI::pushButton17Clicked );
	connect( ui.pushButton_18, &QPushButton::clicked, this, &AI::pushButton18Clicked );
	connect( ui.pushButton_19, &QPushButton::clicked, this, &AI::pushButton19Clicked );
	connect( ui.pushButton_20, &QPushButton::clicked, this, &AI::pushButton20Clicked );
	connect( ui.pushButton_21, &QPushButton::clicked, this, &AI::preview );

	connect( ui.lineEdit_7, &QLineEdit::returnPressed, this, &AI::initOffset );
	connect( ui.lineEdit_2, &QLineEdit::returnPressed, this, &AI::initOffset );
	connect( ui.lineEdit_3, &QLineEdit::returnPressed, this, &AI::initOffset );
	connect( ui.lineEdit_5, &QLineEdit::returnPressed, this, &AI::initOffset );
	connect( ui.lineEdit_11, &QLineEdit::returnPressed, this, &AI::initOffset );
	connect( ui.pushButton_8, &QPushButton::clicked, this, &AI::initOffset );

	ui.progressBar->setVisible( false );

	previewWidget->setFixedWidth( QApplication::desktop()->width() );
	previewWidget->setFixedHeight( QApplication::desktop()->width() / 4 / 1.333 );
	previewLabel = new QLabel( previewWidget.data() );

	readSettings();
	addThread();
	
	connect( &timer, &QTimer::timeout, this, &AI::animate );
	timer.start( 500 );

	sound1 = "sound1.temp.mp3";
	sound2 = "sound2.temp.mp3";
	sound3 = "sound3.temp.mp3";
	sound4 = "sound4.temp.mp3";
}

void AI::removeSound() {
	QFile::remove( sound1 );
	QFile::remove( sound2 );
	QFile::remove( sound3 );
	QFile::remove( sound4 );
}

AI::~AI() {
	emit stop();
	emit kill();

	if ( compressName != "" ) {
		QFile::remove( compressName + "temp.avi" );
	}
	removeSound();
}

void AI::readSettings() {
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

int AI::get( QLineEdit & lineEdit ) {
	if ( lineEdit.text().isEmpty() ) {
		lineEdit.setText( "0" );
	}
	return lineEdit.text().toInt();
}

void AI::init() {
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

void AI::initOffset() {
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

void AI::addThread() {
	auto thread = new QThread();
	auto object = new Panomizer();
	object->moveToThread( thread );

	qRegisterMetaType< QList< QString > >();
	qRegisterMetaType< QList< int > >();
	connect( this, &AI::initNames, object, &Panomizer::initNames );
	connect( this, &AI::initOffsets, object, &Panomizer::initOffsets );
	connect( object, &Panomizer::finished, thread, &QThread::quit );
	connect( object, &Panomizer::finished, object, &QObject::deleteLater );
	connect( thread, &QThread::finished, thread, &QThread::deleteLater );
	connect( this, &AI::createVideo, object, &Panomizer::createVideo );
	connect( this, &AI::stop, object, &Panomizer::stop );
	connect( this, &AI::stopCreating, object, &Panomizer::stopCreating );

	connect( this, &AI::getFPS, object, &Panomizer::sendFPS );
	connect( object, &Panomizer::returnMaxFPS, this, &AI::getSound );

	connect( object, &Panomizer::firstFrame, this, &AI::setImage );
	connect( object, &Panomizer::percent, this, &AI::setPercent );
	connect( object, &Panomizer::error, this, &AI::setError );
	connect( object, &Panomizer::status, this, &AI::setStatus );
	thread->start();

	init();
}

void AI::setImage( QPixmap data ) {
	previewLabel->setPixmap( data.scaled( previewWidget->size() ) );
	previewWidget->show();
}

void AI::pushButton1Clicked() {
	auto name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit->text() );
	if ( !name.isEmpty() ) {
		ui.lineEdit->setText( name );
		init();
	}
}

void AI::pushButton2Clicked() {
	QString name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit_4->text() );
	if ( name != "" )
		ui.lineEdit_4->setText( name );
	init();
}

void AI::pushButton3Clicked() {
	QString name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit_6->text() );
	if ( name != "" )
		ui.lineEdit_6->setText( name );
	init();
}

void AI::pushButton4Clicked() {
	QString name = QFileDialog::getOpenFileName( this, u8"Выберите файл", ui.lineEdit_10->text() );
	if ( name != "" )
		ui.lineEdit_10->setText( name );
	init();
}

void AI::addOne( QLineEdit* lineEdit ) {
	int value = lineEdit->text().toInt();
	lineEdit->setText( QString( "%1" ).arg( value + 1 ) );
	initOffset();
}

void AI::removeOne( QLineEdit* lineEdit ) {
	int value = lineEdit->text().toInt();
	lineEdit->setText( QString( "%1" ).arg( value - 1 ) );
	initOffset();
}

void AI::pushButton11Clicked() {
	addOne( ui.lineEdit_2 );
}

void AI::pushButton12Clicked() {
	addOne( ui.lineEdit_3 );
}

void AI::pushButton13Clicked() {
	addOne( ui.lineEdit_5 );
}

void AI::pushButton14Clicked() {
	addOne( ui.lineEdit_11 );
}

void AI::pushButton15Clicked() {
	addOne( ui.lineEdit_7 );
}

void AI::pushButton16Clicked() {
	removeOne( ui.lineEdit_2 );
}

void AI::pushButton17Clicked() {
	removeOne( ui.lineEdit_3 );
}

void AI::pushButton18Clicked() {
	removeOne( ui.lineEdit_5 );
}

void AI::pushButton19Clicked() {
	removeOne( ui.lineEdit_11 );
}

void AI::pushButton20Clicked() {
	removeOne( ui.lineEdit_7 );
}

void AI::saveFile() {
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

void AI::preview() {
	previewWidget->close();

	ui.pushButton_5->setEnabled( false );
	setStatus( u8"Предпросмотр..." );

	emit createVideo(
		"", ui.lineEdit_9->text().toInt(),
		ui.radioButton_2->isChecked(), true
	);
}

void AI::getSound( double maxFPS ) {
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

void AI::setPercent( double p ) {
	ui.progressBar->setVisible( true );
	ui.progressBar->setValue( p );
}

void AI::setError( QString e ) {
	if ( e == "offset" )	e = u8"Смещение превышает длительность файла.";
	else	if ( e == "input" )		e = u8"Один или несколько файлов не могут быть открыты. Выберите другие.";
	else	if ( e == "name/codec" ) e = u8"Ошибка с кодеками/проблема с именем. Установите кодеки/выберите другое имя.";
	ui.error->appendPlainText( e );
}

void AI::setStatus( QString e ) {
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

void AI::compressButton() {
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

void AI::compress( QString in, QString out ) {
	if ( in.isEmpty() || out.isEmpty() ) {
		return;
	}

	QString program{ "x264.exe" };
	QStringList arguments;
	arguments << "-o" << out << in;
	auto proc = new QProcess{ this };
	connect( this, &AI::kill, proc, &QProcess::kill );
	connect( proc, &QProcess::stateChanged, this, &AI::compressState );
	proc->start( program, arguments );
}

void AI::compressState( QProcess::ProcessState s ) {
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

void AI::convertState( QProcess::ProcessState s ) {
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

void AI::audioState( QProcess::ProcessState s ) {
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

void AI::animate() {
	ui.status->setText( animateString( ui.status->text() ) );
	ui.cstatus->setText( animateString( ui.cstatus->text() ) );
}

QString AI::animateString( QString s ) {
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

void AI::convert( QString in, QString out ) {
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
	connect( this, &AI::kill, process, &QProcess::kill );
	connect( process, &QProcess::stateChanged, this, &AI::convertState );
	process->start( program, arguments );
}

QString AI::getAudio( QString in, QString out, double offset, double time ) {
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
	connect( this, &AI::kill, process, &QProcess::kill );
	connect( process, &QProcess::stateChanged, this, &AI::audioState );
	process->start( program, arguments );

	return out;
}