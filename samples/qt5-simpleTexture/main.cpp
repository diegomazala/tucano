
#include <QApplication>
#include "GLWidget.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
	GLWidget window;
	window.show();
    return app.exec();
}

