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


/**
 * @brief Visual representation of a 3D coordinate axes 
 *
 * The actual geometry is handled by the arrow meshes	
 *
 **/
class CoordinateAxes : public Tucano::Model {

private:

	Tucano::Shapes::Cylinder cylinder;

public:

	/**
	* @brief Default Constructor
	*/
	CoordinateAxes()
	{
		resetModelMatrix();
		cylinder.create(32, 0.1, 1.0);
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
		cylinder.resetModelMatrix();
		cylinder.modelMatrix()->scale(0.2);
		cylinder.modelMatrix()->rotate(this->modelMatrix()->rotation());

		cylinder.setColor(Eigen::Vector4f(0.0, 0.0, 1.0, 1.0));
		cylinder.render(camera, light);

		cylinder.modelMatrix()->rotate(Eigen::AngleAxisf(-M_PI*0.5, Eigen::Vector3f::UnitX()));
		cylinder.setColor(Eigen::Vector4f(0.0, 1.0, 0.0, 1.0));
		cylinder.render(camera, light);

		cylinder.modelMatrix()->rotate(Eigen::AngleAxisf(M_PI*0.5, Eigen::Vector3f::UnitX()));
		cylinder.modelMatrix()->rotate(Eigen::AngleAxisf(M_PI*0.5, Eigen::Vector3f::UnitY()));
		cylinder.setColor(Eigen::Vector4f(1.0, 0.0, 0.0, 1.0));
		cylinder.render(camera, light);
	
	}


};
}
}
#endif
