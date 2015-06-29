/**
 * Tucano - A library for rapid prototying with Modern OpenGL and GLSL
 * Copyright (C) 2014
 * LCG - Laboratório de Computação Gráfica (Computer Graphics Lab) - COPPE
 * UFRJ - Federal University of Rio de Janeiro
 *
 * This file is part of Tucano Library.
 *
 * Tucano Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tucano Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tucano Library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __TUCANOBASE__
#define __TUCANOBASE__

#include <iostream>
#include <sstream>
#include <utils/misc.hpp>


#if QT_VERSION >= 0x050400
	#include <QOpenGLFunctions_4_3_Core>
#else
	#include <GL/glew.h>
	#include <GL/glu.h>
#endif

#if QT_VERSION >= 0x050400
class GLObject : protected QOpenGLFunctions_4_3_Core
#else
static bool glewInitialized = false;
class GLObject
#endif
{
public:
	GLObject(){}
	
	void initGL()
	{
#if QT_VERSION >= 0x050400
		initializeOpenGLFunctions();

#else
		if (glewInitialized)
			return;

		GLenum glewInitResult = glewInit();
		if (GLEW_OK != glewInitResult)
		{
			std::cerr << "Error: " << glewGetErrorString(glewInitResult) << std::endl;
			exit(EXIT_FAILURE);
		}
		glewInitialized = true;

#ifdef TUCANODEBUG
		errorCheckFunc(__FILE__, __LINE__);
		std::cout << "GLEW INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl << std::endl;
#endif
#endif
	}


	void errorCheckFunc(std::string file, int line, std::string message = "")
	{
#if QT_VERSION >= 0x050400
#else
		//OpenGL Error Handling Function:
		GLenum ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			std::cerr << "GL error in " << file << "  line " << line << " : " << gluErrorString(ErrorCheckValue) << std::endl;
			std::cerr << message.c_str() << std::endl;
			exit(EXIT_FAILURE);
		}
#endif
	}

	std::string infoGL()
	{
		std::stringstream info;

#if QT_VERSION >= 0x050400
		
		info<< "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR) << std::endl
			<< "                    RENDERER:     " << (const char*)glGetString(GL_RENDERER) << std::endl
			<< "                    VERSION:      " << (const char*)glGetString(GL_VERSION) << std::endl
			<< "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#else
		std::stringstream info;
		info<<"GLEW INFO: OpenGL Version: "
			<< glGetString(GL_VERSION) << std::endl;
#endif
		return info.str();
	}
};


#endif // __TUCANOBASE__