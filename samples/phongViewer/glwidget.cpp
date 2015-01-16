#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent)
{
    phong = 0;
}

GLWidget::~GLWidget()
{
    if (phong)
    {
        delete phong;
    }
}

void GLWidget::initialize (void)
{
    // initialize the shader effect
    phong = new Effects::Phong();
    phong->setShadersDir("../effects/shaders/");
    phong->initialize();

    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh("../samples/models/toy.ply");
}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (phong && mesh)
    {
        phong->render(mesh, camera_trackball, light_trackball);
    }
    camera_trackball->render();
}
