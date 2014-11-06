#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent)
{
    ssao = 0;
}

GLWidget::~GLWidget()
{
    if (ssao)
    {
        delete ssao;
    }
}

void GLWidget::initialize (void)
{
    // initialize the shader effect
    ssao = new Effects::SSAO();
    ssao->setShadersDir("../effects/shaders/");
    ssao->initialize();

    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh("../samples/models/toy.obj");
}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (ssao && mesh)
    {
        ssao->render(mesh, camera_trackball, light_trackball);
    }
    camera_trackball->render();
}
