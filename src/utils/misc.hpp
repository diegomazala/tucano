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

#ifndef __MISC__
#define __MISC__


#include <QtGlobal>
#if QT_VERSION >= 0x050400
	#include <QOpenGLFunctions_4_3_Core>
#else
	#include <GL/glew.h>
	#include <GL/glu.h>
#endif

#include <iostream>
#include <string>
#include <cstdlib>

#define stringify( x ) stringify_literal( x )
#define stringify_literal( x ) # x

namespace Tucano
{

namespace Misc
{


/**
 * @brief GL error check method.
 *
 * This is a method to check for OpenGL erros. Note this will catch the last OpenGL error,
 * independently form where it was thrown. This should be used more for debug purposes.
 * Usually the macros __FILE__ and __LINE__ are passed as parameters, with and optional custom message.
 * @param file File from where method was called.
 * @param line Line in file.
 * @param message A custom message to be exhibited.
 */
inline void errorCheckFunc (std::string file, int line, std::string message = "")
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

/**
 * @brief Initialize Glew
 */
inline void initGlew (void)
{
#if QT_VERSION >= 0x050400
#else
    glewExperimental = true;
    GLenum glewInitResult = glewInit();
    if (GLEW_OK != glewInitResult)
    {
        std::cerr << "Error: " << glewGetErrorString(glewInitResult) << std::endl;
        exit(EXIT_FAILURE);
    }

    #ifdef TUCANODEBUG
    errorCheckFunc(__FILE__, __LINE__);
    std::cout << "GLEW INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl << std::endl;
    #endif
#endif
}

}
}
#endif
