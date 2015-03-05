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


using namespace Tucano;

namespace Effects
{

/**
 * @brief Renders a mesh using a normal map as colors
 */
class NormalMap : public Tucano::Effect
{

public:

    /**
     * @brief Default constructor.
     */
    NormalMap ()
    {}

    /**
     * @brief Default destructor
     */
    virtual ~NormalMap (void ) {}

    /**
     * @brief Load and initialize shaders
     */
    virtual void initialize (void)
    {
        // searches in default shader directory (/shaders) for shader files normalmap.(vert,frag,geom,comp)
         loadShader(normalmap_shader, "normalmap");
    }

    /**
     * @brief Render the mesh given a camera trackball
     * @param mesh Given mesh
     * @param cameraTrackball Given camera trackball     
     */
    virtual void render (Tucano::Mesh& mesh, const Tucano::Trackball& cameraTrackball)
    {
        Eigen::Vector4f viewport = cameraTrackball.getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        normalmap_shader.bind();

        // sets all uniform variables for the phong shader
        normalmap_shader.setUniform("projectionMatrix", cameraTrackball.getProjectionMatrix());
        normalmap_shader.setUniform("modelMatrix", mesh.getModelMatrix());
        normalmap_shader.setUniform("viewMatrix", cameraTrackball.getViewMatrix());

        mesh.setAttributeLocation(normalmap_shader);

        glEnable(GL_DEPTH_TEST);
        mesh.render();
        glDisable(GL_DEPTH_TEST);

        normalmap_shader.unbind();
    }

private:

    /// NormalMap Shader
    Tucano::Shader normalmap_shader;
};

}

#endif
