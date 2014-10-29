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

#include "toon.hpp"

void Toon::initialize() {

	shader = new Shader("toonShader", "shaders/toonShader.vert","shaders/toonShader.frag");
	shader->initialize();
	errorCheckFunc(__FILE__, __LINE__);

}

void Toon::reloadShaders() {

	shader->reloadShaders();

}

void Toon::render(Mesh* mesh, Trackball* cameraTrackball, Trackball* lightTrackball){

	shader->enable();

	shader->setUniform("quantizationLevel",quantizationLevel);

	//Define standard color for mesh rendering. Toon shading works best with saturated colors:
	Eigen::Vector4f colorVector;
	colorVector << 1.0,0.0,0.0,1.0;

	//Setting Uniforms:
	shader->setUniform("in_Color",colorVector.data(),4,1);
	shader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix().data(), 4 ,GL_FALSE, 1);
	shader->setUniform("modelMatrix",cameraTrackball->getModelMatrix().data(), 4, GL_FALSE, 1);
	shader->setUniform("viewMatrix",cameraTrackball->getViewMatrix().data(), 4, GL_FALSE, 1);
	Eigen::Matrix3f lightViewMatrix = lightTrackball->getViewMatrix().linear();
	shader->setUniform("lightViewMatrix",lightViewMatrix.data(),3,GL_FALSE,1);
	errorCheckFunc(__FILE__, __LINE__);

	//Mesh Rendering:
	mesh->render();

	shader->disable();

}
