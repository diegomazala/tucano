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


#include "camera.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{


/// Default fragment shader for rendering camera coordinate system representation.
const string flycamera_fragment_code = "\n"
        "#version 430\n"
        "in vec4 ex_Color;\n"
        "out vec4 out_Color;\n"
        "in float depth;\n"
        "void main(void)\n"
        "{\n"
        "    out_Color = ex_Color;\n"
        "    gl_FragDepth = depth;\n"
        "}\n";

/// Default vertex shader for rendering camera coordinate system representation.
const string flycamera_vertex_code = "\n"
        "#version 430\n"
        "layout(location=0) in vec4 in_Position;\n"
        "out vec4 ex_Color;\n"
        "out float depth;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform vec4 in_Color;\n"
        "uniform float nearPlane;\n"
        "uniform float farPlane;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = (viewMatrix * modelMatrix) * in_Position;\n"
        "   depth = (farPlane+nearPlane)/(farPlane-nearPlane) + ( (2*nearPlane*farPlane)/(farPlane-nearPlane) ) * (1/gl_Position[2]);\n"
        "   depth = (depth+1.0)/2.0;\n"
        "   gl_Position = projectionMatrix * gl_Position;\n"
        "   ex_Color = in_Color;\n"
        "}\n";


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
	Eigen::Matrix3f rotation_matrix;

	/// Camera position
	Eigen::Vector3f translation_vector;

	// Default start translation vector
	Eigen::Vector3f default_translation;

	/// Rotation angles
	float rotation_Y_axis;
	float rotation_X_axis;

    /// The vertices for drawing the axis on screen
    float vertices[8];
    
    /// Flycamera shader, used for render the axis
    Shader* flycamera_shader;

    /// Buffer Objects for drawing axis on screen
    GLuint * bufferIDs;

public:

    /**
     * @brief Resets camera to initial position and orientation
     */
    void reset (void)
    {
		start_mouse_pos = Eigen::Vector2f::Zero();
		translation_vector = Eigen::Vector3f::Zero();
		rotation_matrix = Eigen::Matrix3f::Identity();
		default_translation = Eigen::Vector3f (0.0, 0.0, -5.0);
        rotation_X_axis = 0.0;
        rotation_Y_axis = 0.0;
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
        speed = 0.05;
        initOpenGLMatrices();

		// vertices for on screen representation of camera coordinate system
        vertices[0] = 0.0;
        vertices[1] = 0.0;
        vertices[2] = 0.0;
        vertices[3] = 1.0;
        vertices[4] = 1.0;
        vertices[5] = 0.0;
        vertices[6] = 0.0;
        vertices[7] = 1.0;

        flycamera_shader = new Shader("flycameraShader");

        bufferIDs = new GLuint[3];
        glGenVertexArrays(1, &bufferIDs[0]);
        glGenBuffers(2, &bufferIDs[1]);

        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        flycamera_shader->initializeFromStrings(flycamera_vertex_code, flycamera_fragment_code);
    }

    void bindBuffers (void)
    {
        // VAO
        glBindVertexArray(bufferIDs[0]);

        // VBO
        glBindBuffer (GL_ARRAY_BUFFER, bufferIDs[1]);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
    }

    void unbindBuffers (void)
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDisableVertexAttribArray(0);
    }

	/**
	* @brief Renders the camera's coordinate axis at the lower right corner of the screen.
	*/
    void render (void)
    {
        float ratio = (viewport[2] - viewport[0]) / (viewport[3] - viewport[1]);
        Eigen::Matrix4f repProjectionMatrix = createOrthographicMatrix(-ratio, ratio, -1.0, 1.0, 0.1, 100.0);

        flycamera_shader->bind();
        Misc::errorCheckFunc(__FILE__, __LINE__);
    
        Eigen::Affine3f repViewMatrix = Eigen::Affine3f::Identity();

        repViewMatrix.translate( Eigen::Vector3f(1.0, -0.75, -2.0));
        repViewMatrix.rotate(rotation_matrix.inverse());
        repViewMatrix.scale(0.2);
        
        flycamera_shader->setUniform("viewMatrix", repViewMatrix);
        flycamera_shader->setUniform("projectionMatrix", repProjectionMatrix);
        flycamera_shader->setUniform("nearPlane", near_plane);
        flycamera_shader->setUniform("farPlane", far_plane);

        bindBuffers();
        Eigen::Vector4f color (1.0, 0.0, 0.0, 1.0);
        flycamera_shader->setUniform("modelMatrix", Eigen::Affine3f::Identity());
        flycamera_shader->setUniform("in_Color", color);
        glDrawArrays(GL_LINES, 0, 2);

        color << 0.0, 1.0, 0.0, 1.0;
        flycamera_shader->setUniform("modelMatrix", Eigen::Affine3f::Identity() * Eigen::AngleAxisf(M_PI*0.5, Eigen::Vector3f::UnitZ()));
        flycamera_shader->setUniform("in_Color", color);
        glDrawArrays(GL_LINES, 0, 2);

        color << 0.0, 0.0, 1.0, 1.0;
        flycamera_shader->setUniform("modelMatrix", Eigen::Affine3f::Identity() * Eigen::AngleAxisf(-M_PI*0.5, Eigen::Vector3f::UnitY()));
        flycamera_shader->setUniform("in_Color", color);
        glDrawArrays(GL_LINES, 0, 2);

        unbindBuffers();
        
        flycamera_shader->unbind();
        Misc::errorCheckFunc(__FILE__, __LINE__);
    }


	/**
	 * @brief Compose rotation and translation
	 */
	void updateViewMatrix()
	{
		resetViewMatrix();

		// compute X axis restricted to a rotation around Y axis
        Eigen::Vector3f rotX = Eigen::AngleAxisf(rotation_Y_axis, Eigen::Vector3f::UnitY()) * Eigen::Vector3f::UnitX();
        rotX.normalize();

		// Z axis follows X axis rotation plus a rotation around new X axis
        Eigen::Vector3f rotZ = Eigen::AngleAxisf(rotation_Y_axis, Eigen::Vector3f::UnitY()) * Eigen::Vector3f::UnitZ();
        rotZ = Eigen::AngleAxisf(rotation_X_axis, rotX) * rotZ;
        rotZ.normalize();

		// Z axis is restricted to X and Y to form an orthonormal system
		Eigen::Vector3f rotY = Eigen::AngleAxisf(rotation_X_axis, rotX) * Eigen::Vector3f::UnitY();
		rotY.normalize();

        rotation_matrix.row(2) = rotZ;
        rotation_matrix.row(1) = rotY;
        rotation_matrix.row(0) = rotX;

		viewMatrix.rotate (rotation_matrix);
        viewMatrix.translate (Eigen::Vector3f(0.0, 0.0, -5.0));
		viewMatrix.translate (translation_vector);
	}	

    /**
     * @brief Translates the view matrix to the left.
     */
    void strideLeft ( void )
    {
		Eigen::Vector3f dir = (Eigen::AngleAxisf(rotation_Y_axis, Eigen::Vector3f::UnitY())) * Eigen::Vector3f(1.0, 0.0, 0.0);
		translation_vector += dir * speed;
    }

    /**
     * @brief Translates the view matrix to the right.
     */
    void strideRight ( void )
    {
		Eigen::Vector3f dir = (Eigen::AngleAxisf(rotation_Y_axis, Eigen::Vector3f::UnitY())) * Eigen::Vector3f(-1.0, 0.0, 0.0);
		translation_vector += dir * speed;
    }

    /**
     * @brief Translates the view matrix back.
     */
    void moveBack ( void )
    {
		Eigen::Vector3f dir = (Eigen::AngleAxisf(rotation_Y_axis, Eigen::Vector3f::UnitY())) * Eigen::Vector3f(0.0, 0.0, -1.0);
		translation_vector += dir * speed;
    }

    /**
     * @brief Translates the view matrix forward.
     */
    void moveForward ( void )
    {
		Eigen::Vector3f dir = (Eigen::AngleAxisf(rotation_Y_axis, Eigen::Vector3f::UnitY())) * Eigen::Vector3f(0.0, 0.0, 1.0);
		translation_vector += dir * speed;
    }

    /**
     * @brief Translates the view matrix down.
     */
    void moveDown ( void )
    {
		translation_vector += Eigen::Vector3f::UnitY() * speed;
    }

    /**
     * @brief Translates the view matrix up.
     */
    void moveUp ( void )
    {
    	translation_vector -= Eigen::Vector3f::UnitY() * speed;
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
		start_mouse_pos = normalizePosition ( pos );
	}	

	/**
	 * @brief Rotates the camera view direction
	 * @param new_pos New mouse position
	 */
	void rotate ( Eigen::Vector2f new_mouse_pos )
	{
		Eigen::Vector2f new_position = normalizePosition(new_mouse_pos);
		Eigen::Vector2f dir2d = new_position - start_mouse_pos;
		
		start_mouse_pos = new_position;

		float anglex = -dir2d[1]*M_PI;
		float angley = -dir2d[0]*M_PI;
	
		rotation_X_axis += anglex;
		rotation_Y_axis += angley;
	}

};

}
#endif
