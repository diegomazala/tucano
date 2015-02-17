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

#ifndef __CAMERAREP__
#define __CAMERAREP__

#include "mesh.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

/// Default fragment shader for rendering camera representation.
const string camerarep_fragment_code = "\n"
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
const string camerarep_vertex_code = "\n"
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
 * @brief Camera visual representation
 *
 * Renders a camera as a oriented frustum and x,y, and z axis
 **/
class CameraRep : public Tucano::Mesh {

private:

	Tucano::Shader *camerarep_shader;

public:

	/**
	* @brief Default Constructor
	*/
	CameraRep()
	{

		resetModelMatrix();
		createGeometry();

		camerarep_shader = new Shader("cameraRepShader");
		camerarep_shader->initializeFromStrings(camerarep_vertex_code, camerarep_fragment_code);
		
	}

    ///Default destructor.
    ~CameraRep() 
	{
		delete camerarep_shader;
	}


	/**
	* @brief Render camera representation
	*/
	void render (Tucano::Camera *camera, Tucano::Camera *light)
	{
		camerarep_shader->bind();
        
		camerarep_shader->setUniform("viewMatrix", camera->getViewMatrix());
       	camerarep_shader->setUniform("projectionMatrix", camera->getProjectionMatrix());
       	camerarep_shader->setUniform("nearPlane", camera->getNearPlane());
       	camerarep_shader->setUniform("farPlane", camera->getFarPlane());

       	Eigen::Vector4f color (1.0, 0.0, 0.0, 1.0);
       	camerarep_shader->setUniform("modelMatrix", modelMatrix);
       	camerarep_shader->setUniform("in_Color", color);

		bindBuffers();
		renderElements();
		unbindBuffers();

       	camerarep_shader->unbind();
		
	}

private:


	/**
	* @brief Define camera geometry
	*
	* Camera is represented as a frustum and three axis
	*/
	void createGeometry (void)
	{
		vector< Eigen::Vector4f > vert;
		vector< GLuint > elementsVertices;
		
		vert.push_back ( Eigen::Vector4f( 0.25, -0.25, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.25, -0.50,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.25,  0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.25,  0.25, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25, -0.5, 1.0) );

		// left face
		elementsVertices.push_back(0);
		elementsVertices.push_back(1);
		elementsVertices.push_back(2);
		elementsVertices.push_back(2);
		elementsVertices.push_back(3);
		elementsVertices.push_back(0);
		// right face
		elementsVertices.push_back(4);
		elementsVertices.push_back(7);
		elementsVertices.push_back(6);
		elementsVertices.push_back(6);
		elementsVertices.push_back(5);
		elementsVertices.push_back(4);
		// top face
		elementsVertices.push_back(3);
		elementsVertices.push_back(2);
		elementsVertices.push_back(6);
		elementsVertices.push_back(6);
		elementsVertices.push_back(7);
		elementsVertices.push_back(3);
		// bottom face
		elementsVertices.push_back(0);
		elementsVertices.push_back(4);
		elementsVertices.push_back(5);
		elementsVertices.push_back(5);
		elementsVertices.push_back(1);
		elementsVertices.push_back(0);
		// front face
		elementsVertices.push_back(1);
		elementsVertices.push_back(2);
		elementsVertices.push_back(6);
		elementsVertices.push_back(6);
		elementsVertices.push_back(5);
		elementsVertices.push_back(1);
		// back face
		elementsVertices.push_back(4);
		elementsVertices.push_back(7);
		elementsVertices.push_back(3);
		elementsVertices.push_back(3);
		elementsVertices.push_back(0);
		elementsVertices.push_back(4);

		loadVertices(vert);
		loadIndices(elementsVertices);

		setDefaultAttribLocations();
		
	}


};

}
#endif
