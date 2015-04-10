#include "glwidget.hpp"
#include <QDebug>

void GLWidget::initialize (void)
{
    // initialize the shader effect
    phong.setShadersDir("../effects/shaders/");
    phong.initialize();

    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh("../samples/models/toy.ply");
}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    phong.render(mesh, camera, light_trackball);

    camera.render();
}
