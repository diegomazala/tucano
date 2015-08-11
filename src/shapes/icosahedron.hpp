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

#ifndef __ICOSAHEDRON__
#define __ICOSAHEDRON__

#include "mesh.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

namespace Shapes
{

/// Default fragment shader for rendering sphere
const string icosahedron_fragment_code = "\n"
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

/// Default vertex shader for rendering sphere
const string icosahedron_vertex_code = "\n"
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
 * @brief A simple sphere shape
 **/
class Icosahedron : public Tucano::Mesh {

private:

	// Shader to render sphere
	Tucano::Shader shader;

	// Icosahedron color
	Eigen::Vector4f color;

public:

	/**
	* @brief Default Constructor
	*/
	Icosahedron()
	{
	}

    ///Default destructor.
    ~Icosahedron() 
	{}

	void initGL()
	{
		Mesh::initGL();

		resetModelMatrix();
		createGeometry();

		color << 1.0, 0.48, 0.16, 1.0;

		//		shader.setShaderName("sphereShader");
		//		shader.initializeFromStrings(icosahedron_vertex_code, icosahedron_fragment_code);

	}


	/**
	* @brief Sets the sphere color
	* @param c New color
	*/
	void setColor (const Eigen::Vector4f &c)
	{
		color = c;
	}

	/**
	* @brief Render sphere
	*/
	void render (const Tucano::Camera& camera, const Tucano::Camera& light)
	{
		Eigen::Vector4f viewport = camera.getViewport();
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		shader.bind();

       	shader.setUniform("modelMatrix", model_matrix);
		shader.setUniform("viewMatrix", camera.getViewMatrix());
       	shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
		shader.setUniform("lightViewMatrix", light.getViewMatrix());
       	shader.setUniform("in_Color", color);

		vector <string> attribs;
		shader.getActiveAttributes(attribs);

 		this->setAttributeLocation(&shader);

		glEnable(GL_DEPTH_TEST);
		this->bindBuffers();
		this->renderElements();
		this->unbindBuffers();
		glDisable(GL_DEPTH_TEST);

       	shader.unbind();

		#ifdef TUCANODEBUG
		errorCheckFunc(__FILE__, __LINE__);
		#endif
		
	}

private:


	void createGeometry()
	{
		vector< Eigen::Vector4f > vert;
		vert.push_back(Eigen::Vector4f(0.000f, 0.000f, 1.000f, 1.0f));
		vert.push_back(Eigen::Vector4f(0.894f, 0.000f, 0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(0.276f, 0.851f, 0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(-0.724f, 0.526f, 0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(-0.724f, -0.526f, 0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(0.276f, -0.851f, 0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(0.724f, 0.526f, -0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(-0.276f, 0.851f, -0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(-0.894f, 0.000f, -0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(-0.276f, -0.851f, -0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(0.724f, -0.526f, -0.447f, 1.0f));
		vert.push_back(Eigen::Vector4f(0.000f, 0.000f, -1.000f, 1.0f));


		vector< GLuint > faces = {
			2, 1, 0,
			3, 2, 0,
			4, 3, 0,
			5, 4, 0,
			1, 5, 0,

			11, 6, 7,
			11, 7, 8,
			11, 8, 9,
			11, 9, 10,
			11, 10, 6,

			1, 2, 6,
			2, 3, 7,
			3, 4, 8,
			4, 5, 9,
			5, 1, 10,

			2, 7, 6,
			3, 8, 7,
			4, 9, 8,
			5, 10, 9,
			1, 6, 10 };

		loadVertices(vert);
		loadIndices(faces);

		setDefaultAttribLocations();
	}

};
}
}
#endif
