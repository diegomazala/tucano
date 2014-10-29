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

#ifndef __COMPUTE__
#define __COMPUTE__

#include "effect.hpp"
#include "framebuffer.hpp"

using namespace std;

/**
 * Just beginning to play with Compute Shaders. 
**/
class ComputeShaderTest : public Effect {

	public:

	///Default Constructor:
	ComputeShaderTest(const char* name, int currentWidth, int currentHeight): ssbo(0){

		effectName = name;
		viewportSize = Eigen::Vector2i(currentWidth, currentHeight);	

	}

	///Initializes the shaders and creates the FBO.
	virtual void initialize();

	///Renders the scene.
	virtual void render(Mesh* mesh = NULL, Trackball* cameraTrackball = NULL, Trackball* lightTrackball = NULL);

	///Reloads all shaders, reading the files again.
	virtual void reloadShaders();

	Shader* getFirstPassShader() {
		return firstPassShader;
	}

	///Default destructor. Just deletes the shaders and destroy the buffers.
	~ComputeShaderTest() {

		if(firstPassShader) {
			delete firstPassShader;
		}

		if(shader) {
			delete shader;
		}

		if(thirdPassShader) {
			delete thirdPassShader;
		}

	}

	Shader* getShader() {
		return shader;
	}

	protected:

	///Set opengl matrices.
	void setOpenGLMatricesUniforms(Shader* currentShader, Trackball* cameraTrackball, Trackball* lightTrackball);

	///Creates the Shader Storage Buffer Object.
	void createSSBO();

	///Creates the final image to store compute shader manipulations.
	void createImage();

	///Shader used to offscreen rendering.
	Shader* firstPassShader;

	///Compute shader
	Shader* shader;

	///Shader used in back-buffer rendering.
	Shader* thirdPassShader;

	///Framebuffer
	Framebuffer* fbo;

	///Shader Storage Buffer Object.
	GLuint ssbo;

	///Image to store the compute shader manipulations.
	GLuint finalImage;

};

#endif
