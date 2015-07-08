
#include "GLWidget.h"
#include <QKeyEvent>
#include <QDir>



GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent), GLObject()
{
}


GLWidget::~GLWidget()
{
	this->makeCurrent();
	image_texture->destroy();
	delete image_texture;
	delete rendertexture;
	this->doneCurrent();
}


void GLWidget::initializeGL()
{	
	initGL();


	// look for shader dir 
	QDir dir;
	std::string shader_dir("effects/shaders/");
	for (int i = 0; i < 5; ++i)
		if (!dir.exists(shader_dir.c_str()))
			shader_dir.insert(0, "../");

	rendertexture = new Effects::RenderTexture();
	rendertexture->setShadersDir(shader_dir);
	rendertexture->initialize();

	//// initialize texture with given image
	QImage image(":/images/camelo.jpg");

	image_texture = new Texture();
	image_texture->create(GL_TEXTURE_2D, GL_RGBA32F, image.width(), image.height(), GL_BGRA, GL_UNSIGNED_BYTE, image.mirrored().bits());
}





void GLWidget::paintGL()
{
	makeCurrent();
	
	glClearColor(0.0, 0.2, 0.3, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// renders the given image, not that we are setting a fixed viewport that follows the widgets size
	// so it may not be scaled correctly with the image's size (just to keep the example simple)
	Eigen::Vector2i viewport(this->width(), this->height());
	rendertexture->renderTexture(*image_texture, viewport);
}






void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Q:
		close();
		break;


	default:
		QWidget::keyReleaseEvent(event);
	}
}
