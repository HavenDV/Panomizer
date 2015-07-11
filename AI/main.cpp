#include "ai.h"
#include <QCoreApplication>
#include <QApplication>
#include <QIcon>

int main( int argc, char *argv[] ) {
	auto paths = QCoreApplication::libraryPaths();
	paths.append( "." );
	paths.append( "platforms" );
	QCoreApplication::setLibraryPaths( paths );
	QCoreApplication::setOrganizationName( "h Soft" );
	QCoreApplication::setApplicationName( "MultiCam" );

	QApplication a{ argc, argv };
	a.setQuitOnLastWindowClosed( true );
	a.setWindowIcon( QIcon( ":/AI/H16.png" ) );

	AI w;
	w.move( 600, 400 );
	w.show();
	w.setFixedSize( w.width(), w.height() );
	return a.exec();
}