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

#include "computeShaderTest.hpp"

void ComputeShaderTest::initialize() {

	//First pass shader. Simple Phong shading:
	firstPassShader = new Shader("firstPassShader", "shaders/phongShader.vert", "shaders/phongShader.frag");
	firstPassShader->initialize();
	errorCheckFunc(__FILE__, __LINE__);

	//Compute shader:
	shader = new Shader(shaders_dir,"computeShaderTest");
	shader->initialize();
	errorCheckFunc(__FILE__, __LINE__);

	//Third pass shader:
	thirdPassShader = new Shader("thirdPassShader", "shaders/thirdPassShader.vert", "shaders/thirdPassShader.frag");
	thirdPassShader->initialize();
	errorCheckFunc(__FILE__, __LINE__);

	fbo = new Framebuffer(viewportSize[0], viewportSize[1]);

	createImage();
	//createSSBO();
}

/*I won't be needing this for now.
void ComputeShaderTest::createSSBO() {

	glGenBuffers(1,&ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,NUMBEROFARRAYELEMENTS*sizeof(TYPEOFARRAYELEMENTS),NULL,GL_STATIC_DRAW);

	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

}
*/

void ComputeShaderTest::createImage() {

	//Creating texture:
	if(finalImage) {
		glDeleteTextures(1,&finalImage);
	}
	glGenTextures(1,&finalImage);
	glBindTexture(GL_TEXTURE_2D,finalImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewportSize[0], viewportSize[1], 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D,0);

}

void ComputeShaderTest::setOpenGLMatricesUniforms(Shader* currentShader, Trackball* cameraTrackball, Trackball* lightTrackball) {

	//OpenGL Matrices uniforms:
	currentShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix().data(), 4 ,GL_FALSE, 1);
	currentShader->setUniform("modelMatrix",cameraTrackball->getModelMatrix().data(), 4, GL_FALSE, 1);
	currentShader->setUniform("viewMatrix",cameraTrackball->getViewMatrix().data(), 4, GL_FALSE, 1);
	errorCheckFunc(__FILE__, __LINE__);

	//Light Rotation matrix uniform:
	Eigen::Matrix3f lightViewMatrix = lightTrackball->getViewMatrix().linear();
	currentShader->setUniform("lightViewMatrix",lightViewMatrix.data(),3,GL_FALSE,1);	
	errorCheckFunc(__FILE__, __LINE__);
}

void ComputeShaderTest::render(Mesh* mesh, Trackball* cameraTrackball, Trackball* lightTrackball) {

	//First pass: Render the scene to the FBO:
	firstPassShader->enable();

	//Defining mesh color:
	Eigen::Vector4f colorVector;
	colorVector << 0.686, 0.933, 0.933, 1.0;

	//Setting uniforms:
	setOpenGLMatricesUniforms(firstPassShader, cameraTrackball, lightTrackball);
	firstPassShader->setUniform("in_Color",colorVector.data(),4,1);
	errorCheckFunc(__FILE__, __LINE__);

	//Bind buffer to store depth information in framebuffer:
	fbo->bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	errorCheckFunc(__FILE__, __LINE__);

	//First pass mesh rendering:
	mesh->render();
	errorCheckFunc(__FILE__, __LINE__);

	//Unbind the framebuffer:
	fbo->unbind();
	glDrawBuffer(GL_BACK);
	errorCheckFunc(__FILE__, __LINE__);

	firstPassShader->disable();

errorCheckFunc(__FILE__, __LINE__);

	//Second pass: Use compute shaders to change red and green channels.
	shader->enable();

errorCheckFunc(__FILE__, __LINE__);

	//Binding fbo texture for reading inside compute shader:
	glEnable(GL_TEXTURE_2D);
	fbo->bindAttachment(0,0);
	errorCheckFunc(__FILE__, __LINE__);

	//Binding the final image to write to it inside the shader:
	glBindImageTexture(1, finalImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	errorCheckFunc(__FILE__, __LINE__);

	//Setting uniforms:
	shader->setUniform("sampleTexture",0);
	shader->setUniform("finalImage",1);
	errorCheckFunc(__FILE__, __LINE__, "SetUniform");

	//Run the compute shader:
	glDispatchCompute(viewportSize[0],viewportSize[1],1); //I will have a number of threads equal to the viewportSize[0] x viewportSize[1] because I want to use the program once per fragment.
	errorCheckFunc(__FILE__, __LINE__, "Dispatch Compute");

	//Unbind Framebuffer Texture:
	fbo->unbindAttachments();

	//Unbind Image Texture:
	glBindImageTexture(1,0,0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F);
	errorCheckFunc(__FILE__, __LINE__, "BindImage");

	shader->disable();

	//Third pass:
	thirdPassShader->enable();

	//Setting final image for reading:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalImage);
	errorCheckFunc(__FILE__, __LINE__);

	//Setting Uniforms:
	setOpenGLMatricesUniforms(thirdPassShader, cameraTrackball, lightTrackball);
	thirdPassShader->setUniform("sampleTexture", 0);
	thirdPassShader->setUniform("viewportSize", &viewportSize[0], 2, 1);
	errorCheckFunc(__FILE__, __LINE__);

	//Third pass mesh rendering:
	mesh->render();
	errorCheckFunc(__FILE__, __LINE__);

	//Unbind texture:
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	errorCheckFunc(__FILE__, __LINE__);

	thirdPassShader->disable();

}

void ComputeShaderTest::reloadShaders() {

	firstPassShader->reloadShaders();
	shader->reloadShaders();
	thirdPassShader->reloadShaders();

}
