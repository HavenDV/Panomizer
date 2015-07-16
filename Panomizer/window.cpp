#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QSettings>
#include <QThread>
#include <QHBoxLayout>
#include <QMessageBox>
#include "window.h"

constexpr auto saveText = u8"Сохранить как";
constexpr auto selectText = u8"Выберите файл";
constexpr auto selectVideoFilterText = u8"Видео (*.avi *.mp4 *.mkv *.mov)";
constexpr auto saveVideoFilterText = u8"Видео (*.mkv)";
constexpr auto defaultSavePathText = "C:/untitled.mkv";

Window::Window( QWidget *parent )
	: QMainWindow{ parent }, //, previewWidget{ new QWidget(), &QObject::deleteLater },
	timer{ this }, numCompletedAudio{ 0 },
	screenWidth{ 0 } {
	ui.setupUi( this );
	/*
	auto newFileButton = new QPushButton{ u8"Добавить новый файл", ui.fileTable };
	newFileButton->resize( ui.fileTable->width() - 10, 30 );
	newFileButton->move( 5, ui.fileTable->height() - newFileButton->height() - 5 );
	connect( newFileButton, &QPushButton::clicked, this, &Window::newFile );
	*/
	connect( ui.pushButton, &QPushButton::clicked, this, &Window::pushButton1Clicked );
	connect( ui.pushButton_2, &QPushButton::clicked, this, &Window::pushButton2Clicked );
	connect( ui.pushButton_3, &QPushButton::clicked, this, &Window::pushButton3Clicked );
	connect( ui.compressButton, &QPushButton::clicked, this, &Window::compressButton );
	connect( ui.pushButton_7, &QPushButton::clicked, this, &Window::pushButton4Clicked );
	connect( ui.createButton, &QPushButton::clicked, this, &Window::saveFile );
	connect( ui.stopButton, &QPushButton::clicked, this, &Window::stopButton );
	connect( ui.pushButton_11, &QPushButton::clicked, this, &Window::pushButton11Clicked );
	connect( ui.pushButton_12, &QPushButton::clicked, this, &Window::pushButton12Clicked );
	connect( ui.pushButton_13, &QPushButton::clicked, this, &Window::pushButton13Clicked );
	connect( ui.pushButton_14, &QPushButton::clicked, this, &Window::pushButton14Clicked );
	connect( ui.pushButton_15, &QPushButton::clicked, this, &Window::pushButton15Clicked );

	connect( ui.addFiftyFrameCheckBox, &QCheckBox::stateChanged, this, &Window::addFiftyFrameState );
	connect( ui.splitValueSlider, &QSlider::sliderReleased, this, &Window::sendSplit );
	connect( ui.splitValueSlider, &QSlider::sliderPressed, this, &Window::sendSplit );
	connect( ui.splitValueSlider, &QSlider::actionTriggered, this, &Window::splitState );
	connect( ui.highQualityRadioButton, &QRadioButton::clicked, this, &Window::qualityState );
	connect( ui.mediumQualityRadioButton, &QRadioButton::clicked, this, &Window::qualityState );
	connect( ui.lowQualityRadioButton, &QRadioButton::clicked, this, &Window::qualityState );

	connect( ui.pushButton_16, &QPushButton::clicked, this, &Window::pushButton16Clicked );
	connect( ui.pushButton_17, &QPushButton::clicked, this, &Window::pushButton17Clicked );
	connect( ui.pushButton_18, &QPushButton::clicked, this, &Window::pushButton18Clicked );
	connect( ui.pushButton_19, &QPushButton::clicked, this, &Window::pushButton19Clicked );
	connect( ui.pushButton_20, &QPushButton::clicked, this, &Window::pushButton20Clicked );
	connect( ui.previewButton, &QPushButton::clicked, this, &Window::preview );
	connect( ui.helpButton, &QPushButton::clicked, this, &Window::help );

	connect( ui.lineEdit_7, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_2, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_3, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_5, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.lineEdit_11, &QLineEdit::returnPressed, this, &Window::initOffset );
	connect( ui.pushButton_8, &QPushButton::clicked, this, &Window::initOffset );

	ui.progressBar->setVisible( false );

	//previewWidget->setFixedWidth( QApplication::desktop()->width() );
	//previewWidget->setFixedHeight( QApplication::desktop()->width() / 5 / 1.333 );
	//previewLabel = new QLabel{ previewWidget.data() };
	screenWidth = QApplication::desktop()->width();

	readSettings();
	addThread();
	
	connect( &timer, &QTimer::timeout, this, &Window::animate );
	timer.start( 500 );

	sound1 = "sound1.temp.mp3";
	sound2 = "sound2.temp.mp3";
	sound3 = "sound3.temp.mp3";
	sound4 = "sound4.temp.mp3";
}

void	Window::help() {
	auto helpBox = new QMessageBox{ 
		{}, u8"Справка", ui.helpButton->toolTip(),
		QMessageBox::Button::Ok, this 
	};
	helpBox->show();
}

void	Window::removeSound() {
	QFile::remove( sound1 );
	QFile::remove( sound2 );
	QFile::remove( sound3 );
	QFile::remove( sound4 );
}

Window::~Window() {
	emit stop();
	emit kill();

	if ( !compressName.isEmpty() ) {
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
	auto object = new Panomizer( screenWidth );
	object->moveToThread( thread );

	qRegisterMetaType< QList< QString > >();
	qRegisterMetaType< QList< int > >();
	qRegisterMetaType< Quality >();
	connect( this, &Window::initNames, object, &Panomizer::initNames );
	connect( this, &Window::initOffsets, object, &Panomizer::initOffsets );
	connect( object, &Panomizer::finished, thread, &QThread::quit );
	connect( object, &Panomizer::finished, object, &QObject::deleteLater );
	connect( thread, &QThread::finished, thread, &QThread::deleteLater );
	connect( this, &Window::createVideo, object, &Panomizer::createVideo );
	connect( this, &Window::stop, object, &Panomizer::stop );
	connect( this, &Window::stopCreating, object, &Panomizer::stopCreating );

	connect( this, &Window::getFPS, object, &Panomizer::sendMaxFPS );
	connect( this, &Window::setAddFiftyFrame, object, &Panomizer::setAddFiftyFrame );
	connect( this, &Window::setSplit, object, &Panomizer::setSplit );
	connect( this, &Window::setQuality, object, &Panomizer::setQuality );

	connect( object, &Panomizer::returnMaxFPS, this, &Window::getSound );
	connect( object, &Panomizer::returnFrameCount, this, &Window::setFrameCount );
	connect( object, &Panomizer::created, this, &Window::afterCreated );
	connect( object, &Panomizer::percent, this, &Window::setPercent );
	connect( object, &Panomizer::error, this, &Window::setError );
	connect( object, &Panomizer::status, this, &Window::setStatus );
	thread->start();

	init();
}

void Window::pushButton1Clicked() {
	auto name = QFileDialog::getOpenFileName( this, selectText, ui.lineEdit->text() );
	if ( !name.isEmpty() ) {
		ui.lineEdit->setText( name );
		init();
	}
}

void Window::pushButton2Clicked() {
	QString name = QFileDialog::getOpenFileName( this, selectText, ui.lineEdit_4->text() );
	if ( name != "" )
		ui.lineEdit_4->setText( name );
	init();
}

void Window::pushButton3Clicked() {
	QString name = QFileDialog::getOpenFileName( this, selectText, ui.lineEdit_6->text() );
	if ( name != "" )
		ui.lineEdit_6->setText( name );
	init();
}

void Window::pushButton4Clicked() {
	QString name = QFileDialog::getOpenFileName( this, selectText, ui.lineEdit_10->text() );
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
/*
#include <QMap>
QMap< QString, QString > files;

void	Window::deleteFile() {
	auto button = static_cast< QPushButton* >( sender() );
	auto name = button->objectName();

	for ( auto i = 0; i < ui.fileTable->rowCount(); ++i ) {
		auto widget = ui.fileTable->cellWidget( i, 0 );
		if ( widget->objectName() == name ) {
			ui.fileTable->removeRow( i );
			widget->deleteLater();
		}
	}
	files.remove( name );
}

void	Window::newFile() {
	auto filename = QFileDialog::getOpenFileName(
		this, selectText, 0, selectVideoFilterText
	);
	if ( filename.isEmpty() ) {
		return;
	}
	
	ui.fileTable->setRowCount( files.size() + 1 );
	ui.fileTable->setColumnCount( 2 );
	ui.fileTable->setColumnWidth( 0, ui.fileTable->width() - 40 );
	ui.fileTable->setColumnWidth( 1, 20 );

	auto widget = new QWidget{ ui.fileTable };
	widget->setObjectName( filename );
	auto lineEdit = new QLineEdit{ filename, widget };
	auto layout = new QHBoxLayout{ widget };
	layout->addWidget( lineEdit );
	layout->setContentsMargins( 5, 5, 5, 5 );
	widget->setLayout( layout );

	auto deleteWidget = new QWidget{ ui.fileTable };
	auto deleteButton = new QPushButton{ "x", deleteWidget };
	deleteButton->setObjectName( filename );
	connect( deleteButton, &QPushButton::clicked, this, &Window::deleteFile );
	auto deleteLayout = new QHBoxLayout{ deleteWidget };
	deleteLayout->addWidget( deleteButton );
	//layout->setAlignment( Qt::AlignCenter );
	deleteLayout->setContentsMargins( 0, 0, 0, 0 );
	deleteWidget->setLayout( layout );
	
	ui.fileTable->setCellWidget( files.size(), 0, widget );
	ui.fileTable->setCellWidget( files.size(), 1, deleteWidget );

	files[ filename ] = filename;
}
*/

auto	Window::getFrameCount() -> int {
	return get( *ui.frameCountLineEdit );
}

auto	Window::getSplitValue() -> float {
	return ui.splitValueSlider->value() / 100.0F;
}

auto	Window::isHighQuality() -> bool {
	return ui.highQualityRadioButton->isChecked();
}

auto	Window::isAutoSelectCodec() -> bool {
	return ui.autoSelectCodecCheckBox->isChecked();
}

auto	Window::isAddFiftyFrame() -> bool {
	return ui.addFiftyFrameCheckBox->isChecked();
}

auto	Window::isAutoCompress() -> bool {
	return ui.autoCompessCheckBox->isChecked();
}

auto	Window::isSoundEnabled() -> bool {
	return ui.enableSoundCheckBox->isChecked();
}

// If filename is empty, video create in preview mode.
void	Window::create( const QString & filename ) {
	emit createVideo(
		filename, getFrameCount(), isAutoSelectCodec()
	);
}

void Window::saveFile() {
	numCompletedAudio = 0;
	removeSound();
	auto name = QFileDialog::getSaveFileName( 
		this, saveText, defaultSavePathText, saveVideoFilterText
	);
	if ( name.isEmpty() ) {
		return;
	}

	compressName = name;
	disableAll();
	setStatus( u8"Файл создается. Пожалуйста, подождите..." );

	if ( isSoundEnabled() ) {
		emit getFPS();
	}
	else {
		create( name + "temp.avi" );
	}
}

void Window::preview() {
	disableButtons();
	setStatus( u8"Предпросмотр..." );
	create( "" );
}

void Window::getSound( double maxFPS ) {
	auto offset = get( *ui.lineEdit_7 );
	auto offset1 = ( offset + get( *ui.lineEdit_2 ) ) / maxFPS;
	auto offset2 = ( offset + get( *ui.lineEdit_3 ) ) / maxFPS;
	auto offset3 = ( offset + get( *ui.lineEdit_5 ) ) / maxFPS;
	auto offset4 = ( offset + get( *ui.lineEdit_11 ) ) / maxFPS;

	auto time = getFrameCount();
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

void Window::setFrameCount( double frameCount ) {
	ui.frameCountLineEdit->setText( QString( "%1" ).arg( frameCount ) );
}

void Window::setError( QString text ) {
	if ( text == "offset" ) {
		text = u8"Смещение превышает длительность файла.";
	}
	else if ( text == "input" ) {
		text = u8"Один или несколько файлов не могут быть открыты. Выберите другие.";
	}
	else if ( text == "name/codec" ) {
		text = u8"Ошибка с кодеками/проблема с именем. Установите кодеки/выберите другое имя.";
	}
	ui.error->appendPlainText( text );

	reset();
}

void Window::setStatus( const QString & text ) {
	ui.statusLabel->setText( text );
}

void Window::sendSplit() {
	emit setSplit( getSplitValue() );
}

void	Window::afterCreated() {
	ui.compressButton->setEnabled( true );
	if ( isAutoCompress() && isHighQuality() ) {
		setStatus( u8"Видео создано. Происходит сжатие. Пожалуйста, подождите..." );
		compress( compressName + "temp.avi", compressName + "temp.mkv" );
	}
	else {
		convert( compressName + "temp.avi", compressName );
	}
}

void Window::compressButton() {
	auto && openFileName = QFileDialog::getOpenFileName( 
		this, selectText, 0, selectVideoFilterText
	);
	if ( !openFileName.isEmpty() ) {
		auto && saveFileName = QFileDialog::getSaveFileName( 
			this, saveText, defaultSavePathText, selectVideoFilterText
		);
		compress( openFileName, saveFileName );
	}
}

void	Window::enableButtons() {
	ui.createButton->setEnabled( true );
	ui.previewButton->setEnabled( true );
	ui.compressButton->setEnabled( true );
}

void	Window::disableButtons() {
	ui.createButton->setEnabled( false );
	ui.previewButton->setEnabled( false );
	ui.compressButton->setEnabled( false );
}

void	Window::enableAll() {
	ui.settingGroupBox->setEnabled( true );
	enableButtons();
}

void	Window::disableAll() {
	ui.settingGroupBox->setEnabled( false );
}

void	Window::removeTempFiles() {
	if ( !compressName.isEmpty() ) {
		QFile::remove( compressName + "temp.avi" );
		QFile::remove( compressName + "temp.mkv" );
	}
	removeSound();
}

void	Window::reset() {
	emit kill();
	emit stopCreating();
	removeTempFiles();
	enableAll();
	setPercent( 0.0 );
	setStatus( "" );
}

void	Window::stopButton() {
	reset();
	setPercent( 0.0 );
	setStatus( u8"Отменено." );
}

void	Window::compress( QString in, QString out ) {
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
		enableButtons();
		setPercent( 99.0 );
		convert( compressName + "temp.mkv", compressName );
	}
	else if ( s == 2 ) {
		disableButtons();
		setPercent( 99.0 );
		setStatus( u8"Сжатие. Пожалуйста, подождите..." );
	}
}

void Window::convertState( QProcess::ProcessState s ) {
	if ( s == 0 ) {
		reset();
		setStatus( u8"Видео создано." );
		setPercent( 100.0 );
	}
	else if ( s == 2 ) {
		setStatus( u8"Конвертация..." );
		setPercent( 99.0 );
	}
}

void	Window::audioState( QProcess::ProcessState s ) {
	if ( s == 0 ) {
		++numCompletedAudio;
	}

	if ( numCompletedAudio > 3 ) {
		create( compressName + "temp.avi" );
	}
}

void Window::addFiftyFrameState( int state ) {
	emit setAddFiftyFrame( state == Qt::CheckState::Checked ? true : false );
}

void Window::splitState( int state ) {
	if ( state == QAbstractSlider::SliderMove ) {
		return;
	}

	sendSplit();
}

void Window::qualityState( bool checked ) {
	auto isMedium = ui.mediumQualityRadioButton->isChecked();
	emit setQuality( isHighQuality() ? Quality::High : isMedium ? Quality::Medium : Quality::Low );
}

void Window::animate() {
	ui.statusLabel->setText( animateString( ui.statusLabel->text() ) );
}

//swap "xxx..." to "xxx   ", "xxx   " to "xxx.  ", "xxx.   " to "xxx.. "
auto	Window::animateString( QString s ) -> QString {
	if ( s.size() < 3 ) {
		return s;
	}

	if ( s.at( s.size() - 3 ) == '.' ) {
		if ( s.at( s.size() - 2 ) == '.' ) {
			if ( s.at( s.size() - 1 ) == '.' ) {
				s.replace( s.size() - 1, 1, ' ' );
				s.replace( s.size() - 2, 1, ' ' );
				s.replace( s.size() - 3, 1, ' ' );
			}
			else if ( s.at( s.size() - 1 ) == ' ' ) {
				s.replace( s.size() - 1, 1, '.' );
			}
		}
		else if ( s.at( s.size() - 2 ) == ' ' ) {
			s.replace( s.size() - 2, 1, '.' );
		}
	}
	else if ( s.at( s.size() - 3 ) == ' ' ) {
		s.replace( s.size() - 3, 1, '.' );
	}
	return s;
}

void	Window::convert( QString in, QString out ) {
	if ( in.isEmpty() || out.isEmpty() ) {
		return;
	}

	auto program = "mkvmerge.exe";
	QStringList arguments;
	arguments << "-o" << out << in;
	if ( isSoundEnabled() ) {
		arguments << sound1 << sound2 << sound3 << sound4;
	}

	auto process = new QProcess( this );
	connect( this, &Window::kill, process, &QProcess::kill );
	connect( process, &QProcess::stateChanged, this, &Window::convertState );
	process->start( program, arguments );
}

auto	Window::getAudio( QString in, QString out, double offset, double time ) -> QString {
	if ( in.isEmpty() || out.isEmpty() ) {
		return{};
	}

	auto program = "ffmpeg.exe";
	QStringList arguments;
	arguments << "-i" << in;
	arguments << "-vn" << "-ar" << "44100" << "-ac" << "2" << "-ab" << "192" << "-f" << "mp3";
	arguments << "-ss" << QString( "%1" ).arg( offset );
	if ( time > 0.1 ) {
		arguments << "-t" << QString( "%1" ).arg( time );
	}
	arguments << out;

	auto process = new QProcess{ this };
	connect( this, &Window::kill, process, &QProcess::kill );
	connect( process, &QProcess::stateChanged, this, &Window::audioState );
	process->start( program, arguments );

	return out;
}