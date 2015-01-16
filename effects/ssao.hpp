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

#include <tucano.hpp>
#include <cmath>

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

    /// Save coord, normal and color to FBO
    Shader* deferredShader;

    /// Join original render with SSAO (blur it first)
    Shader* ssaoFinalShader;

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

    /// The ID of the color attachment holding the SSAO result.
    int ssaoTextureID;

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
    SSAO (int noiseTextureDimension = 64, int sampleKernelSize = 64, float rad = 35.0)
    {
        depthTextureID = 0;
        normalTextureID = 1;
        colorTextureID = 2;
        ssaoTextureID = 3;
        blurTextureID = 4;

        noise_size = noiseTextureDimension;
        numberOfSamples = sampleKernelSize;
        radius = rad;

        blurRange = 3;
        intensity = 20;
        max_dist = 0.15;

        apply_blur = true;
        displayAmbientPass = false;

        ssaoShader = 0;
        ssaoFinalShader = 0;
        deferredShader = 0;
        fbo = 0;
        quad = 0;
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
     * @brief First pass of the SSAO, writes coords, normals and colors to a buffer.
     * @param mesh A pointer to the mesh that will be rendered.
     * @param cameraTrackball A pointer to the camera trackball object.
     * @param lightTrackball A pointer to the light trackball object.
     */
    void createViewSpaceBuffer (Mesh* mesh, Trackball* camera_trackball, Trackball* light_trackball)
    {

        // Bind buffer to store coord, normal and color information
        fbo->clearAttachments();
        fbo->bindRenderBuffers(depthTextureID, normalTextureID, colorTextureID);

        deferredShader->bind();
        deferredShader->setUniform("projectionMatrix", camera_trackball->getProjectionMatrix());
        deferredShader->setUniform("modelMatrix",mesh->getModelMatrix());
        deferredShader->setUniform("viewMatrix", camera_trackball->getViewMatrix());
        deferredShader->setUniform("lightViewMatrix", light_trackball->getViewMatrix());
        deferredShader->setUniform("has_color", mesh->hasAttribute("in_Color"));

        mesh->setAttributeLocation(deferredShader);
        mesh->render();

        deferredShader->unbind();
        fbo->unbind();
        fbo->clearDepth();

    }

    /**
     * @brief Compute the Ambient Occlusion factor for each pixel.
     */
    void computeSSAO (void)
    {

        fbo->bindRenderBuffer(ssaoTextureID);

        ssaoShader->bind();

        ssaoShader->setUniform("kernel", kernel, 2, numberOfSamples);

        ssaoShader->setUniform("coordsTexture", fbo->bindAttachment(depthTextureID));
        ssaoShader->setUniform("normalTexture", fbo->bindAttachment(normalTextureID));

        ssaoShader->setUniform("radius", radius);
        ssaoShader->setUniform("intensity", (float)intensity);
        ssaoShader->setUniform("max_dist", max_dist);
        ssaoShader->setUniform("noiseTexture", noiseTexture.bind());

        quad->setAttributeLocation(ssaoShader);
        quad->render();

        ssaoShader->unbind();
        noiseTexture.unbind();
        fbo->unbind();
        fbo->clearDepth();
    }


    /**
     * @brief Blur SSAO result and mix with original render
     * @param light_trackball A pointer to the light trackball object.
     */
    void applySSAO (Trackball* light_trackball)
    {
        ssaoFinalShader->bind();

        ssaoFinalShader->setUniform("lightViewMatrix", light_trackball->getViewMatrix());

        ssaoFinalShader->setUniform("coordsTexture", fbo->bindAttachment(depthTextureID));
        ssaoFinalShader->setUniform("normalTexture", fbo->bindAttachment(normalTextureID));
        ssaoFinalShader->setUniform("colorTexture", fbo->bindAttachment(colorTextureID));
        ssaoFinalShader->setUniform("ssaoTexture", fbo->bindAttachment(ssaoTextureID));
        ssaoFinalShader->setUniform("blurRange", blurRange);

        quad->setAttributeLocation(ssaoFinalShader);
        quad->render();

        ssaoFinalShader->unbind();
        fbo->unbind();
    }

	/**
     * @brief Renders the mesh with the desired effect.
     *
     * The algorithm has three passes:
     * 1. compute buffer with coords, normals and color per pixel
     * 2. compute AO per pixel
     * 3. blur the final result
     * An option to pass an output buffer is available in case of offline rendering.
     * For example, when taking snapshots of the current result.
	 * @param mesh A pointer to the mesh that will be rendered.
	 * @param cameraTrackball A pointer to the camera trackball object.
	 * @param lightTrackball A pointer to the light trackball object.
     * @param output_fbo Output buffer, alternative to GL back buffer for offline rendering
     * @param output_attach In case of offline rendering, the output attachment of the fbo
     */
    virtual void render(Mesh* mesh, Trackball* camera_trackball, Trackball* light_trackball)
    {
        Eigen::Vector4f viewport = camera_trackball->getViewport();
        Eigen::Vector2i viewport_size = camera_trackball->getViewportSize();

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        // check if viewport was modified, if so, regenerate fbo
        if (fbo->getWidth() != viewport_size[0] || fbo->getHeight() != viewport_size[1])
        {
            fbo->create(viewport_size[0], viewport_size[1], 5);
        }

        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // first pass
        createViewSpaceBuffer (mesh, camera_trackball, light_trackball);

        // second pass
        computeSSAO();

        // final pass, blur SSAO and join with original render
        applySSAO(light_trackball);
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
     * Increases blur range.
     */
    void setBlurRange (int value)
    {
        blurRange = value;
	}

    /**
     * @brief Toggles the displayAmbientPass flag.
     */
    void changeAmbientPassFlag (void)
    {
        displayAmbientPass = !displayAmbientPass;
    }


private:

    /**
     * @brief Creates and loads all shaders.
     */
    void initializeShaders (void)
    {
        ssaoShader = loadShader("ssao");
        deferredShader = loadShader("viewspacebuffer");
        ssaoFinalShader = loadShader("ssaofinal");
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
