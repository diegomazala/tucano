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

#ifndef __NORMALMAP__
#define __NORMALMAP__

#include <tucano.hpp>

namespace Effects
{

/**
 * @brief Renders a mesh using a normal map as colors
 */
class BlurredNormalMap : public Tucano::Effect
{

public:

    /**
     * @brief Default constructor.
     */
    BlurredNormalMap ()
    {
        normalmap_shader = 0;
        blur_shader = 0;
    }

    /**
     * @brief Default destructor
     */
    virtual ~BlurredNormalMap (void ) {}

    /**
     * @brief Load and initialize shaders
     */
    virtual void initialize (void)
    {
        // searches in default shader directory (/shaders) for shader files normalmap.(vert,frag,geom,comp)
        normalmap_shader = loadShader("normalmap");
        blur_shader = loadShader("meanfilter");
        quad = new Tucano::Mesh();
        quad->createQuad();
        fbo = new Tucano::Framebuffer();

    }

    /**
     * @brief Render the mesh given a camera trackball
     * @param mesh Given mesh
     * @param cameraTrackball Given camera trackball     
     */
    virtual void render (Tucano::Mesh* mesh, Tucano::Trackball* cameraTrackball)
    {
        Eigen::Vector4f viewport = cameraTrackball->getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);


        if (fbo->getWidth() != (viewport[2]-viewport[1]) || fbo->getHeight() != (viewport[3]-viewport[1]))
        {
            fbo->create(viewport[2]-viewport[1], viewport[3]-viewport[1], 1);
        }

        // sets the FBO first (and only) attachment as output
        fbo->clearAttachments();
        fbo->bindRenderBuffer(0);

        normalmap_shader->bind();

        // sets all uniform variables for the phong shader
        normalmap_shader->setUniform("projectionMatrix", cameraTrackball->getProjectionMatrix());
        normalmap_shader->setUniform("modelMatrix", mesh->getModelMatrix());
        normalmap_shader->setUniform("viewMatrix", cameraTrackball->getViewMatrix());

        mesh->setAttributeLocation(normalmap_shader);

        glEnable(GL_DEPTH_TEST);
        mesh->render();
        glDisable(GL_DEPTH_TEST);

        normalmap_shader->unbind();


        // *** unbind the buffer as output buffer, and applies the blur filter ****
        fbo->unbind(); // automatically returns the draw buffer to GL_BACK

        blur_shader->bind();
        blur_shader->setUniform("imageTexture", fbo->bindAttachment(0));
        blur_shader->setUniform("kernelsize", 5);
        quad->render();
        blur_shader->unbind();
        fbo->unbindAttachments();
    }

private:

    /// NormalMap Shader
    Tucano::Shader *normalmap_shader;
    Tucano::Shader *blur_shader;
    Tucano::Mesh* quad;
    Tucano::Framebuffer* fbo;
};

}



#endif
