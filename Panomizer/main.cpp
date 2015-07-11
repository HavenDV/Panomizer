#include <QCoreApplication>
#include <QApplication>
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
	window.move( 600, 400 );
	window.show();
	window.setFixedSize( window.size() );
	return a.exec();
}