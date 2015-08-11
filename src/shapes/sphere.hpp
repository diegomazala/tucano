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

#ifndef __SPHERE__
#define __SPHERE__

#include "mesh.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

namespace Shapes
{

/// Default fragment shader for rendering sphere
const string sphere_fragment_code = "\n"
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
const string sphere_vertex_code = "\n"
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
class Sphere : public Tucano::Mesh {

private:

	// Shader to render sphere
	Tucano::Shader sphere_shader;

	// Sphere color
	Eigen::Vector4f color;

public:

	/**
	* @brief Default Constructor
	*/
	Sphere()
	{
		initGL();

		resetModelMatrix();
		createGeometry();

		color << 1.0, 0.48, 0.16, 1.0;

		sphere_shader.setShaderName("sphereShader");
		sphere_shader.initializeFromStrings(sphere_vertex_code, sphere_fragment_code);

	}

    ///Default destructor.
    ~Sphere() 
	{}


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

		sphere_shader.bind();

       	sphere_shader.setUniform("modelMatrix", model_matrix);
		sphere_shader.setUniform("viewMatrix", camera.getViewMatrix());
       	sphere_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
		sphere_shader.setUniform("lightViewMatrix", light.getViewMatrix());
       	sphere_shader.setUniform("in_Color", color);

		vector <string> attribs;
		sphere_shader.getActiveAttributes(attribs);

 		this->setAttributeLocation(&sphere_shader);

		glEnable(GL_DEPTH_TEST);
		this->bindBuffers();
		this->renderElements();
		this->unbindBuffers();
		glDisable(GL_DEPTH_TEST);

       	sphere_shader.unbind();

		#ifdef TUCANODEBUG
		errorCheckFunc(__FILE__, __LINE__);
		#endif
		
	}

private:


	/**
	* @brief Define a unitary sphere geometry
	*
	* Sphere is created by starting with an octahedron and subdividing triangles
	* for a nice reference see: https://sites.google.com/site/dlampetest/python/triangulating-a-sphere-recursively
	* No need to set normal, just use the position in shader since it is the same
	*/
	void createGeometry (int subdivisions = 4)
	{
		vector< Eigen::Vector4f > vert;
		vector< GLuint > faces;
	
		vert.push_back ( Eigen::Vector4f( 1.0, 0.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f(-1.0, 0.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 1.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0,-1.0, 0.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 0.0, 1.0, 1.0) );
		vert.push_back ( Eigen::Vector4f( 0.0, 0.0,-1.0, 1.0) );

		int a[24] = { 0, 4, 2, 2, 4, 1, 1, 4, 3, 3, 4, 0, 0, 2, 5, 2, 1, 5, 1, 3, 5, 3, 0, 5 };
		faces.insert(faces.end(), a, a+24);
	
		// now subdivide, divide each triangle into 4
		for (int s = 0; s < subdivisions; ++s)
		{
			vector< GLuint > sub_faces;
			for (int i = 0; i < (int)faces.size(); i=i+3)
			{
				Eigen::Vector4f p0 = vert[faces[i+0]];
				Eigen::Vector4f p1 = vert[faces[i+1]];
				Eigen::Vector4f p2 = vert[faces[i+2]];

				Eigen::Vector4f p3 = (p0 + p1)*0.5;
				Eigen::Vector4f p4 = (p0 + p2)*0.5;
				Eigen::Vector4f p5 = (p1 + p2)*0.5;
				p3.head(3).normalize();
				p4.head(3).normalize();
				p5.head(3).normalize();
			
				vert.push_back(p3);
				vert.push_back(p4);
				vert.push_back(p5);

				int ind = vert.size()-3;
				// new faces are: (p0, p4, p3), (p4, p1, p5), (p3, p4, p5), (p3, p5, p2)
				int b[12] = {(int)faces[i+0], ind, ind+1, ind+1, ind+2, (int)faces[i+2], ind, ind+2, ind+1, ind, (int)faces[i+1], ind+2};
				sub_faces.insert(sub_faces.end(), b, b+12);  
			}
			faces.clear();
			faces = sub_faces;
		}

		loadVertices(vert);
		loadIndices(faces);

		setDefaultAttribLocations();
		
	}

};
}
}
#endif
