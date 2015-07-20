#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize( void )
{
    ui->glwidget->initialize();
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    int key = ke->key();
    int modifiers = ke->modifiers();

    if (modifiers == 0 && (key == Qt::Key_Escape || key == Qt::Key_Q))
    {
        close();
    }

	if (key == Qt::Key_W)
	{
		ui->glwidget->setWireframe(!ui->glwidget->wireframe());
	}

    ke->accept();
}
