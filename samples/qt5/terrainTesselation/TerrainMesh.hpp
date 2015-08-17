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

#ifndef __TERRAIN_MESH__
#define __TERRAIN_MESH__

#include <mesh.hpp>
#include <shader.hpp>

using namespace std;

namespace Tucano
{


class TerrainMesh : public Mesh
{

public:

    /**
   * @brief Default Constructor.
   */
	TerrainMesh(void) : Mesh()
    {
       
    }

   
    ///Default Destructor. 
	virtual ~TerrainMesh(void)
    {
    }


	//void render(void)
	//{
	//	bindBuffers();
	//	//if (numberOfElements == 0)
	//	//	renderPoints();
	//	//else
	//	//	renderElements();
	//	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
	//	unbindBuffers();
	//}

	void render(void)
	{
		bindBuffers();
#if 1
		renderPatches(3);
#else
		renderElements();
#endif
		unbindBuffers();
	}


	void create(int width, int depth, bool normalized = true)
	{
		initGL();

		float width_size = width - 1;
		float depth_size = depth - 1;
		float width_half_size = 0.5f;
		float depth_half_size = 0.5f;
		if (!normalized)
		{
			width_size = 0.9;
			depth_size = 0.9;
			width_half_size = float(width) * 0.5f;
			depth_half_size = float(depth) * 0.5f;
		}


		vector<Eigen::Vector4f> vert;
		vector<Eigen::Vector2f> uvs;
		vector<Eigen::Vector3f> normals;
		vector<GLuint> indices;

		//fill terrain vertices
		for (int j = 0; j<depth; j++)
		{
			for (int i = 0; i<width; i++)
			{
				float x = float(i) / width_size;
				float z = float(j) / depth_size;

				vert.push_back(Eigen::Vector4f((x - width_half_size) * 2.f, 0.f, (z - depth_half_size) * 2.f, 1.0f));
				normals.push_back(Eigen::Vector3f(0, 1, 0));
				uvs.push_back(Eigen::Vector2f(x, z));
			}
		}

		for (int j = 0; j < depth - 1; j++)
		{
			for (int i = 0; i < width - 1; i++)
			{
				// face 1
				indices.push_back(j * width + i);
				indices.push_back(j * width + i + width);
				indices.push_back(j * width + i + 1);

				// face 2
				indices.push_back(j * width + i + width);
				indices.push_back(j * width + i + width + 1);
				indices.push_back(j * width + i + 1);
			}
		}

		loadVertices(vert);
		loadTexCoords(uvs);
		loadNormals(normals);
		loadIndices(indices);

		setDefaultAttribLocations();
	}

};

}
#endif	//__TERRAIN_MESH__
