#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent)
{
    ssao = 0;
    phong = 0;
    toon = 0;
    active_effect = 0;
    draw_trackball = true;
}

GLWidget::~GLWidget()
{
    delete ssao;
    delete phong;
    delete toon;
}

void GLWidget::initialize (void)
{
    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh("../samples/models/toy.obj");

    // initialize the effects
    ssao = new Effects::SSAO();
    ssao->setShadersDir("../effects/shaders/");
    ssao->initialize();

    phong = new Effects::Phong();
    phong->setShadersDir("../effects/shaders/");
    phong->initialize();

    toon = new Effects::Toon();
    toon->setShadersDir("../effects/shaders/");
    toon->initialize();

}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if (active_effect == 0 )
	{
		phong->render(mesh, camera, light_trackball);
	}
	if (active_effect == 1)
	{
		toon->render(mesh, camera, light_trackball);
	}
	if (active_effect == 2)
	{
		ssao->render(mesh, camera, light_trackball);
	}

    glEnable(GL_DEPTH_TEST);
    if (draw_trackball)
    {
        camera.render();
    }
}
