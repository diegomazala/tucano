#include "glwidget.h"
#include <QDebug>
#include "tbn.h"



GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent), wireframeEnabled(false), currentMap(0)
{
}

GLWidget::~GLWidget() 
{
	makeCurrent();

	for (int i = 0; i < 3; ++i)
	{
		diffuse_map[i]->destroy();
		delete diffuse_map[i];
		normal_map[i]->destroy();
		delete normal_map[i];
		specular_map[i]->destroy();
		delete specular_map[i];
		height_map[i]->destroy();
		delete height_map[i];
	}
	bumpMesh.cleanupGL();
	doneCurrent();
}


void GLWidget::loadTextures()
{
	// initialize texture with given image
	QImage image_diffuse[3];
	QImage image_normal[3];
	QImage image_specular[3];
	QImage image_height[3];

	image_diffuse[0].load(":/images/bricks-diffuse-map.jpg");
	image_normal[0].load(":/images/bricks-normal-map.jpg");
	image_specular[0].load(":/images/bricks-specular-map.jpg");
	image_height[0].load(":/images/bricks-height-map.jpg");

	image_diffuse[1].load(":/images/lcg-diffuse-map.png");
	image_normal[1].load(":/images/lcg-normal-map.png");
	image_specular[1].load(":/images/lcg-specular-map.png");
	image_height[1].load(":/images/lcg-height-map.png");

	image_diffuse[2].load(":/images/shapes-diffuse-map.png");
	image_normal[2].load(":/images/shapes-normal-map.png");
	image_specular[2].load(":/images/shapes-specular-map.png");
	image_height[2].load(":/images/shapes-height-map.png");


	TextureManager::Instance();



	for(int i = 0; i < 3; ++i)
	{
		diffuse_map[i] = new Texture();
		diffuse_map[i]->create(GL_TEXTURE_2D, GL_RGBA, image_diffuse[i].width(), image_diffuse[i].height(), GL_BGRA, GL_UNSIGNED_BYTE, image_diffuse[i].mirrored().bits());
		diffuse_map[i]->bind();
		diffuse_map[i]->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		diffuse_map[i]->unbind();

		normal_map[i] = new Texture();
		normal_map[i]->create(GL_TEXTURE_2D, GL_RGBA, image_normal[i].width(), image_normal[i].height(), GL_BGRA, GL_UNSIGNED_BYTE, image_normal[i].mirrored().bits());
		normal_map[i]->bind();
		normal_map[i]->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		normal_map[i]->unbind();

		specular_map[i] = new Texture();
		specular_map[i]->create(GL_TEXTURE_2D, GL_RGBA, image_specular[i].width(), image_specular[i].height(), GL_BGRA, GL_UNSIGNED_BYTE, image_specular[i].mirrored().bits());
		specular_map[i]->bind();
		specular_map[i]->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		specular_map[i]->unbind();

		height_map[i] = new Texture();
		height_map[i]->create(GL_TEXTURE_2D, GL_RGBA, image_height[i].width(), image_height[i].height(), GL_BGRA, GL_UNSIGNED_BYTE, image_height[i].mirrored().bits());
		height_map[i]->bind();
		height_map[i]->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		height_map[i]->unbind();
	}
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
	parallaxMapping.setShadersDir(shader_dir);
	parallaxMapping.initialize();
	parallaxMapping.setTextures(*diffuse_map[currentMap], *normal_map[currentMap], *specular_map[currentMap], *height_map[currentMap]);
    

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
	parallaxMapping.bind(mesh, *camera, *light_trackball);
	glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
	bumpMesh.draw();
	parallaxMapping.unbind();
	bumpMesh.unbind();

	camera->render();

}

void GLWidget::onWireframeToggled(bool toggled)
{
	wireframeEnabled = toggled;
	update();
}

void GLWidget::onImageBricksToggled(bool toggled)
{
	if (toggled)
	{
		currentMap = 0;
		parallaxMapping.setTextures(*diffuse_map[currentMap], *normal_map[currentMap], *specular_map[currentMap], *height_map[currentMap]);
		update();
	}
}


void GLWidget::onImageLCGToggled(bool toggled)
{
	if (toggled)
	{
		currentMap = 1;
		parallaxMapping.setTextures(*diffuse_map[currentMap], *normal_map[currentMap], *specular_map[currentMap], *height_map[currentMap]);
		update();
	}
}


void GLWidget::onImageShapesToggled(bool toggled)
{
	if (toggled)
	{
		currentMap = 2;
		parallaxMapping.setTextures(*diffuse_map[currentMap], *normal_map[currentMap], *specular_map[currentMap], *height_map[currentMap]);
		update();
	}
}


void GLWidget::onDiffuseMapEnabled(bool enabled)
{
	parallaxMapping.enableDiffuseTexture(enabled);
	update();
}


void GLWidget::onNormalMapEnabled(bool enabled)
{
	parallaxMapping.enableNormalTexture(enabled);
	update();
}


void GLWidget::onSpecularMapEnabled(bool enabled)
{
	parallaxMapping.enableSpecularTexture(enabled);
	update();
}


void GLWidget::onParallaxMapEnabled(bool enabled)
{
	parallaxMapping.enableParallax(enabled);
	update();
}


void GLWidget::onParallaxScaleChanged(double value)
{
	parallaxMapping.setScale((float)value);
	update();
}


void GLWidget::onParallaxBiasChanged(double value)
{
	parallaxMapping.setBias((float)value);
	update();
}


void GLWidget::onLightIntensityChanged(int value)
{
	parallaxMapping.setLightIntensity((float)value / 25.0f);
	update();
}