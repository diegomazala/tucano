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
    }

    ///Default destructor.
    ~Flycamera() 
	{}

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
        speed = 0.1;
        initOpenGLMatrices();
    }

    /**
     * @brief Translates the view matrix to the left.
     */
    void strideLeft ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(speed, 0.0, 0.0);
        translate(dir);
    }

    /**
     * @brief Translates the view matrix to the right.
     */
    void strideRight ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(-speed, 0.0, 0.0);
        translate(dir);
    }

    /**
     * @brief Translates the view matrix back.
     */
    void moveBack ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, 0.0, -speed);
        translate(dir);
    }

    /**
     * @brief Translates the view matrix forward.
     */
    void moveForward ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, 0.0, speed);
        translate(dir);
    }

    /**
     * @brief Translates the view matrix down.
     */
    void moveDown ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, speed, 0.0);
        translate(dir);
    }

    /**
     * @brief Translates the view matrix up.
     */
    void moveUp ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, -speed, 0.0);
        translate(dir);
    }


};

}
#endif
