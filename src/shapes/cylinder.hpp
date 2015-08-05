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

#ifndef __CYLINDER__
#define __CYLINDER__

#include "mesh.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

namespace Shapes
{

/// Default fragment shader for rendering cylinder 
const string cylinder_fragment_code = "\n"
        "#version 430\n"
        "in vec4 color;\n"
		"in vec3 normal;\n"
		"in vec4 vert;\n"
        "out vec4 out_Color;\n"
		"uniform mat4 lightViewMatrix;\n"
        "void main(void)\n"
        "{\n"
		"   vec3 lightDirection = (lightViewMatrix * vec4(0.0, 0.0, 1.0, 0.0)).xyz;\n"
		"   lightDirection = normalize(lightDirection);\n"
		"   vec3 lightReflection = reflect(-lightDirection, normal);\n"
		"   vec3 eyeDirection = -normalize(vert.xyz);\n"
		"   float shininess = 100.0;\n"
		"	vec4 ambientLight = color * 0.4;\n"
		"	vec4 diffuseLight = color * 0.6 * max(dot(lightDirection, normal),0.0);\n"
		"	vec4 specularLight = vec4(1.0) *  max(pow(dot(lightReflection, eyeDirection), shininess),0.0);\n"
		"	out_Color = vec4(ambientLight.xyz + diffuseLight.xyz + specularLight.xyz,1.0);\n"
        "}\n";

/// Default vertex shader for rendering cylinder 
const string cylinder_vertex_code = "\n"
        "#version 430\n"
		"layout(location=0) in vec4 in_Position;\n"
        "out vec4 color;\n"
		"out vec3 normal;\n"
		"out vec4 vert;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform vec4 in_Color;\n"
        "void main(void)\n"
        "{\n"
		"   mat4 modelViewMatrix = viewMatrix * modelMatrix;\n"
		"   mat4 normalMatrix = transpose(inverse(modelViewMatrix));\n"
		"   normal = normalize(vec3(normalMatrix * vec4(in_Position.xyz,0.0)).xyz);\n"
		"   vert = modelViewMatrix * in_Position;\n"
        "   gl_Position = projectionMatrix * modelViewMatrix * in_Position;\n"
        "   color = in_Color;\n"
        "}\n";


/**
 * @brief A simple cylinder shape
 **/
class Cylinder : public Tucano::Mesh {

private:

	/// Shader to render cylinder 
	Tucano::Shader cylinder_shader;

	/// Cylinder color
	Eigen::Vector4f color;

	/// Cylinder cylinder_height
	float cylinder_height;

	/// Cylinder cylinder_radius
	float cylinder_radius;

public:

	/**
	* @brief Default Constructor
	*/
	Cylinder()
	{
		initGL();
		
		resetModelMatrix();
		create(1.0, 2.0);

		color << 0.0, 0.48, 1.0, 1.0;

		cylinder_shader.setShaderName("cylinderShader");
		cylinder_shader.initializeFromStrings(cylinder_vertex_code, cylinder_fragment_code);

	}

    ///Default destructor.
    ~Cylinder() 
	{}


	/**
	* @brief Sets the cylinder color
	* @param c New color
	*/
	void setColor (const Eigen::Vector4f &c)
	{
		color = c;
	}

	/**
	* @brief Render cylinder
	*/
	void render (const Tucano::Camera& camera, const Tucano::Camera& light)
	{
		Eigen::Vector4f viewport = camera.getViewport();
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		cylinder_shader.bind();

       	cylinder_shader.setUniform("modelMatrix", model_matrix);
		cylinder_shader.setUniform("viewMatrix", camera.getViewMatrix());
       	cylinder_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
		cylinder_shader.setUniform("lightViewMatrix", light.getViewMatrix());
       	cylinder_shader.setUniform("in_Color", color);

 		this->setAttributeLocation(&cylinder_shader);

		glEnable(GL_DEPTH_TEST);
		this->bindBuffers();
		this->renderElements();
		this->unbindBuffers();

       	cylinder_shader.unbind();

		#ifdef TUCANODEBUG
		errorCheckFunc(__FILE__, __LINE__);
		#endif
		
	}


	/**
	* @brief Create cylinder with given parameters
	* @param r Radius
	* @param h Height
	* @param s Number of subdivisions
	*/
	void create (float r, float h, int s = 32)
	{
		cylinder_radius = r;
		cylinder_height = h;
		createGeometry(s);
	}

	/**
	* @brief Returns cylinder cylinder_height
	*/
	float getHeight (void)
	{
		return cylinder_height;
	}

	/**
	* @brief Returns cylinder cylinder_radius 
	*/
	float getRadius (void)
	{
		return cylinder_radius;
	}

private:


	/**
	* @brief Define cylinder geometry
	*
	* Cylinder is created by creating two disks (caps) and generating triangles between them
	*
	* @param subdivisions Number of subdivisons for cap and body
	*/
	void createGeometry (int subdivisions)
	{
		reset();

		vector< Eigen::Vector4f > vert;
		vector< Eigen::Vector3f > norm;
		vector< GLuint > faces;

		float x, y, theta;
		// create vertices for top and bottom caps
		for (int i = 0; i < subdivisions; ++i)
		{
			theta = 2.0*M_PI*i/(float)subdivisions;
			x = sin(theta)*cylinder_radius;
			y = cos(theta)*cylinder_radius;
			vert.push_back(Eigen::Vector4f(x, y, cylinder_height, 1.0));
			vert.push_back(Eigen::Vector4f(x, y, 0.0, 1.0));
			norm.push_back(Eigen::Vector3f(x, y, 0.0));
			norm.push_back(Eigen::Vector3f(x, y, 0.0));
		}
		
		// create a face with every three vertices for cylinder body
		for (int i = 0; i < subdivisions*2; ++i)
		{
			faces.push_back(i);
			faces.push_back((i+1)%(subdivisions*2));
			faces.push_back((i+2)%(subdivisions*2));
		}

		// create top cap
		vert.push_back(Eigen::Vector4f(0.0, 0.0, cylinder_height, 1.0));
		norm.push_back(Eigen::Vector3f(0.0, 0.0,  1.0));
		int center_index = vert.size()-1;
		int offset = vert.size();
		for (int i = 0; i < subdivisions; ++i)
		{
			theta = 2.0*M_PI*i/(float)subdivisions;
			x = sin(theta)*cylinder_radius;
			y = cos(theta)*cylinder_radius;
			vert.push_back(Eigen::Vector4f(x, y, cylinder_height, 1.0));
			norm.push_back(Eigen::Vector3f(0.0, 0.0,  1.0));
		}

		for (int i = 0; i < subdivisions; ++i)
		{
			faces.push_back(i+offset);
			faces.push_back((i+1)%(subdivisions) + offset);
			faces.push_back(center_index);
		}
		// create bottom cap
		vert.push_back(Eigen::Vector4f(0.0, 0.0, 0.0, 1.0));
		norm.push_back(Eigen::Vector3f(0.0, 0.0, -1.0));
		center_index = vert.size()-1;
		offset = vert.size();
		for (int i = 0; i < subdivisions; ++i)
		{
			theta = 2.0*M_PI*i/(float)subdivisions;
			x = sin(theta)*cylinder_radius;
			y = cos(theta)*cylinder_radius;
			vert.push_back(Eigen::Vector4f(x, y, 0.0, 1.0));
			norm.push_back(Eigen::Vector3f(0.0, 0.0, -1.0));
		}

		for (int i = 0; i < subdivisions; ++i)
		{
			faces.push_back(i+offset);
			faces.push_back((i+1)%(subdivisions) + offset);
			faces.push_back(center_index);
		}


		loadVertices(vert);
		loadNormals(norm);
		loadIndices(faces);

		setDefaultAttribLocations();
		
	}

};
}
}
#endif
