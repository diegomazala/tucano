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
    ui->glwidget->initialize ();

    connect(ui->checkbox_mean, &QCheckBox::toggled, ui->glwidget, &GLWidget::toggleMean);
    connect(ui->spinbox_kernelsize, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->glwidget, &GLWidget::setMeanKernel);
    connect(ui->checkbox_hgradient, &QCheckBox::toggled, ui->glwidget, &GLWidget::toggleHGradient);
    connect(ui->checkbox_vgradient, &QCheckBox::toggled, ui->glwidget, &GLWidget::toggleVGradient);
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    int key = ke->key();
    int modifiers = ke->modifiers();

    if (modifiers == 0 && key == Qt::Key_Escape)
    {
        close();
    }

    ke->accept();
}
