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

#ifndef __FLYCAMERA__
#define __FLYCAMERA__

#define PI 3.14159265358979323846

#include "camera.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

/**
 * @brief Flythrough camera class for manipulating a camera.
 *
 * This class simulates a flythrough camera with 6 DOF.
 **/
class Flycamera : public Tucano::Camera {


private:

    /// Global movement speed
    float speed;

public:

    /**
     * @brief Resets trackball to initial position and orientation
     */
    void reset (void)
    {
        Camera::reset();
        updateViewMatrix();
    }

    ///Default destructor.
    ~Flycamera() 
	{}

private:

public:

    /**
     * @brief Initializes the view and projection matrices.
     * They are all initialized as Identity matrices, but view is then translated by the defaultTranslation vector.
     */
    void initOpenGLMatrices (void)
    {
        // reset all matrices
        reset();
    }

    /**
     * @brief Default constructor.
     */
    Flycamera ()
    {

        speed = 1.0;

        initOpenGLMatrices();
    }


    /**
     * @brief Increases the zoom on the scene by appling a scale to the View Matrix.
     * The current scale used in View Matrix is multiplied by the given scale factor.
     * @param scale The given scale to be multiplied by current scale factor.
     */
    void increaseZoom (float scale)
    {
        zoom *= scale;
        updateViewMatrix();
    }

    /**
     * @brief Decreases the zoom on the scene by appling a scale to the View Matrix.
     * The current scale used in View Matrix is divided by the given scale factor.
     * @param scale The given scale factor, to which the current scale factor will be divided by.
     */
    void decreaseZoom (float scale)
    {
        zoom /= scale;
        updateViewMatrix();
    }

    /**
     * @brief Applies a scale factor to the viewMatrix.
     * The current scale used in view matrix will be substituted by the given scale factor.
     * @param scale The new scale factor to be applied to View Matrix.
     */
    void applyScaleToViewMatrix (float scale)
    {
        zoom = scale;
        updateViewMatrix();
    }

    /**
     * @brief Translates the view matrix by a given vector trans.
     * @param translation Given translation vector.
     */
    void translateViewMatrix (const Eigen::Vector3f& translation)
    {
        translate(translation);
    }


    /**
     * @brief Applies all trackball's transformations to the view matrix.
     */
    void updateViewMatrix (void)
    {
        resetViewMatrix();
        translate(defaultTranslation);
        rotate(quaternion);
        translate(translationVector);
        scale(zoom);
    }

    /**
     * @brief Creates the circle representating rotation around Z axis.
     * The other circles will be created by simply rotating this one around the desired axis.
     */
    void createTrackballRepresentation (void)
    {
        int index = 0;
        for (float theta = 0; theta<2*PI; theta += (2*PI)/200.0) {

            vertices[index] = 0.8*cos(theta);
            vertices[index+1] = 0.8*sin(theta);
            vertices[index+2] = 0.0f;
            vertices[index+3] = 1.0f;
            index += 4;
        }
    }

    /**
     * @brief Initializes buffers for trackball's representation.
     * Creates and fills the buffers.
     */
    void initializeBuffers (void)
    {
        createBuffers();
        bufferData();

    }

    /**
     * @brief Creates buffers for trackball's representation.
     */
    void createBuffers (void)
    {
        glGenVertexArrays(1, &bufferIDs[0]);
        glGenBuffers(2, &bufferIDs[1]);
    }

    /**
     * @brief Buffer vertices and indices data for trackball's representation.
     */
    void bufferData (void)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /**
     * @brief Bind buffers for rendering trackball's representation.
     */
    void bindBuffers (void)
    {
        //VAO:
        glBindVertexArray(bufferIDs[0]);

        //VBO:
        glBindBuffer(GL_ARRAY_BUFFER,bufferIDs[1]);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
    }

    /**
     * @brief Unbind buffers for rendering trackball's representation.
     */
    void unbindBuffers (void)
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDisableVertexAttribArray(0);
    }

    /**
     * @brief Renders the trackball representation.
     * @todo setTrackballOrthographicMatrix should be set during viewport resize
     */
    void render (void)
    {
        if(drawTrackball)
        {

            float ratio = (viewport[2] - viewport[0]) / (viewport[3] - viewport[1]);
            setTrackballOrthographicMatrix(-ratio, ratio, -1.0, 1.0, 0.1, 100.0);

            trackballShader->bind();

            //Using unique viewMatrix for the trackball, considering only the rotation to be visualized.
            Eigen::Affine3f trackballViewMatrix = Eigen::Affine3f::Identity();
            trackballViewMatrix.translate(defaultTranslation);
            trackballViewMatrix.rotate(quaternion);

            trackballShader->setUniform("viewMatrix", trackballViewMatrix);
            trackballShader->setUniform("projectionMatrix", trackballProjectionMatrix);
            trackballShader->setUniform("nearPlane", near_plane);
            trackballShader->setUniform("farPlane", far_plane);

            bindBuffers();

            //X:
            Eigen::Vector4f colorVector(1.0, 0.0, 0.0, 1.0);
            trackballShader->setUniform("modelMatrix", Eigen::Affine3f::Identity()*Eigen::AngleAxis<float>(PI/2.0,Eigen::Vector3f(0.0,1.0,0.0)));
            trackballShader->setUniform("in_Color", colorVector);
            glDrawArrays(GL_LINE_LOOP, 0, 200);

            //Y:
            colorVector << 0.0, 1.0, 0.0, 1.0;
            trackballShader->setUniform("modelMatrix", Eigen::Affine3f::Identity()*Eigen::AngleAxis<float>(PI/2.0,Eigen::Vector3f(1.0,0.0,0.0)));
            trackballShader->setUniform("in_Color", colorVector);
            glDrawArrays(GL_LINE_LOOP, 0, 200);

            //Z:
            colorVector << 0.0, 0.0, 1.0, 1.0;            
            trackballShader->setUniform("modelMatrix", Eigen::Affine3f::Identity());
            trackballShader->setUniform("in_Color", colorVector);
            glDrawArrays(GL_LINE_LOOP, 0, 200);

            unbindBuffers();

            trackballShader->unbind();
        }
    }
};

}
#endif
