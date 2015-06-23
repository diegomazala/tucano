/**
 ***********************************************************************
 * \file GlutMain.cpp
 ***********************************************************************
 * 
 *
 ***********************************************************************
 * \author		Diego Mazala - diegomazala@gmail.com
 * \date		January, 2009
 ***********************************************************************/
 
#include <iostream>
#include <exception>
#include <GL/glew.h>
#include "GlutApp.h"
#include "GlutWindow.h"
#include "rendertexture.hpp"
#include "FreeImage.h"

typedef struct
{
	float XYZW[4];
	float UV[4];
} Vertex;

GLuint	VertexShaderId,
FragmentShaderId,
ProgramId,
VaoId,
VboId;


class MyWindow : public GlutWindow
{
public:
	MyWindow() :GlutWindow() {}
	virtual ~MyWindow(){}

	void SetFileImage(const std::string& filename) { fileImage = filename; }

protected:
	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Eigen::Vector2i viewport(this->Width(), this->Height());
		rendertexture.renderTexture(image_texture, viewport);

		SwapBuffers();
	}

	virtual void OnInit(void)
	{
		GLenum glewOk = glewInit();

		if (glewOk != GLEW_OK)
		{
			throw std::exception((const char*)glewGetErrorString(glewOk));
		}

		std::cerr << "<Info> OpenGL version: " << glGetString(GL_VERSION) << std::endl;

		CreateShaders();
		CreateVBO();

		CreateTexture(fileImage.c_str());

		// the default is /shaders from your running dir
		string shaders_dir("../../effects/shaders/");

		rendertexture.setShadersDir(shaders_dir);
		rendertexture.initialize();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}

	void OnClose()
	{
		DestroyShaders();
		DestroyVBO();
	}

	virtual void OnResize(int w, int h)
	{
		glViewport(0,0,w,h);
	}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27 || cAscii == 'q') // 0x1b = ESC  or  'q'
		{
			this->Close(); // Close Window!
			GlutApp::Exit();
		} 
	}


	void CreateTexture(const char* pFileName)
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


	void CreateVBO(void)
	{
		Vertex vertices[] =
		{
			{ { -1.f, -1.f, 0.f, 1.f }, { 0.f, 0.f, 0.f, 1.f } },
			{ { -1.f, 1.f, 0.f, 1.f }, { 0.f, 1.f, 0.f, 1.f } },
			{ { 1.f, 1.f, 0.f, 1.f }, { 1.f, 1.f, 1.f, 1.f } },
			{ { 1.f, -1.f, 0.f, 1.f }, { 1.f, 0.f, 1.f, 1.f } }
		};

		GLenum errorCheckValue = glGetError();
		const size_t bufferSize = sizeof(vertices);
		const size_t vertexSize = sizeof(vertices[0]);
		const size_t rgbOffset = sizeof(vertices[0].XYZW);

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		errorCheckValue = glGetError();
		if (errorCheckValue != GL_NO_ERROR)
		{
			std::string error_msg("<Error> Could not create a VBO: ");
			error_msg.append((const char*)gluErrorString(errorCheckValue));
			throw std::exception((const char*)glewGetErrorString(errorCheckValue));
		}

	}

	void DestroyVBO(void)
	{
		GLenum errorCheckValue = glGetError();

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vboId);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vaoId);

		errorCheckValue = glGetError();
		if (errorCheckValue != GL_NO_ERROR)
		{
			std::string error_msg("<Error> Could not destroy a VBO: ");
			error_msg.append((const char*)gluErrorString(errorCheckValue));
			throw std::exception((const char*)glewGetErrorString(errorCheckValue));
		}

	}

	void CreateShaders(void)
	{

		const GLchar* vertexShader =
		{
			"#version 400\n"\

			"layout(location=0) in vec4 in_Position;\n"\
			"layout(location=1) in vec4 in_TexCoord;\n"\
			"out vec4 ex_TexCoord;\n"\

			"void main(void)\n"\
			"{\n"\
			"  gl_Position = in_Position;\n"\
			"  ex_TexCoord = in_TexCoord;\n"\
			"}\n"
		};


		const GLchar* fragmentShader =
		{
			"#version 400\n"\

			"in vec4 ex_TexCoord;\n"\
			"out vec4 out_Color;\n"\
			"uniform sampler2D in_Texture;\n"\

			"void main(void)\n"\
			"{\n"\
			"  vec3 result = texture(in_Texture, ex_TexCoord.xy).rgb;\n"\
			"  out_Color = vec4(result, 1.0f);\n"\
			"}\n"
		};


		GLenum errorCheckValue = glGetError();

		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
		glCompileShader(vertexShaderId);

		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
		glCompileShader(fragmentShaderId);

		programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);
		glUseProgram(programId);

		errorCheckValue = glGetError();
		if (errorCheckValue != GL_NO_ERROR)
		{
			std::string error_msg("<Error> Could not create the shaders: ");
			error_msg.append((const char*)gluErrorString(errorCheckValue));
			throw std::exception((const char*)glewGetErrorString(errorCheckValue));
		}
	}

	void DestroyShaders(void)
	{
		GLenum errorCheckValue = glGetError();

		glUseProgram(0);

		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);

		glDeleteShader(fragmentShaderId);
		glDeleteShader(vertexShaderId);

		glDeleteProgram(programId);

		errorCheckValue = glGetError();
		if (errorCheckValue != GL_NO_ERROR)
		{
			std::string error_msg("<Error> Could not destroy the shaders: ");
			error_msg.append((const char*)gluErrorString(errorCheckValue));
			throw std::exception((const char*)glewGetErrorString(errorCheckValue));
		}

	}


private:
	std::string fileImage;

	GLuint	vertexShaderId,
			fragmentShaderId,
			programId,
			vaoId,
			vboId;
	
	/// Render image effect (simply renders a texture)
	Effects::RenderTexture rendertexture;

	/// Texture to hold input image
	Texture image_texture;

};





int main(int argc, char** argv)
{
	try
	{
		GlutApp::Init(&argc, argv);

		MyWindow lWin;
		if (argc > 1)
			lWin.SetFileImage(argv[1]);
		else
			lWin.SetFileImage("../../tucano.png");
		lWin.Create("glut-simpleTexture", GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

		GlutApp::Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "<Error> " << e.what() << std::endl;
	}

	return GlutApp::Exit();
}
 
 
 
 
 
 