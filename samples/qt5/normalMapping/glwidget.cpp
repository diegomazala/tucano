#include "glwidget.h"
#include <QDebug>
#include "tbn.h"



GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent), wireframeEnabled(false)
{
}

GLWidget::~GLWidget() 
{
	makeCurrent();
	diffuse_map->destroy();
	delete diffuse_map;
	normal_map->destroy();
	delete normal_map;
	specular_map->destroy();
	delete specular_map;
	bumpMesh.cleanupGL();
	doneCurrent();
}


void GLWidget::loadTextures()
{
	// initialize texture with given image
	QImage image_diffuse(":/images/lcg-diffuse-map.png");
	QImage image_normal(":/images/lcg-normal-map.png");
	QImage image_specular(":/images/lcg-specular-map.png");

	TextureManager::Instance();

	diffuse_map = new Texture();
	diffuse_map->create(GL_TEXTURE_2D, GL_RGBA, image_diffuse.width(), image_diffuse.height(), GL_BGRA, GL_UNSIGNED_BYTE, image_diffuse.mirrored().bits());
	diffuse_map->bind();
	diffuse_map->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	diffuse_map->unbind();

	normal_map = new Texture();
	normal_map->create(GL_TEXTURE_2D, GL_RGBA, image_normal.width(), image_normal.height(), GL_BGRA, GL_UNSIGNED_BYTE, image_normal.mirrored().bits());
	normal_map->bind();
	normal_map->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	normal_map->unbind();

	specular_map = new Texture();
	specular_map->create(GL_TEXTURE_2D, GL_RGBA, image_specular.width(), image_specular.height(), GL_BGRA, GL_UNSIGNED_BYTE, image_specular.mirrored().bits());
	specular_map->bind();
	specular_map->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	specular_map->unbind();
}

void GLWidget::setWireframe(bool enabled)
{
	wireframeEnabled = enabled;
}

void GLWidget::initializeGL (void)
{
	initGL();
	bumpMesh.initGL();

	// look for mesh file
	QFile file;
	std::string mesh_file("samples/models/cube.obj");
	for (int i = 0; i < 5; ++i)
		if (!file.exists(mesh_file.c_str()))
			mesh_file.insert(0, "../");

	// look for shader dir 
	QDir dir;
	std::string shader_dir("effects/shaders/");
	for (int i = 0; i < 5; ++i)
		if (!dir.exists(shader_dir.c_str()))
			shader_dir.insert(0, "../");
	
	loadTextures();

	// initialize the shader effect
	normalMapping.setShadersDir(shader_dir);
	normalMapping.initialize();
	normalMapping.setTextures(*diffuse_map, *normal_map, *specular_map);
    

	if (!bumpMesh.loadOBJ(mesh_file.c_str()))
		std::cerr << "<Error> Could not load obj file" << std::endl;
	else
		bumpMesh.initBuffers();


	Tucano::QtTrackballWidget::initialize();
	camera->setPerspectiveMatrix(60.0, (float)this->width() / (float)this->height(), 0.1f, 100.0f);
	
	glClearColor(0.1f, 0.15f, 0.1f, 0.0f);		// background color
	glEnable(GL_DEPTH_TEST);					// Enable depth test
	glDepthFunc(GL_LESS);						// Accept fragment if it closer to the camera than the former one

}

void GLWidget::paintGL (void)
{
    makeCurrent();

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	bumpMesh.bind();
	normalMapping.bind(mesh, *camera, *light_trackball);
	glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
	bumpMesh.draw();
	normalMapping.unbind();
	bumpMesh.unbind();

	camera->render();

}




