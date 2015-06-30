#include "glwidget.h"
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
}

void GLWidget::cleanup()
{
	makeCurrent();
	delete ssao;
	delete phong;
	delete toon;
	doneCurrent();
}

void GLWidget::initializeGL (void)
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

	initGL();

	// look for shader dir 
	QDir dir;
	std::string shader_dir("effects/shaders/");
	for (int i = 0; i < 5; ++i)
		if (!dir.exists(shader_dir.c_str()))
			shader_dir.insert(0, "../");

	// look for mesh file
	QFile file;
	std::string mesh_file("samples/models/toy.ply");
	for (int i = 0; i < 5; ++i)
		if (!file.exists(mesh_file.c_str()))
			mesh_file.insert(0, "../");

	
	// initialize the widget, camera and light trackball, and opens default mesh
	Tucano::QtTrackballWidget::initialize();
	Tucano::QtTrackballWidget::openMesh(mesh_file);

    // initialize the effects
    ssao = new Effects::SSAO();
    ssao->setShadersDir(shader_dir);
    ssao->initialize();

    phong = new Effects::Phong();
	phong->setShadersDir(shader_dir);
    phong->initialize();

    toon = new Effects::Toon();
	toon->setShadersDir(shader_dir);
    toon->initialize();

}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	if (active_effect == 0 )
	{
		phong->render(mesh, *camera, *light_trackball);
	}
	if (active_effect == 1)
	{
		toon->render(mesh, *camera, *light_trackball);
	}
	if (active_effect == 2)
	{
		ssao->render(mesh, *camera, *light_trackball);
	}

    glEnable(GL_DEPTH_TEST);
    if (draw_trackball)
    {
        camera->render();
    }
}
