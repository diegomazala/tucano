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

#ifndef __CAMERAPATH__
#define __CAMERAPATH__

#include "camera.hpp"
#include "mesh.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{


/// Default fragment shader for rendering trackball representation.
const string camerapath_fragment_code = "\n"
        "#version 430\n"
        "in vec4 ex_Color;\n"
        "out vec4 out_Color;\n"
        "in float depth;\n"
        "void main(void)\n"
        "{\n"
        "    out_Color = ex_Color;\n"
        "    gl_FragDepth = depth;\n"
        "}\n";

/// Default vertex shader for rendering trackball representation.
const string camerapath_vertex_code = "\n"
        "#version 430\n"
        "in vec4 in_Position;\n"
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
 * @brief Camera path class, defines control points and a cubic Bezier approximation
 * for defining a smooth camera path from key frames.
 * Reference: https://www.particleincell.com/2012/bezier-splines/
 **/
class CameraPath: public Tucano::Camera {


private:

    /// Movement speed
    float speed;

	/// Camera position at each Key frames
	vector< Eigen::Vector4f > key_position;

	/// View matrix at each key frame
	vector< Eigen::Affine3f > key_matrix;

	/// Mesh with key positions and computed control points for drawing
	/// smooth curve between key positions
	Mesh curve;
    
    /// Path shader, used for rendering the curve
    Shader* camerapath_shader;

public:

    /**
     * @brief Resets the path 
     */
    void reset (void)
    {
		key_position.clear();
		key_matrix.clear();
    }

    ///Default destructor.
    ~CameraPath() 
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
    CameraPath ()
    {
        speed = 0.05;

        initOpenGLMatrices();

        camerapath_shader = new Shader("camerapathShader");
        camerapath_shader->initializeFromStrings(camerapath_vertex_code, camerapath_fragment_code);
    }

	/**
	* @brief Fill the vertices array
	*/
	void fillVertexData (void)
	{

		curve.reset();
		curve.loadVertices(key_position);
		curve.setDefaultAttribLocations();
		
        Misc::errorCheckFunc(__FILE__, __LINE__);
//		curve.createAttribute("p0", key_positions);

	}

	/**
	* @brief Add key position
	* @param pt New key point
	*/
	void addKeyPosition (Eigen::Vector3f & pt)
	{
		key_position.push_back ( Eigen::Vector4f(pt[0], pt[1], pt[2], 1.0) );
		fillVertexData();
		cout << "included key point : " << pt.transpose() << endl;
	}

	/**
	* @brief Renders smooth path
	* End points for each Beziér is passed as line_strip
	* and control points as vertex attributes
	* @param camera Current camera for viewing scene
	*/
    void render (Tucano::Camera *camera)
    {
		if (key_position.size() < 2)
			return;

        camerapath_shader->bind();
        Misc::errorCheckFunc(__FILE__, __LINE__);
        
        camerapath_shader->setUniform("viewMatrix", camera->getViewMatrix());
        camerapath_shader->setUniform("projectionMatrix", camera->getProjectionMatrix());
        camerapath_shader->setUniform("nearPlane", camera->getNearPlane());
        camerapath_shader->setUniform("farPlane", camera->getFarPlane());

        Eigen::Vector4f color (1.0, 0.0, 0.0, 1.0);
        camerapath_shader->setUniform("modelMatrix", Eigen::Affine3f::Identity());
        camerapath_shader->setUniform("in_Color", color);

		curve.setAttributeLocation(camerapath_shader);
		curve.bindBuffers();
        glDrawArrays(GL_LINE_STRIP, 0, key_position.size());
		curve.unbindBuffers();

        camerapath_shader->unbind();
        Misc::errorCheckFunc(__FILE__, __LINE__);
    }


	/**
	* @brief Compute inner control points from key positions
	* For each pair of subsequent key positions, compute two control points
	* to define a Beziér Spline.
	* Since we are restricting the splines to join with same position, and first
	* two derivatives, the result is a smooth curve passing through all control points
	* See reference at class info for more details.
	*/
	void computeInnerControlPoints (void)
	{
		const int n = key_position.size()-1;
		Eigen::MatrixXf A = Eigen::MatrixXf::Zero(n,n);
		Eigen::MatrixXf x = Eigen::MatrixXf::Zero(n,3);
		Eigen::MatrixXf b = Eigen::MatrixXf::Zero(n,3);

		// build the weight matrix, it is the same for all coordinates
		A(0, 0) = -2.0;
		A(0, 1) = 1.0;

		for (int i = 1; i < n-2; i++)
		{
			A(i, i-1) = 1.0;
			A(i, i) = 4.0;
			A(i, i+1) = 1.0;
		}

		A(n-1, n-2) = 2.0;
		A(n-1, n-1) = 7.0;

		// solve the system three times, one for each coordinate (x, y, and z)
		b(0, 0) = key_position[0][0] + 2.0 * key_position[1][0];	
		b(0, 1) = key_position[0][1] + 2.0 * key_position[1][1];
		b(0, 2) = key_position[0][2] + 2.0 * key_position[1][2];

		for (int i = 1; i < n-2; i++)
		{
			b(i, 0) = 4.0*key_position[i][0] + 2.0*key_position[i+1][0];
			b(i, 1) = 4.0*key_position[i][1] + 2.0*key_position[i+1][1];
			b(i, 2) = 4.0*key_position[i][2] + 2.0*key_position[i+1][2];

		}
		b(n-1, 0) = 8.0*key_position[n-1][0] + key_position[n][0];
		b(n-1, 1) = 8.0*key_position[n-1][1] + key_position[n][1];
		b(n-1, 2) = 8.0*key_position[n-1][2] + key_position[n][2];
	
		x.col(0) = A.colPivHouseholderQr().solve(b.col(0));
		x.col(1) = A.colPivHouseholderQr().solve(b.col(1));
		x.col(2) = A.colPivHouseholderQr().solve(b.col(2));
	}


};

}
#endif
