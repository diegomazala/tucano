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

#ifndef __QTPLAINWIDGET__
#define __QTPLAINWIDGET__

#include <qglobal.h>
#if QT_VERSION >= 0x050400
#include <QOpenGLWidget>
#else
#include <GL/glew.h>
#include <QGLWidget>
#endif

#include <QMouseEvent>
#include <iostream>

#include <tucano.hpp>


namespace Tucano
{

/**
 * @brief A plain QtWidget.
 *
 * Extends QGLWidget class to include common methods for using ShaderLib
 * This widget is empty, only has initialization methods, such as GLEW.
 */
#if QT_VERSION >= 0x050400
class QtPlainWidget : public QOpenGLWidget, public GLObject
#else
class QtPlainWidget : public QGLWidget, public GLObject
#endif
{
    Q_OBJECT

protected:

public:

#if QT_VERSION >= 0x050400
	explicit QtPlainWidget(QWidget *parent) : QOpenGLWidget(parent) {}
#else
	explicit QtPlainWidget(QWidget *parent) : QGLWidget(parent) {}
#endif

    virtual ~QtPlainWidget()
    {
    }

    /**
     * @brief Initializes openGL and GLEW.
     */
    virtual void initializeGL (void)
    {
        makeCurrent();

		initGL();

#if QT_VERSION < 0x050400
#ifdef TUCANODEBUG
		QGLFormat glCurrentFormat = this->format();
		std::cout << "set version : " << glCurrentFormat.majorVersion() << " , " << glCurrentFormat.minorVersion() << std::endl;
		std::cout << "set profile : " << glCurrentFormat.profile() << std::endl;
#endif
#endif

    }

    /**
     * @brief Resize callback for the widget.
     */
    virtual void resizeGL (void)
    {
#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif
    }


protected:

    /**
     * @brief Callback for key press event.
     * @param event The key event that triggered the callback.
     */
    void keyPressEvent (QKeyEvent * event)
    {
        event->ignore();
#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif
    }




signals:

public slots:

};

}
#endif
