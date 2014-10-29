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

#ifndef __QTTRACKBALLWIDGET__
#define __QTTRACKBALLWIDGET__

#include <GL/glew.h>

#include "objimporter.hpp"
//#include "plyimporter.hpp"

#include <tucano.hpp>

#include <QGLWidget>
#include <QMouseEvent>

using namespace std;

namespace Tucano
{

/**
 * @brief A widget with a trackball iteration for a single mesh and a single light source.
 * Extends QGLWidget class to include common methods for using ShaderLib
 * this widget already has a default camera and light trackball and associated mouse methods
 */
class QtTrackballWidget : public QGLWidget
{
    Q_OBJECT

protected:

    /// Triangle mesh.
    Mesh* mesh;

    /// Trackball for manipulating the camera.
    Trackball *camera_trackball;

    /// Trackball for manipulating the light position.
    Trackball *light_trackball;

public:

    explicit QtTrackballWidget(QWidget *parent) : QGLWidget(parent)
    {
        mesh = 0;
        camera_trackball = 0;
        light_trackball = 0;
    }

    ~QtTrackballWidget()
    {

        if (mesh)
            delete mesh;
        if (camera_trackball)
            delete camera_trackball;
        if (light_trackball)
            delete light_trackball;
    }

    /**
     * @brief Initializes openGL and GLEW.
     */
    virtual void initializeGL (void)
    {
        makeCurrent();

        QGLFormat glCurrentFormat = this->format();
        cout << "set version : " << glCurrentFormat.majorVersion() << " , " << glCurrentFormat.minorVersion() << endl;
        cout << "set profile : " << glCurrentFormat.profile() << endl;
        // Glew Initialization:
        cout << "initializing glew ..." << endl;

        glewExperimental = true;
        GLenum glewInitResult = glewInit();

        //errorCheckFunc(__FILE__, __LINE__);
        if (GLEW_OK != glewInitResult) {
            cerr << "Error: " << glewGetErrorString(glewInitResult) << endl;
            exit(EXIT_FAILURE);
        }
        cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl << endl;

    }

    /**
     * @brief Resize callback for the widget.
     */
    virtual void resizeGL (void)
    {
        camera_trackball->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));
        light_trackball->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));
        updateGL();
    }

    /**
     * @brief Initializes the trackball and the mesh with a given filename
     * @todo move modelmatrix to mesh class.
     */
    void initialize (string filename)
    {
        Eigen::Vector2i size;
        size << this->width(), this->height();

        mesh = new Mesh();
        MeshImporter::loadObjFile(mesh, filename);
        //ShaderLib::MeshImporter::loadPlyFile(mesh, filename);
        mesh->normalizeModelMatrix();

        camera_trackball = new Trackball();
        camera_trackball->setPerspectiveMatrix(60.0, this->width()/this->height(), 1.0f, 100.0f);
        camera_trackball->setRenderFlag(true);

        camera_trackball->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));

        light_trackball = new Trackball();
        light_trackball->setRenderFlag(false);
        light_trackball->setViewport(Eigen::Vector2f ((float)this->width(), (float)this->height()));

        updateGL();
        //ShaderLib::errorCheckFunc(__FILE__, __LINE__);
    }

protected:

    /**
     * @brief Callback for mouse double click.
     * @param event The mouse event that triggered the callback.
     */
    void mouseDoubleClickEvent (QMouseEvent * event)
    {
        event->ignore();
        updateGL();
    }

    /**
     * @brief Callback for key release event.
     * @param event The key event that triggered the callback.
     */
    void keyReleaseEvent (QKeyEvent * event)
    {
        event->ignore();
        updateGL ();
    }

    /**
     * @brief Callback for key press event.
     * @param event The key event that triggered the callback.
     */
    void keyPressEvent (QKeyEvent * event)
    {
        event->ignore();
        updateGL();
    }

    /**
     * @brief Callback for mouse press event.
     * @param event The mouse event that triggered the callback.
     */
    void mousePressEvent (QMouseEvent * event)
    {
        setFocus ();
        Eigen::Vector2f screen_pos (event->x(), event->y());
        if (event->modifiers() & Qt::ShiftModifier)
        {
            if (event->button() == Qt::LeftButton)
            {
                camera_trackball->translateCamera(screen_pos);
            }
        }
        else
        {
            if (event->button() == Qt::LeftButton)
            {
                camera_trackball->rotateCamera(screen_pos);
            }
            if (event->button() == Qt::RightButton)
            {
                light_trackball->rotateCamera(screen_pos);
            }
        }
        updateGL ();
    }

    /**
     * @brief Callback for mouse mouse event.
     * @param event The mouse event that triggered the callback.
     */
    void mouseMoveEvent (QMouseEvent * event)
    {
        Eigen::Vector2f screen_pos (event->x(), event->y());
        if (event->modifiers() & Qt::ShiftModifier && event->buttons() & Qt::LeftButton)
        {
            camera_trackball->translateCamera(screen_pos);
        }
        else
        {
            if (event->buttons() & Qt::LeftButton)
            {
                camera_trackball->rotateCamera(screen_pos);
            }
            if (event->buttons() & Qt::RightButton)
            {
                light_trackball->rotateCamera(screen_pos);
            }
        }

        updateGL ();

    }

    void mouseReleaseEvent (QMouseEvent * event)
    {
        if (event->button() == Qt::LeftButton)
        {
            camera_trackball->endTranslation();
            camera_trackball->endRotation();
        }
        if (event->button() == Qt::RightButton)
        {
            light_trackball->endRotation();
        }

        updateGL ();
    }

    void wheelEvent (QWheelEvent * event)
    {
        const int WHEEL_STEP = 120;

        float pos = event->delta () / float (WHEEL_STEP);

        if (event->modifiers() & Qt::ShiftModifier) // change FOV
        {
            camera_trackball->incrementFov(pos);
        }
        else // change ZOOM
        {
            if( (pos > 0) )
            {
                camera_trackball->increaseZoom(1.05);
            }

            else if(pos < 0)
            {
                camera_trackball->increaseZoom(1.0/1.05);
            }
        }
        updateGL ();
    }

signals:

public slots:

};

}
#endif
