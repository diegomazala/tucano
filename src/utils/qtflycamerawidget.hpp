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

#ifndef __QTFLYCAMERAWIDGET__
#define __QTFLYCAMERAWIDGET__

#include <qglobal.h>
#if QT_VERSION >= 0x050400
	#include <QOpenGLWidget>
#else
	#include <GL/glew.h>
	#include <QGLWidget>
#endif

#include "objimporter.hpp"
#include "plyimporter.hpp"

#include <tucano.hpp>
#include <utils/flycamera.hpp>
#include <utils/trackball.hpp>

#include <QMouseEvent>
#include <QFileDialog>


namespace Tucano
{


/**
 * @brief A widget with a trackball iteration for a single mesh and a single light source.
 * Extends QGLWidget class to include common methods for using ShaderLib
 * this widget already has a default camera and light trackball and associated mouse methods
 */
#if QT_VERSION >= 0x050400
class QtFlycameraWidget : public QOpenGLWidget, public GLObject
#else
class QtFlycameraWidget : public QGLWidget, public GLObject
#endif
{
    Q_OBJECT

protected:

    /// Triangle mesh.
    Mesh mesh;

    /// Flycamera.
    Flycamera *camera;

    /// Trackball for manipulating the light position.
    Trackball *light_trackball;

public:

    /**
     * @brief Default constructor.
     * @param parent Parent widget.
     */
#if QT_VERSION >= 0x050400
	explicit QtFlycameraWidget(QWidget *parent) : QOpenGLWidget(parent) {}
#else
	explicit QtFlycameraWidget(QWidget *parent) : QGLWidget(parent) {}
#endif

    /**
     * @brief Default destructor.
     */
    ~QtFlycameraWidget () 
	{
		delete camera;
		delete light_trackball;
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
		std::cout << "QT GL version : " << glCurrentFormat.majorVersion() << " , " << glCurrentFormat.minorVersion() << std::endl;
#endif
#endif
    }

    /**
     * @brief Resize callback for the widget.
     */
    virtual void resizeGL (void)
    {
        camera->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));
        camera->setPerspectiveMatrix(camera->getFovy(), (float)this->width()/(float)this->height(), 0.1f, 100.0f);
        light_trackball->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));
#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif
    }

    /**
     * @brief Initializes the trackball and the mesh with a given filename
     */
    void initialize (void)
    {
		camera = new Flycamera();
		light_trackball = new Trackball();

        Eigen::Vector2i size;
        size << this->width(), this->height();

        camera->setPerspectiveMatrix(60.0, (float)this->width()/(float)this->height(), 0.1f, 100.0f);
        camera->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));

        light_trackball->setRenderFlag(false);
        light_trackball->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));

#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif
    }

    /**
     * @brief Opens a mesh from file.
     *
     * @param filename Given mesh file.
     */
    virtual void openMesh (string filename)
    {
        QString str (filename.c_str());
        QStringList strlist = str.split(".");
        QString extension = strlist[strlist.size()-1];

        if (extension.compare("ply") != 0 && extension.compare("obj") != 0)
        {
            cerr << "file format [" << extension.toStdString() << "] not supported" << endl;
            return;
        }

		mesh.reset();
		mesh.initGL();

        if (extension.compare("ply") == 0)
        {
            MeshImporter::loadPlyFile(&mesh, filename);
        }
        if (extension.compare("obj") == 0)
        {
            MeshImporter::loadObjFile(&mesh, filename);
        }

        mesh.normalizeModelMatrix();
    }

protected:

    /**
     * @brief Callback for key press event.
     * @param event The key event that triggered the callback.
     */
    virtual void keyPressEvent (QKeyEvent * event)
    {
        if (event->key() == Qt::Key_O)
        {
            QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Mesh Files (*.obj *.ply)"));
            if (!filename.isEmpty())
            {
                openMesh (filename.toStdString());
            }
        }
        if (event->key() == Qt::Key_R)
			camera->reset();
        if (event->key() == Qt::Key_A)
			camera->strideLeft();
        if (event->key() == Qt::Key_D)
			camera->strideRight();
        if (event->key() == Qt::Key_S)
			camera->moveBack();
        if (event->key() == Qt::Key_W)
			camera->moveForward();
        if (event->key() == Qt::Key_C)
			camera->moveDown();
        if (event->key() == Qt::Key_E)
			camera->moveUp();

		camera->updateViewMatrix();
	
        event->ignore();
#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif
    }

    /**
     * @brief Callback for mouse press event.
     *
     * The mouse press starts a rotation or a translation if Shift is pressed.
     * @param event The mouse event that triggered the callback.
     */
    void mousePressEvent (QMouseEvent * event)
    {
        setFocus ();
        Eigen::Vector2f screen_pos (event->x(), event->y());
		if (event->button() == Qt::LeftButton)
		{
			camera->startRotation(screen_pos);
			camera->updateViewMatrix();
		}
		if (event->button() == Qt::RightButton)
		{
			light_trackball->rotateCamera(screen_pos);
		}
#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif
    }

    /**
     * @brief Callback for mouse move event.
     *
     * If rotating or translating, this method updates the trackball position.
     * @param event The mouse event that triggered the callback.
     */
    void mouseMoveEvent (QMouseEvent * event)
    {
        Eigen::Vector2f screen_pos (event->x(), event->y());
		if (event->buttons() & Qt::LeftButton)
		{
			camera->rotate(screen_pos);
			camera->updateViewMatrix();
		}
		if (event->buttons() & Qt::RightButton)
		{
			light_trackball->rotateCamera(screen_pos);
		}

#if QT_VERSION >= 0x050400
		update();
#else
		updateGL();
#endif

    }

    /**
     * @brief Callback for mouse release event.
     *
     * Stops rotation or translation.
     * @param event The mouse event that triggered the callback.
     */
    virtual void mouseReleaseEvent (QMouseEvent * event)
    {
        if (event->button() == Qt::RightButton)
        {
			light_trackball->endRotation();
        }

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
