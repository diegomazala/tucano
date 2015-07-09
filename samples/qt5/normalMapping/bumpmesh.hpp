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

#ifndef __BUMP_MESH__
#define __BUMP_MESH__

#include <model.hpp>
#include <shader.hpp>
#include <tbn.h>

using namespace std;

namespace Tucano
{


class BumpMesh : public GLObject
{

public:

    /**
   * @brief Default Constructor.
   */
	BumpMesh(void) : GLObject(),
					vao(0)
    {
       
    }

   
    ///Default Destructor. 
    virtual ~BumpMesh (void)
    {
    }

	void draw()
	{
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
			);
	}


	void bind()
	{
		if (vao == 0)
			glGenVertexArrays(1, &vao);
		glBindVertexArray(vao); // bind Vertex Array Object
	}

	void unbind()
	{
		glBindVertexArray(0); // unbind Vertex Array Object
	}

	void initBuffers()
	{
		if (vao == 0)
			glGenVertexArrays(1, &vao);
		glBindVertexArray(vao); // bind Vertex Array Object

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(Eigen::Vector3f), &indexed_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(Eigen::Vector2f), &indexed_uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(Eigen::Vector3f), &indexed_normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(Eigen::Vector3f), &indexed_tangents[0], GL_STATIC_DRAW);

		glGenBuffers(1, &bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(Eigen::Vector3f), &indexed_bitangents[0], GL_STATIC_DRAW);

		// Generate a buffer for the indices as well

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 4th attribute buffer : tangents
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glVertexAttribPointer(
			3,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 5th attribute buffer : bitangents
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glVertexAttribPointer(
			4,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glBindVertexArray(0); // unbind Vertex Array Object

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
	}

	void cleanupGL()
	{
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &tangentbuffer);
		glDeleteBuffers(1, &bitangentbuffer);
		glDeleteBuffers(1, &elementbuffer);
	}

protected:

	GLuint vao;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint tangentbuffer;
	GLuint bitangentbuffer;
	GLuint elementbuffer;
	
	std::vector<Eigen::Vector3f> vertices;
	std::vector<Eigen::Vector2f> uvs;
	std::vector<Eigen::Vector3f> normals;
	std::vector<Eigen::Vector3f> tangents;
	std::vector<Eigen::Vector3f> bitangents;
	std::vector<unsigned short> indices;
	std::vector<Eigen::Vector3f> indexed_vertices;
	std::vector<Eigen::Vector2f> indexed_uvs;
	std::vector<Eigen::Vector3f> indexed_normals;
	std::vector<Eigen::Vector3f> indexed_tangents;
	std::vector<Eigen::Vector3f> indexed_bitangents;

public:

    
	bool loadOBJ(const char * path)
	{
		bool success = loadOBJ(path, vertices, uvs, normals);

		TBN::computeTangentBasis(
			vertices, uvs, normals, // input
			tangents, bitangents    // output
			);


		TBN::indexVBO(
			vertices, uvs, normals, tangents, bitangents,
			indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
			);

		return success;
	}


	static bool loadOBJ(
		const char * path,
		std::vector<Eigen::Vector3f> & out_vertices,
		std::vector<Eigen::Vector2f> & out_uvs,
		std::vector<Eigen::Vector3f> & out_normals
		)
	{
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<Eigen::Vector3f> temp_vertices;
		std::vector<Eigen::Vector2f> temp_uvs;
		std::vector<Eigen::Vector3f> temp_normals;


		FILE * file = fopen(path, "r");
		if (file == NULL)
			return false;

		while (1)
		{
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0){
				float x, y, z;
				fscanf(file, "%f %f %f\n", &x, &y, &z);
				temp_vertices.push_back(Eigen::Vector3f(x, y, z));
			}
			else if (strcmp(lineHeader, "vt") == 0){
				float x, y;
				fscanf(file, "%f %f\n", &x, &y);
				temp_uvs.push_back(Eigen::Vector2f(x, y));
			}
			else if (strcmp(lineHeader, "vn") == 0){
				float x, y, z;
				fscanf(file, "%f %f %f\n", &x, &y, &z);
				temp_normals.push_back(Eigen::Vector3f(x, y, z));
			}
			else if (strcmp(lineHeader, "f") == 0){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9){
					printf("File can't be read by our simple parser :-( Try exporting with other options\n");
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else{
				// Probably a comment, eat up the rest of the line
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
			}

		}

		// For each vertex of each triangle
		for (unsigned int i = 0; i<vertexIndices.size(); i++){

			// Get the indices of its attributes
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			// Get the attributes thanks to the index
			Eigen::Vector3f vertex = temp_vertices[vertexIndex - 1];
			Eigen::Vector2f uv = temp_uvs[uvIndex - 1];
			Eigen::Vector3f normal = temp_normals[normalIndex - 1];

			// Put the attributes in buffers
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}

		return true;
	}




};

}
#endif	//__BUMP_MESH__
