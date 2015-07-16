#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>
#include "window.h"

int main( int argc, char *argv[] ) {
	auto paths = QCoreApplication::libraryPaths();
	paths.append( "." );
	paths.append( "platforms" );
	QCoreApplication::setLibraryPaths( paths );
	QCoreApplication::setOrganizationName( "h Soft" );
	QCoreApplication::setApplicationName( "Panomizer" );

	QApplication a{ argc, argv };
	a.setQuitOnLastWindowClosed( true );
	a.setWindowIcon( QIcon( ":/Icons/Resources/H16.png" ) );

	Window window;
	auto desktopSize = QApplication::desktop()->size();
	window.move( desktopSize.width() / 2 - window.width() / 2, desktopSize.height() / 2 - window.height() / 2 );
	window.show();
	window.setFixedSize( window.size() );

	return a.exec();
}