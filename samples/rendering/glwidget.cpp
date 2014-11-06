#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent)
{
    ssao = 0;
    phong = 0;
    active_effect = 0;
}

GLWidget::~GLWidget()
{
    if (ssao)
    {
        delete ssao;
    }
    if (phong)
    {
        delete phong;
    }
}

void GLWidget::initialize (void)
{
    // initialize the effects
    ssao = new Effects::SSAO();
    ssao->setShadersDir("../effects/shaders/");
    ssao->initialize();

    phong = new Effects::Phong();
    phong->setShadersDir("../effects/shaders/");
    phong->initialize();

    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh("../samples/models/toy.obj");
}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (mesh)
    {
        if (active_effect == 0 && phong)
        {
            phong->render(mesh, camera_trackball, light_trackball);
        }
        if (active_effect == 1 && ssao)
        {
            ssao->render(mesh, camera_trackball, light_trackball);
        }
    }

    glEnable(GL_DEPTH_TEST);
    camera_trackball->render();
}
