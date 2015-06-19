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
#include "phongshader.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

namespace Shapes
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

	Tucano::Shader camerarep_shader;

public:

	/**
	* @brief Default Constructor
	* @todo load shader from string and remove phong dependency
	*/
	CameraRep()
	{
		initGL();
		resetModelMatrix();
		createGeometry();

        #ifdef TUCANOSHADERDIR
        initialize(stringify(TUCANOSHADERDIR));
        #endif
    }

    /**
     * @brief Initializes shaders
     * @param shader_dir Location of shader files
     */
    void initialize (string shader_dir = "../effects/shaders/")
    {
        camerarep_shader.load("phongshader", shader_dir);
		camerarep_shader.initialize();
    }


    ///Default destructor.
    ~CameraRep() 
	{
	}


	/**
	* @brief Render camera representation
	*/
	void render (const Tucano::Camera& camera, const Tucano::Camera& light)
	{
	    Eigen::Vector4f viewport = camera.getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		camerarep_shader.bind();
        
		camerarep_shader.setUniform("viewMatrix", camera.getViewMatrix());
       	camerarep_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
       	camerarep_shader.setUniform("nearPlane", camera.getNearPlane());
       	camerarep_shader.setUniform("farPlane", camera.getFarPlane());

       	Eigen::Vector4f color (1.0, 1.0, 0.0, 1.0);
       	camerarep_shader.setUniform("modelMatrix", model_matrix);
		camerarep_shader.setUniform("lightViewMatrix", light.getViewMatrix());
       	camerarep_shader.setUniform("has_color", true);
       	camerarep_shader.setUniform("default_color", color);

		setAttributeLocation(camerarep_shader);

		glEnable(GL_DEPTH_TEST);
		bindBuffers();
		renderElements();
		unbindBuffers();
		glDisable(GL_DEPTH_TEST);

       	camerarep_shader.unbind();
		
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
		vector< Eigen::Vector3f > norm;
		vector< Eigen::Vector4f > color;
		vector< GLuint > elementsVertices;
		
		// repeating vertices for each face to make life easier with normals
		// and other attributes

		// right face
		vert.push_back ( Eigen::Vector4f( 0.25, -0.25,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.50, -0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.50,  0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.25,  0.25,  0.5, 1.0) );
		norm.push_back ( Eigen::Vector3f( 1.0, 0.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 1.0, 0.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 1.0, 0.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 1.0, 0.0, 0.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		elementsVertices.push_back(0);
		elementsVertices.push_back(1);
		elementsVertices.push_back(2);
		elementsVertices.push_back(2);
		elementsVertices.push_back(3);
		elementsVertices.push_back(0);

		// left face
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.50, -0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.50,  0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25,  0.25,  0.5, 1.0) );
		norm.push_back ( Eigen::Vector3f( -1.0, 0.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( -1.0, 0.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( -1.0, 0.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( -1.0, 0.0, 0.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		elementsVertices.push_back(4);
		elementsVertices.push_back(7);
		elementsVertices.push_back(6);
		elementsVertices.push_back(6);
		elementsVertices.push_back(5);
		elementsVertices.push_back(4);

		// bottom face
		vert.push_back ( Eigen::Vector4f( 0.25, -0.25,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.50, -0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.50, -0.50, -0.5, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, -1.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, -1.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, -1.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, -1.0, 0.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		elementsVertices.push_back(8);
		elementsVertices.push_back(10);
		elementsVertices.push_back(11);
		elementsVertices.push_back(11);
		elementsVertices.push_back(9);
		elementsVertices.push_back(8);

		// top face
		vert.push_back ( Eigen::Vector4f( 0.25, 0.25,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.50, 0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, 0.25,  0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.50, 0.50, -0.5, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 1.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 1.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 1.0, 0.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 1.0, 0.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		elementsVertices.push_back(12);
		elementsVertices.push_back(13);
		elementsVertices.push_back(15);
		elementsVertices.push_back(15);
		elementsVertices.push_back(14);
		elementsVertices.push_back(12);

		// front face
		vert.push_back ( Eigen::Vector4f( 0.5, -0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.5,  0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.5, -0.50, -0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.5,  0.50, -0.5, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, -1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, -1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, -1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, -1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		elementsVertices.push_back(16);
		elementsVertices.push_back(18);
		elementsVertices.push_back(19);
		elementsVertices.push_back(19);
		elementsVertices.push_back(17);
		elementsVertices.push_back(16);

		// back face
		vert.push_back ( Eigen::Vector4f( 0.25, -0.25, 0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.25,  0.25, 0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25, -0.25, 0.5, 1.0) );
		vert.push_back ( Eigen::Vector4f(-0.25,  0.25, 0.5, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, 1.0) );
		norm.push_back ( Eigen::Vector3f( 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		elementsVertices.push_back(20);
		elementsVertices.push_back(21);
		elementsVertices.push_back(23);
		elementsVertices.push_back(23);
		elementsVertices.push_back(22);
		elementsVertices.push_back(20);


		loadVertices(vert);
		loadNormals(norm);
		loadColors(color);
		loadIndices(elementsVertices);

		setDefaultAttribLocations();
		
	}


};
}
}
#endif
