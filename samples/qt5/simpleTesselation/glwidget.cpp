#include "glwidget.h"
#include <QDebug>



GLWidget::GLWidget(QWidget *parent) : 
	Tucano::QtTrackballWidget(parent), 
	wireframeEnabled(false)
{
}

GLWidget::~GLWidget() 
{
}




void GLWidget::initializeGL(void)
{
	initGL();

	// look for shader dir 
	QDir dir;
	std::string shader_dir("effects/shaders/");
	for (int i = 0; i < 5; ++i)
		if (!dir.exists(shader_dir.c_str()))
			shader_dir.insert(0, "../");


	tess_shader.load("simpleTess", shader_dir);
	tess_shader.initialize();
	tess_shader.bind();

	tessLevelInner = 3;
	tessLevelOuter = 2;
	

	icosahedron = new Tucano::Shapes::Icosahedron();
	icosahedron->initGL();
	icosahedron->bindBuffers();
	
	// Initialize various state:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.7f, 0.6f, 0.5f, 1.0f);

	Tucano::QtTrackballWidget::initialize();
	camera->setPerspectiveMatrix(40.0f, width() / height(), 0.1f, 100.0f);
}



void GLWidget::paintGL(void)
{
	makeCurrent();

	Eigen::Vector3f lightPosition(0.25, 0.25, 1);
	Eigen::Vector3f ambientMaterial(0.04f, 0.04f, 0.04f);
	Eigen::Vector3f diffuseMaterial(0.55, 0.55, 0.15);

	tess_shader.setUniform("TessLevelInner", tessLevelInner);
	tess_shader.setUniform("TessLevelOuter", tessLevelOuter);

	tess_shader.setUniform("LightPosition", lightPosition);
	tess_shader.setUniform("AmbientMaterial", ambientMaterial);
	tess_shader.setUniform("DiffuseMaterial", diffuseMaterial);

	tess_shader.setUniform("projectionMatrix", camera->getProjectionMatrix());
	tess_shader.setUniform("modelViewMatrix", camera->getViewMatrix());
	tess_shader.setUniform("lightViewMatrix", light_trackball->getViewMatrix());

	Eigen::Matrix3f normalMatrix;
	normalMatrix = camera->getViewMatrix().inverse().matrix().block<3, 3>(0, 0);
	tess_shader.setUniform("normalMatrix", normalMatrix);


	// Render the scene:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, icosahedron->getNumberOfElements(), GL_UNSIGNED_INT, 0);
}


void GLWidget::onWireframeToggled(bool toggled)
{
	wireframeEnabled = toggled;
	update();
}


void GLWidget::onTessInnerChanged(int value)
{
	tessLevelInner = value;
	update();
}

void GLWidget::onTessOuterChanged(int value)
{
	tessLevelOuter = value;
	update();
}