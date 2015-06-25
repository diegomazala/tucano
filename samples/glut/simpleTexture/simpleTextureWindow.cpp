
#include <iostream>
#include <exception>
#include "simpleTextureWindow.h"
#include "rendertexture.hpp"
#include "FreeImage.h"

SimpleTextureWindow::SimpleTextureWindow(): GlutWindow() 
{
}

SimpleTextureWindow::~SimpleTextureWindow()
{
}

void SimpleTextureWindow::SetFileImage(const std::string& filename) 
{ 
	fileImage = filename; 
}

void SimpleTextureWindow::OnRender(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	Eigen::Vector2i viewport(this->Width(), this->Height());
	rendertexture.renderTexture(image_texture, viewport);

	SwapBuffers();
}

void SimpleTextureWindow::OnInit(void)
{
	GLenum glewOk = glewInit();

	if (glewOk != GLEW_OK)
	{
		throw std::exception((const char*)glewGetErrorString(glewOk));
	}

	std::cerr << "<Info> OpenGL version: " << glGetString(GL_VERSION) << std::endl;


	CreateTexture(fileImage.c_str());

	// the default is /shaders from your running dir
	string shaders_dir("../../effects/shaders/");

	rendertexture.setShadersDir(shaders_dir);
	rendertexture.initialize();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void SimpleTextureWindow::OnClose()
{

}

void SimpleTextureWindow::OnResize(int w, int h)
{
	glViewport(0,0,w,h);
}


void SimpleTextureWindow::OnKeyDown(int nKey, char cAscii)
{       
	if (cAscii == 27 || cAscii == 'q') // 0x1b = ESC  or  'q'
	{
		this->Close(); // Close Window!
		GlutApp::Exit();
	} 
}


void SimpleTextureWindow::CreateTexture(const char* pFileName)
{
	if (!pFileName)
		return;

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(pFileName, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(pFileName);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		throw std::exception("<Image::Load> File type unsuported");
		return;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, pFileName);
	//if the image failed to load, return failure
	if (!dib)
	{
		throw std::exception("<Image::Load> Cannot load image file");
		return;
	}

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
	{
		throw std::exception("<Image::Load> Cannot load image file");
		return;
	}
		
	//image_texture.create(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, bits);
	image_texture.create(GL_TEXTURE_2D, GL_RGBA32F, width, height, GL_RGBA, GL_UNSIGNED_BYTE, bits);

}




