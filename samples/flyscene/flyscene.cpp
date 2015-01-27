#include "flyscene.hpp"

Flyscene::Flyscene(QWidget *parent) : Tucano::QtTrackballWidget(parent)
{
    phong = 0;
	flycamera = 0;
	light = 0;
}

Flyscene::~Flyscene()
{
    if (phong)
    {
        delete phong;
    }
}

void Flyscene::initialize (void)
{
    // initialize the shader effect
    phong = new Effects::Phong();
    phong->setShadersDir("../effects/shaders/");
    phong->initialize();



    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh("../samples/models/toy.ply");
}

void Flyscene::paintGL (void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (phong && mesh)
    {
        phong->render(mesh, flycamera, light);
    }
}
