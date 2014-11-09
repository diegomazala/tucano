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

#ifndef __SSAO__
#define __SSAO__

#include "tucano.hpp"
#include <math.h>

using namespace std;

using namespace Tucano;

namespace Effects
{

/**
 * Screen Space Ambient Occlusion effect class. Handles the pre-computations needed in order to use this effect, as well as handles the rendering with this effect. 
 * This class handles sampling kernel generation used in occlusion computation, noise texture generation used to remove banding caused by the low number of sample points.
 * Three rendering passes are needed in order to use this effect: in the first pass, the eye-space depth information of the mesh is stored in one channel of a 2D texture. In the second pass, using this
 * depth information, the occlusion factor is computed and used to scale the ambient light. A small noise texture is tiled through thw whole mesh to remove banding due to low number of samples needed to 
 * keep computational cost low. In the third pass, a gaussian blur is applied in order to remove the high frequency noise.
 *
 * Based on http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-simple-and-practical-approach-to-ssao-r2753
**/
class SSAO: public Effect {

protected:
    ///Noise texture dimension. It will be a noiseSize x noiseSize texture.
    int noise_size;

    ///Scale used to tile the noise texture through screen.
    Eigen::Vector2f noise_scale;

    ///Noise texture
    Texture noiseTexture;

    ///Array of sample points generated inside a unit hemisphere around z axis.
    float *kernel;

    ///Number of sample points that will be used per fragment for occlusion computation.
    int numberOfSamples;

    ///Kernel radius. If the distance between a sample point and the point for which the occlusion is being computed is larger than radius, the occlusion for this sample will be neglected.
    float radius;

    /// Framebuffer to store coord/normal buffer
    Framebuffer* fbo;

    /// The per pixel AO computation shader
    Shader* ssaoShader;

    ///
    Shader* deferredShader;

    ///
    Shader* blurShader;

    /// A quad mesh for framebuffer rendering
    Mesh* quad;

    /// Flag indicating wether blur shall be applied or not.
    bool apply_blur;

    /// Flag indicating if the mesh should be rendered only with ambient occlusion pass or with full illumination. If True, mesh will be rendered only with the ambient occlusion pass.
    bool displayAmbientPass;

    /// Number of neighbour pixels used in blurring. The blur will be applied to a blurRange x blurRange square around the current pixel. It's important to notice that blurRange must be an odd number.
    int blurRange;

    /// The ID defining the color attachment to which the depth texture is bound in the framebuffer.
    int depthTextureID;

    /// The ID defining the color attachment to which the normal texture is bound in the framebuffer.
    int normalTextureID;

    /// The ID defining the color attachment to which the color texture is bound in the framebuffer.
    int colorTextureID;

    /// Global intensity value.
    int intensity;

    /// Global maximum distance value, distance in view space to consider a neighbor or not
    float max_dist;

    /// The ID defining the color attachment to which the blur texture is bound in the framebuffer.
    GLuint blurTextureID;


public:

    /**
     * @brief Default constructor.
     *
     * It's important to notice that if the number of samples is modified here, it also needs to be changed in the shader code. It
	 * can't be passed as a uniform to the shader because it represents the size of the sample points' array.
	 * @param noiseTextureDimension The dimension of the noise texture to be generated.
	 * @param sampleKernelSize The size of the kernel array that will store the sample points. This means that a number of points equal to
	 * sampleKernelSize will be sampled in order to compute occlusion.
	 * @param rad The kernel radius. This is used to define the max distance between the current point and the samples that will be considered for occlusion computation.
	**/
    SSAO (int noiseTextureDimension = 64, int sampleKernelSize = 64, float rad = 20.0)
    {
        depthTextureID = 0;
        normalTextureID = 1;
        colorTextureID = 2;
        blurTextureID = 3;

        noise_size = noiseTextureDimension;
        numberOfSamples = sampleKernelSize;
        radius = rad;

        blurRange = 3;
        intensity = 20;
        max_dist = 0.15;

        apply_blur = false;
        displayAmbientPass = false;

        ssaoShader = 0;
        blurShader = 0;
        deferredShader = 0;
        fbo = 0;
        quad = 0;
        noise_scale = Eigen::Vector2f::Zero();
	}

    ///Default destructor. Destroy the FBO, deletes the shaders and the sampling kernel.
    ~SSAO()
    {
        delete fbo;
        delete kernel;
        delete quad;
    }

    /**
     * @brief Initializes the SSAO effects,
     *
     * generating the sample kernel, creating and loading the shaders and generating the noise texture.
     */
    virtual void initialize (void)
    {
        initializeShaders();
        generateKernel();
        generateNoiseTexture();
        Misc::errorCheckFunc(__FILE__, __LINE__);

        fbo = new Framebuffer();

        quad = new Mesh();
        quad->createQuad();
    }

	/**
     * @brief Renders the mesh with the desired effect.
	 * @param mesh A pointer to the mesh that will be rendered.
	 * @param cameraTrackball A pointer to the camera trackball object.
	 * @param lightTrackball A pointer to the light trackball object.
     */
    virtual void render(Mesh* mesh, Trackball* cameraTrackball, Trackball* lightTrackball)
    {
        Eigen::Vector4f viewport = cameraTrackball->getViewport();
        Eigen::Vector2i viewport_size = cameraTrackball->getViewportSize();

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        // check if viewport was modified, if so, regenerate fbo
        if (fbo->getWidth() != viewport_size[0] || fbo->getHeight() != viewport_size[1])
        {
            fbo->create(viewport_size[0], viewport_size[1], 4);
            computeNoiseScale(viewport_size);
        }

        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First pass - Depth Storage:

        // Bind buffer to store depth information in framebuffer:
        fbo->clearAttachments();
        fbo->bindRenderBuffers(depthTextureID, normalTextureID, colorTextureID);

        deferredShader->bind();
        deferredShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
        deferredShader->setUniform("modelMatrix",mesh->getModelMatrix());
        deferredShader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());
        deferredShader->setUniform("lightViewMatrix", lightTrackball->getViewMatrix());
        deferredShader->setUniform("has_color", mesh->hasAttribute("in_Color"));

        mesh->setAttributeLocation(deferredShader);
        mesh->render();

        deferredShader->unbind();
        fbo->unbind();
        fbo->clearDepth();

        // ******************** Second pass - SSAO Computation:

        if(apply_blur)
        {
            // Set draw buffer to blur texture:
            fbo->bindRenderBuffer(blurTextureID);
        }        

        ssaoShader->bind();

        ssaoShader->setUniform("modelMatrix", mesh->getModelMatrix());
        ssaoShader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());
        ssaoShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
        ssaoShader->setUniform("lightViewMatrix", lightTrackball->getViewMatrix());

        //ssaoShader->setUniform("noiseScale", noise_scale);
        ssaoShader->setUniform("kernel", kernel, 2, numberOfSamples);

        ssaoShader->setUniform("coordsTexture", fbo->bindAttachment(depthTextureID));
        ssaoShader->setUniform("normalTexture", fbo->bindAttachment(normalTextureID));
        ssaoShader->setUniform("colorTexture", fbo->bindAttachment(colorTextureID));
        ssaoShader->setUniform("displayAmbientPass", displayAmbientPass);

        ssaoShader->setUniform("radius", radius);
        ssaoShader->setUniform("intensity", (float)intensity);
        ssaoShader->setUniform("max_dist", max_dist);
        ssaoShader->setUniform("viewportSize", viewport_size);
        ssaoShader->setUniform("noiseTexture", noiseTexture.bind());

        quad->setAttributeLocation(ssaoShader);

        //Second pass mesh rendering:
        quad->render();

        ssaoShader->unbind();
        noiseTexture.unbind();
        fbo->unbind();
        fbo->clearDepth();

        Misc::errorCheckFunc(__FILE__, __LINE__);

        if(apply_blur)
        {
            //Third Pass - Blurring the scene:
            blurShader->bind();

            //Setting Uniforms:
            blurShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
            blurShader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());
            blurShader->setUniform("modelMatrix", mesh->getModelMatrix());

            blurShader->setUniform("blurTexture", fbo->bindAttachment(blurTextureID));
            blurShader->setUniform("blurRange", blurRange);

            mesh->setAttributeLocation(blurShader);
            mesh->render();

            blurShader->unbind();
            fbo->unbind();
        }
    }

    /**
     * @brief Set intensity values.
     * @param value New intensity value.
     */
    void setIntensity (int value)
    {
        intensity = value;
    }

    /**
     * @brief Toggles the current state of apply_blur flag.
     */
    void changeBlurFlag (void)
    {
        apply_blur = !apply_blur;
    }

    /**
     * Increases blur range.
     */
    void incrementBlurRange (void)
    {
		blurRange += 2;
	}

    /**
     * Decreases blur range.
     */
    void decrementBlurRange (void)
    {
		if(blurRange >=5) {
			blurRange -= 2;
		}
		else {
			blurRange = 3;
		}
	}

    /**
     * @brief Toggles the displayAmbientPass flag.
     */
    void changeAmbientPassFlag (void)
    {
        displayAmbientPass = !displayAmbientPass;
    }


private:

	///Computes the noise scale factor, that will be used for tiling the noise texture through the screen.
    void computeNoiseScale (const Eigen::Vector2i &viewport_size)
    {
        noise_scale = Eigen::Vector2f(viewport_size[0]/(float)noise_size, viewport_size[1]/(float)noise_size);
    }

    /**
     * @brief Creates and loads all shaders.
     */
    void initializeShaders (void)
    {
        ssaoShader = loadShader("ssao");
        deferredShader = loadShader("viewspacebuffer");
        blurShader = loadShader("gaussianblurfilter");
    }

    /**
     * @brief Generates a sampling kernel with random 2D unormalized vectors in range [-1,1].
     */
    void generateKernel (void)
    {
        Eigen::Vector2f sample;
        kernel = new float[numberOfSamples * 2];

        float step = 2.0*M_PI/(float)numberOfSamples;

        // divide in numberOfSamples directions in the unit circle, and multiply each vector by a random radius
        for (int i = 1; i <= numberOfSamples; i++)
        {          
            float r = pow(random(0.01, 1.0f),1.0);

            sample[0] = cos(step*i);
            sample[1] = sin(step*i);

            kernel[i*2+0] = r*sample[0];
            kernel[i*2+1] = r*sample[1];
        }
    }

    /**
     * @brief Generates a random noise texture.
     */
    void generateNoiseTexture (void)
    {
        GLfloat *noise = new GLfloat[noise_size*noise_size];

        for(int i = 0; i < noise_size*noise_size; i++)
        {
            noise[i] = random(0.0f,1.0f);
        }

        noiseTexture.create(GL_TEXTURE_2D, GL_RGBA32F, noise_size, noise_size, GL_RGBA, GL_FLOAT, noise);
        noiseTexture.setTexParameters( GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST );
        delete [] noise;
    }


    /**
     * @brief Generates a random number in range [min,max].
	 * @param min The minimum value for random number generation.
	 * @param max The maximum value for random number generation.
     */
    float random(float min, float max)
    {
        //srand ( time(NULL) );
        int random = rand();
        float ret = random / float(RAND_MAX);
        ret *= (max - min);
        ret += min;
        assert(ret >= min && ret <= max);
        return ret;
    }
};
}


#endif
