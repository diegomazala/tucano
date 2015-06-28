
#include <iostream>
#include <exception>
#include "NormalVectorWindow.h"
#include "rendertexture.hpp"

NormalVectorWindow::NormalVectorWindow(): GlutWindow() 
{
}

NormalVectorWindow::~NormalVectorWindow()
{
}



void NormalVectorWindow::OnInit(void)
{
	GLenum glewOk = glewInit();

	if (glewOk != GLEW_OK)
	{
		throw std::exception((const char*)glewGetErrorString(glewOk));
	}

	std::cerr << "<Info> OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	float width = this->Width();
	float height = this->Height();
	
	string shaders_dir("../../../effects/shaders/");
	// initialize the shader effect
	phong.setShadersDir(shaders_dir);
	phong.initialize();

	normal.setShadersDir(shaders_dir);
	normal.initialize();

	normalVector.setShadersDir(shaders_dir);
	normalVector.initialize();
	

	camera = new Tucano::Flycamera();
	camera->setPerspectiveMatrix(60.0, width / height, 0.1f, 100.0f);
	camera->setViewport(Eigen::Vector2f(width, height));

	light = new Tucano::Camera();
	
	MeshImporter::loadObjFile(&mesh, "../../../samples/models/toy.obj");

	mesh.normalizeModelMatrix();


	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
}

void NormalVectorWindow::OnClose()
{

}


void NormalVectorWindow::OnRender(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	phong.render(mesh, *camera, *light);
	normalVector.render(mesh, *camera);

	SwapBuffers();
}

void NormalVectorWindow::OnResize(int w, int h)
{
	glViewport(0,0,w,h);
}


void NormalVectorWindow::OnKeyDown(int nKey, char cAscii)
{       
	if (cAscii == 27 || cAscii == 'q') // 0x1b = ESC  or  'q'
	{
		this->Close(); // Close Window!
		GlutApp::Exit();
	} 
}




