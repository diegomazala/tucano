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

    ui->group_effects->setId(ui->radio_phong, 0);
    ui->group_effects->setId(ui->radio_ssao, 1);
    connect(ui->group_effects, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), ui->glwidget, &GLWidget::toggleEffect);
    connect(ui->button_reload_shaders, &QPushButton::clicked, ui->glwidget, &GLWidget::reloadShaders);
    connect(ui->slider_ssao_intensity, &QSlider::valueChanged, ui->glwidget, &GLWidget::setSSAOIntensity);
    connect(ui->slider_ssao_maxdist, &QSlider::valueChanged, ui->glwidget, &GLWidget::setSSAOMaxDist);
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
