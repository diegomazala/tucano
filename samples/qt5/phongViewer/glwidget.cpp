#include "glwidget.hpp"
#include <QDebug>

void GLWidget::initializeGL (void)
{
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


    // initialize the shader effect
    phong.setShadersDir(shader_dir);
    phong.initialize();

    // initialize the widget, camera and light trackball, and opens default mesh
    Tucano::QtTrackballWidget::initialize();
    Tucano::QtTrackballWidget::openMesh(mesh_file);
}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    phong.render(mesh, *camera, *light_trackball);

    camera->render();
}
