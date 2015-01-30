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

	/// Current mouse position
	Eigen::Vector2f start_mouse_pos;
	
	/// Camera rotation (view direction)
	Eigen::Matrix3f rotationMatrix;

	/// Camera position
	Eigen::Vector3f translationVector;

public:

    /**
     * @brief Resets trackball to initial position and orientation
     */
    void reset (void)
    {
        Camera::reset();
		start_mouse_pos = Eigen::Vector2f::Zero();
		translationVector = Eigen::Vector3f::Zero();
		rotationMatrix = Eigen::Matrix3f::Identity();
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
	 * @brief Compose rotation and translation
	 */
	void updateViewMatrix()
	{
		resetViewMatrix();
		viewMatrix.rotate(rotationMatrix);	
		viewMatrix.translate(translationVector);
	}	

    /**
     * @brief Translates the view matrix to the left.
     */
    void strideLeft ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(speed, 0.0, 0.0);
		translationVector += dir;
    }

    /**
     * @brief Translates the view matrix to the right.
     */
    void strideRight ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(-speed, 0.0, 0.0);
		translationVector += dir;
    }

    /**
     * @brief Translates the view matrix back.
     */
    void moveBack ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, 0.0, -speed);
		translationVector += dir;
    }

    /**
     * @brief Translates the view matrix forward.
     */
    void moveForward ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, 0.0, speed);
		translationVector += dir;
    }

    /**
     * @brief Translates the view matrix down.
     */
    void moveDown ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, speed, 0.0);
		translationVector += dir;

    }

    /**
     * @brief Translates the view matrix up.
     */
    void moveUp ( void )
    {
		Eigen::Vector3f dir = Eigen::Vector3f(0.0, -speed, 0.0);
    	translationVector += dir;
	}

    /**
    * @brief Nomalizes a screen position to range [-1,1].
    * @param pos Screen position
    * @return Returns position in normalized coordinates.
    */
    Eigen::Vector2f normalizePosition (const Eigen::Vector2f& pos)
    {
        return Eigen::Vector2f ((pos[0]/((viewport[2]-viewport[0])/2.0)) - 1.0,
                                1.0 - (pos[1]/((viewport[3] - viewport[1])/2.0)));
    }


	/**
	 * @brief Begin view direction rotation
	 * @param pos Mouse coordinates
	 **/
	void startRotation ( Eigen::Vector2f pos )
	{
//		start_mouse_pos = normalizePosition ( pos );
	}	

	/**
	 * @brief Rotates the camera view direction
	 * @param new_pos New mouse position
	 */
	void rotate ( Eigen::Vector2f new_mouse_pos )
	{
		Eigen::Vector2f new_position = normalizePosition(new_mouse_pos);
		Eigen::Vector2f dir2d = new_position - start_mouse_pos;
		Eigen::Vector3f dir3d (dir2d[0], dir2d[1], sqrt(1.0 - (dir2d[0]*dir2d[0] + dir2d[1]*dir2d[1])));
		float anglex = dir2d[0]*M_PI;
		float angley = dir2d[1]*M_PI;
	
		rotationMatrix = Eigen::AngleAxisf(angley, Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(anglex, Eigen::Vector3f::UnitY());
	}

};

}
#endif
