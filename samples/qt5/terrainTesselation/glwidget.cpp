#include "glwidget.h"
#include <QDebug>
#include <texturemanager.hpp>


GLWidget::GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent), 
		gizmoEnabled(false), 
		wireframeEnabled(true), 
		currentMap(0), 
		tessInnerLevel(1.0f), 
		tessOuterLevel(1.0f), 
		depthLevel(0.5f)
{
}

GLWidget::~GLWidget() 
{
	makeCurrent();

	for (int i = 0; i < 3; ++i)
	{
		if (height_map[i] != nullptr)
		{
			height_map[i]->destroy();
			delete height_map[i];
		}
	}
	doneCurrent();
}



void GLWidget::initializeGL (void)
{
	initGL();
	std::cout << infoGL() << std::endl;

	terrainMesh.initGL();

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

   
	shader.load("terrain", shader_dir);
	shader.initialize();

#if 0
	if (!terrainMesh.loadOBJ(mesh_file.c_str()))
		std::cerr << "<Error> Could not load obj file" << std::endl;
	else
		terrainMesh.initBuffers();
#else
	//terrainMesh.create(10, 10);
#endif

	//terrainMesh.createQuad();
	terrainMesh.create(2, 2, true);
	//terrainMesh.normalizeModelMatrix();

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
	
	glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
	

	Eigen::Vector4f viewport = camera->getViewport();
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	shader.bind();

	if (height_map[currentMap] != nullptr)
		shader.setUniform("in_HeightMap", height_map[currentMap]->bind());

	// sets all uniform variables for the phong shader
	Eigen::Matrix4f modelViewMatrix = (camera->getViewMatrix() * terrainMesh.getModelMatrix()).matrix();
	shader.setUniform("modelViewMatrix", modelViewMatrix);
	
	Eigen::Matrix3f normalMatrix = modelViewMatrix.transpose().inverse().matrix().block<3, 3>(0, 0);
	shader.setUniform("normalMatrix", normalMatrix);
	
	shader.setUniform("projectionMatrix", camera->getProjectionMatrix());
	shader.setUniform("lightViewMatrix", light_trackball->getViewMatrix());
	
	shader.setUniform("tessLevelInner", tessInnerLevel);
	shader.setUniform("tessLevelOuter", tessOuterLevel);
	shader.setUniform("depthLevel", depthLevel);

	terrainMesh.setAttributeLocation(shader);

	glEnable(GL_DEPTH_TEST);
	terrainMesh.render();

	shader.unbind();
	
	if (height_map[currentMap] != nullptr)
		height_map[currentMap]->unbind();

	if (gizmoEnabled)
		camera->render();
}

void GLWidget::onGizmoToggled(bool toggled)
{
	gizmoEnabled = toggled;
	update();
}


void GLWidget::onWireframeToggled(bool toggled)
{
	wireframeEnabled = toggled;
	update();
}

void GLWidget::onImageRioDeJaneiroToggled(bool toggled)
{
	if (toggled)
	{
		currentMap = 0;
		update();
	}
}


void GLWidget::onImageLCGToggled(bool toggled)
{
	if (toggled)
	{
		currentMap = 1;
		update();
	}
}


void GLWidget::onImageShapesToggled(bool toggled)
{
	if (toggled)
	{
		currentMap = 2;
		update();
	}
}


void GLWidget::onUpdateMeshButtonPressed()
{
	terrainMesh.deleteBuffers();
	terrainMesh.create(terrainWidth, terrainHeight);
	update();
}



void GLWidget::onWidthMeshChanged(int value)
{
	terrainWidth = value;
	update();
}


void GLWidget::onHeightMeshChanged(int value)
{
	terrainHeight = value;
	update();
}



void GLWidget::onDepthLevelChanged(int value)
{
	// from [1, 100] to [-1.0, 1.0]
	depthLevel = float(value) * 0.01f;
	update();
}

void GLWidget::onTessInnerChanged(int value)
{
	tessInnerLevel = value;
	update();
}

void GLWidget::onTessOuterChanged(int value)
{
	tessOuterLevel = value;
	update();
}



void GLWidget::loadTextures()
{
	// initialize texture with given image
	QImage image_height[3];

	QString texFilename;

	texFilename = ":/images/rio_de_janeiro-height-map.png";
	if (!image_height[0].load(texFilename))
		std::cerr << "[Error]   Could not load texture: " << texFilename.toStdString() << std::endl;
	
	texFilename = ":/images/lcg-height-map.png";
	if (!image_height[1].load(texFilename))
		std::cerr << "[Error]   Could not load texture: " << texFilename.toStdString() << std::endl;
		
	texFilename = ":/images/shapes-height-map.png";
	if (!image_height[2].load(texFilename))
		std::cerr << "[Error]   Could not load texture: " << texFilename.toStdString() << std::endl;

	Tucano::TextureManager::Instance();

	for (int i = 0; i < 3; ++i)
	{
		height_map[i] = new Tucano::Texture();
		if (image_height[i].format() == QImage::Format::Format_Indexed8)
			height_map[i]->create(GL_TEXTURE_2D, GL_LUMINANCE, image_height[i].width(), image_height[i].height(), GL_LUMINANCE, GL_UNSIGNED_BYTE, image_height[i].mirrored().bits());
		else
			height_map[i]->create(GL_TEXTURE_2D, GL_RGBA, image_height[i].width(), image_height[i].height(), GL_BGRA, GL_UNSIGNED_BYTE, image_height[i].mirrored().bits());
		height_map[i]->bind();
		height_map[i]->setTexParametersMipMap(4, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		height_map[i]->unbind();
	}
}
