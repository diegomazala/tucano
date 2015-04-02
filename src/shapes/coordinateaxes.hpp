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

#ifndef __COORDINATEAXES__
#define __COORDINATEAXES__

#include "mesh.hpp"
#include "shapes/cylinder.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

namespace Shapes
{


/// Default fragment shader for rendering coordinate axes representation.
const string axes_fragment_code = "\n"
        "#version 430\n"
        "in vec4 ex_Color;\n"
        "out vec4 out_Color;\n"
        "in float depth;\n"
        "void main(void)\n"
        "{\n"
        "    out_Color = ex_Color;\n"
        "    gl_FragDepth = depth;\n"
        "}\n";

/// Default vertex shader for rendering coordinate axes representation.
const string axes_vertex_code = "\n"
        "#version 430\n"
        "in vec4 in_Position;\n"
		"in vec4 in_Color;\n"
        "out vec4 ex_Color;\n"
        "out float depth;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
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
 * @brief Visual representation of a 3D coordinate axes 
 *
 **/
class CoordinateAxes : public Tucano::Mesh {

private:

	Tucano::Shader axes_shader;
	Tucano::Shapes::Cylinder cylinder;

public:

	/**
	* @brief Default Constructor
	*/
	CoordinateAxes()
	{
		resetModelMatrix();
		createGeometry();

		axes_shader.setShaderName("coordinateaxes");
		axes_shader.initializeFromStrings(axes_vertex_code, axes_fragment_code);
		
	}

    ///Default destructor.
    ~CoordinateAxes() 
	{}


	/**
	* @brief Render camera representation
    * @todo create 3D representation of axes (tubes) and use light
	*/
	void render (const Tucano::Camera &camera, const Tucano::Camera &light)
	{
	    Eigen::Vector4f viewport = camera.getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		axes_shader.bind();
        
		axes_shader.setUniform("viewMatrix", camera.getViewMatrix());
       	axes_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
       	axes_shader.setUniform("nearPlane", camera.getNearPlane());
       	axes_shader.setUniform("farPlane", camera.getFarPlane());
       	axes_shader.setUniform("modelMatrix", model_matrix);
//		axes_shader.setUniform("lightViewMatriix", light.getViewMatrix());
        (void)light;

		cylinder.setAttributeLocation(&axes_shader);

		glEnable(GL_DEPTH_TEST);
		cylinder.render(camera, light);
		//bindBuffers();
		//glDrawArrays(GL_LINES, 0, 6);
		//unbindBuffers();
		glDisable(GL_DEPTH_TEST);

       	axes_shader.unbind();
		
	}

private:


	/**
	* @brief Define axes geometry
	*
	*/
	void createGeometry (void)
	{
		vector< Eigen::Vector4f > vert;
		vector< Eigen::Vector4f > color;
		
		// coordinate axes (X, Y, Z) 
		vert.push_back ( Eigen::Vector4f( 0.0, 0.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 0.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 0.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		color.push_back( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );

		loadVertices(vert);
		loadColors(color);

		setDefaultAttribLocations();
		
	}


};
}
}
#endif
