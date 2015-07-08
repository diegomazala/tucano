#include "glwidget.h"
#include <QDebug>


void GLWidget::cleanup()
{
	this->makeCurrent();
	//delete image_texture;
	fbo->destroy();
	delete fbo;
	this->doneCurrent();
}

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

	rendertexture.setShadersDir(shader_dir);
	rendertexture.initialize();

	fbo = new Tucano::Framebuffer();
	fbo->create(width(), height(), 1);
}

void GLWidget::paintGL (void)
{
    makeCurrent();

	fbo->bind();
	{
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		phong.render(mesh, *camera, *light_trackball);

		camera->render();
	}
	fbo->unbind();

	makeCurrent();

	glDisable(GL_DEPTH_TEST);
	Eigen::Vector2i viewport(this->width(), this->height());
	rendertexture.renderTexture(*fbo->getTexture(0), viewport);
	glEnable(GL_DEPTH_TEST);

}
