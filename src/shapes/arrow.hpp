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

#ifndef __ARROW__
#define __ARROQ__

#include "mesh.hpp"
#include "shapes/cylinder.hpp"
#include "shapes/cone.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

namespace Shapes
{


/**
 * @brief A rounded arrow shape defined by a cylinder and a cone
 *
 **/
class Arrow : public Tucano::Mesh {

private:

	Tucano::Shapes::Cylinder cylinder;
	Tucano::Shapes::Cone cone;
	Eigen::Vector4f color;

public:

	/**
	* @brief Default Constructor
	*/
	Arrow()
	{
		resetModelMatrix();
		create(0.05, 1.0, 0.1, 0.15);
		setColor(Eigen::Vector4f(0.0, 0.7, 0.7, 1.0));
	}

    ///Default destructor.
    ~Arrow() 
	{}

	/**
	* @brief Sets the arrow color
	*/
	void setColor (const Eigen::Vector4f c)
	{
		color = c;
		cylinder.setColor(color);
		cone.setColor(color);
	}

	/**
	* @brief Create arrow with given parameters
	* @param rcyl Cylinder radius
	* @param hcyl Cylinder height
	* @param rcon Cone radius
	* @param hcon Cone height
	* @param s Number of subdivisions
	*/
	void create (float rcyl, float hcyl, float rcon, float hcon, int s = 32)
	{
		cylinder.create(rcyl, hcyl, Eigen::Vector4f(0.0, 0.0, 0.0, 1.0), s);
		cone.create(rcon, hcon, Eigen::Vector4f(0.0, 0.0, hcyl, 1.0), s);
	}


	/**
	* @brief Render arrow
	*/
	void render (const Tucano::Camera &camera, const Tucano::Camera &light)
	{
		cylinder.setModelMatrix(this->model_matrix);
		cone.setModelMatrix(this->model_matrix);

		cylinder.render(camera, light);
		cone.render(camera, light);
	}
	
	/**
	* @brief For rendering without default shaders
	*/
	void render(void)
	{
		cylinder.setModelMatrix(this->model_matrix);
		cone.setModelMatrix(this->model_matrix);

		cylinder.bindBuffers();
		cylinder.renderElements();
		cylinder.unbindBuffers();
		cone.bindBuffers();
		cone.renderElements();
		cone.unbindBuffers();
	}
};
}
}
#endif
