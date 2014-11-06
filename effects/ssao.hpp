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

    ///
    Framebuffer* fbo;

    ///
    Shader* ssaoShader;

    ///The shader used to store the depth information in the framebuffer.
    Shader* deferredShader;

    ///The shader used to blur the result from the second pass, removing the noise pattern.
    Shader* blurShader;

    /// A quad mesh for framebuffer rendering
    Mesh* quad;

    ///Flag indicating wether blur shall be applied or not.
    bool apply_blur;

    ///Flag indicating if the mesh should be rendered only with ambient occlusion pass or with full illumination. If True, mesh will be rendered only with the ambient occlusion pass.
    bool displayAmbientPass;

    ///Number of neighbour pixels used in blurring. The blur will be applied to a blurRange x blurRange square around the current pixel. It's important to notice that blurRange must be an odd number.
    int blurRange;

    ///The ID defining the color attachment to which the depth texture is bound in the framebuffer.
    int depthTextureID;

    ///The ID defining the color attachment to which the blur texture is bound in the framebuffer.
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
    SSAO (int noiseTextureDimension = 4, int sampleKernelSize = 16, float rad = 0.0001):
         noise_size(noiseTextureDimension), numberOfSamples(sampleKernelSize), radius(rad), apply_blur(true), displayAmbientPass(false), blurRange(3), depthTextureID(0), blurTextureID(1){

        ssaoShader = 0;
        blurShader = 0;
        deferredShader = 0;
        fbo = 0;
        quad = 0;
        noise_scale = Eigen::Vector2f::Zero();

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
            fbo->create(viewport_size[0], viewport_size[1], 2);
            computeNoiseScale(viewport_size);
        }

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.7, 0.7, 0.7, 0.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //First pass - Depth Storage:

        //Bind buffer to store depth information in framebuffer:
        fbo->clearAttachments();
        fbo->bindRenderBuffer(depthTextureID);

        deferredShader->bind();
        deferredShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
        deferredShader->setUniform("modelMatrix",mesh->getModelMatrix());
        deferredShader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());
        deferredShader->setUniform("lightViewMatrix", lightTrackball->getViewMatrix());

        mesh->setAttributeLocation(deferredShader);
        mesh->render();

        deferredShader->unbind();
        fbo->unbind();        

        // ******************** Second pass - SSAO Computation:

        //Define standard color for mesh rendering:
        Eigen::Vector4f colorVector;
        colorVector << 0.686, 0.933, 0.933, 1.0;

        if(apply_blur)
        {
            // Set draw buffer to blur texture:
            fbo->bindRenderBuffer(blurTextureID);
        }

        ssaoShader->bind();        

        //Setting Uniforms:
        ssaoShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
        ssaoShader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());
        ssaoShader->setUniform("modelMatrix",mesh->getModelMatrix());

        ssaoShader->setUniform("default_color", colorVector);
        ssaoShader->setUniform("noiseScale", noise_scale);
        ssaoShader->setUniform("radius", radius);

        ssaoShader->setUniform("kernel", kernel, 3, numberOfSamples);
        ssaoShader->setUniform("noiseTexture", noiseTexture.bind());
        ssaoShader->setUniform("depthTexture", fbo->bindAttachment( depthTextureID ));
        ssaoShader->setUniform("displayAmbientPass", displayAmbientPass);
        ssaoShader->setUniform("viewportSize", viewport_size);

        mesh->setAttributeLocation(ssaoShader);

        //Second pass mesh rendering:
        mesh->render();

        ssaoShader->unbind();
        noiseTexture.unbind();
        fbo->unbind();

        if(apply_blur)
        {
            //Third Pass - Blurring the scene:

            blurShader->bind();

            Eigen::Vector2f texelSize (1.0/(float)viewport_size[0], 1.0/(float)viewport_size[1]);

            //Setting Uniforms:
            blurShader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
            blurShader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());
            blurShader->setUniform("modelMatrix", mesh->getModelMatrix());

            blurShader->setUniform("texelSize", texelSize);
            blurShader->setUniform("blurTexture", fbo->bindAttachment(blurTextureID));
            //blurShader->setUniform("blurTexture", fbo->bindAttachment( depthTextureID ));
            blurShader->setUniform("viewportSize", viewport_size);
            blurShader->setUniform("blurRange", blurRange);

            mesh->setAttributeLocation(blurShader);
            mesh->render();

            blurShader->unbind();
            fbo->unbind();
        }

        Misc::errorCheckFunc(__FILE__, __LINE__);
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

	///Change the current state of displayAmbientPass flag.
    void changeAmbientPassFlag()
    {
        displayAmbientPass = !displayAmbientPass;
    }

	///Reload all shaders needed for SSAO effect.
    virtual void reloadShaders (void)
    {
        deferredShader->reloadShaders();
        ssaoShader->reloadShaders();
        blurShader->reloadShaders();
    }

	///Default destructor. Destroy the FBO, deletes the shaders and the sampling kernel.
    ~SSAO()
    {
        if (fbo)
        {
            delete fbo;
        }

        if(ssaoShader)
        {
            delete ssaoShader;
		}

		if(deferredShader) {
			delete deferredShader;
		}

		if(blurShader) {
			delete blurShader;
		}

		if(kernel) {
			delete kernel;
		}

	}

private:

	///Computes the noise scale factor, that will be used for tiling the noise texture through the screen.
    void computeNoiseScale (const Eigen::Vector2i &viewport_size)
    {
        noise_scale = Eigen::Vector2f(viewport_size[0]/(float)noise_size, viewport_size[1]/(float)noise_size);
    }

	///Creates and loads all shaders.
    void initializeShaders()
    {
        //SSAO Computation Shader:
        ssaoShader = loadShader("ssao");
        ssaoShader->initialize();

        //Deferred Shader, used to store depth information:
        deferredShader = loadShader("deferredShader");
        deferredShader->initialize();

        //Blur Shader:
        blurShader = loadShader("blur");
        blurShader->initialize();

    }

	///Generates a sampling kernel.
    void generateKernel (void)
    {
        float scale;
        Eigen::Vector3f sample;
        kernel = new float[numberOfSamples*3];

        int temp = 0;
        for (int i = 0; i<numberOfSamples; i++)
        {
            sample = Eigen::Vector3f( random(-1.0f,1.0f) , random(-1.0f,1.0f) , random(0.0f,1.0f) );
            sample.normalize();
            //cout << "Kernel: " << sample.transpose() << endl;
            sample *= random(0.0f,1.0f); //Distribute sample points randomly around the kernel.
            scale = float(i)/float(numberOfSamples);
            scale = lerp(0.1f, 1.0f, scale * scale);//Cluster sample points towards origin.
            sample *= scale;
            kernel[temp] = sample[0];
            kernel[temp+1] = sample[1];
            kernel[temp+2] = sample[2];
            temp += 3;
        }
    }

	///Generates a random noise texture.
    void generateNoiseTexture()
    {

        float noise[noise_size*noise_size*4];//Noise is a texture with dimensions noiseSize x noiseSize.
        Eigen::Vector3f randomVector;

        for(int i = 0; i<numberOfSamples; i++) {

            randomVector = Eigen::Vector3f ( random(-1.0f,1.0f), random(-1.0f,1.0f), 0.0f);
            randomVector.normalize();
            (randomVector + Eigen::Vector3f(1.0,1.0,1.0)) * 0.5;
            noise[(i*4)] = randomVector[0];
            noise[(i*4)+1] = randomVector[1];
            noise[(i*4)+2] = 0.0;
            noise[(i*4)+3] = 1.0f;
        }

        //Generate Texture:
        noiseTexture.create( GL_TEXTURE_2D, GL_RGBA32F, 4, 4, GL_RGBA, GL_FLOAT, &noise[0]);
        noiseTexture.setTexParameters( GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST );
    }

    /**
     * Linear interpolation function. Interpolates between a and b with a scale factor s.
	 * @param a The minimum point for interpolation.
	 * @param b The maximum point for interpolation. 
	 * @param s The scale factor used for interpolation.
     */
    float lerp(const float a, const float b, const float s)
    {
        return a + s * (b - a);
    }

    /**
     * Generate random number in range [min,max].
	 * @param min The minimum value for random number generation.
	 * @param max The maximum value for random number generation.
     */
    float random(int min, int max)
    {
        //srand ( time(NULL) );
        int random = rand();
        float ret = random/float(RAND_MAX);
        ret *= (max - min);
        ret += min;
        assert(ret >= min && ret <= max);
        return ret;
    }


};
}


#endif
