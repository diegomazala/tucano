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

#ifndef __TOON__
#define __TOON__

#include "effect.hpp"

using namespace std;

/**
 * Toon effect class. It is used to store the quantization level for the effect and to handle the mesh rendering.
**/
class Toon : public Effect {

	public:

	/**
	 * Constructor. Just initializes the quantization level and effect name.
	 * @param name The effect's name.
	**/
	Toon(const char* name): quantizationLevel(8) {
		effectName = name;
	}

	///Initializes the effect, creating and loading the shader.
	virtual void initialize();

	/**
	 * Renders the mesh with the Toon effect.
	 * @param mesh A pointer to the mesh object.
	 * @param cameraTrackball A pointer to the camera trackball object.
	 * @param lightTrackball A pointer to the light trackball object.
	 **/
	virtual void render(Mesh* mesh = NULL, Trackball* cameraTrackball = NULL, Trackball* lightTrackball = NULL);

	///Reloads the shader, reading the files again.
	virtual void reloadShaders();

	/**
	 * Returns a pointer to the quantization level attribute. Used to create a button to adjust this attribute within the GUI.
	 * @return Returns a pointer to the quantization level.
	**/
	float* getQuantizationLevelPointer() {
		return &quantizationLevel;
	}

	Shader* getShader() {
		return shader;
	}

	private:

	///
	Shader* shader;

	///Represents the number of colors that will be used in color quantization to create the toonish effect.
	float quantizationLevel;

};

#endif
