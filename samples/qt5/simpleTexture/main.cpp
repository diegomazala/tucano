
#include <QApplication>
#include "GLWidget.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
	GLWidget window;
	window.setWindowTitle("Qt5 simpleTexture");
	window.show();
    return app.exec();
}

