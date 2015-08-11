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

#ifndef __NORMALVECTOR__
#define __NORMALVECTOR__

#include <tucano.hpp>
#include <effect.hpp>

using namespace Tucano;

namespace Effects
{

/**
 * @brief Renders a mesh using a normal map as colors
 */
class NormalVector : public Tucano::Effect
{

public:

    /**
     * @brief Default constructor.
     */
    NormalVector ()
    {}

    /**
     * @brief Default destructor
     */
    virtual ~NormalVector (void ) {}

    /**
     * @brief Load and initialize shaders
     */
    virtual void initialize (void)
    {
		initGL();
        // searches in default shader directory (/shaders) for shader files NormalVector.(vert,frag,geom,comp)
         loadShader(normalvec_shader, "normalvector");
    }

    /**
     * @brief Render the mesh given a camera trackball
     * @param mesh Given mesh
     * @param cameraTrackball Given camera trackball     
     */
	virtual void render(Tucano::Mesh& mesh, const Tucano::Camera& camera)
    {
        Eigen::Vector4f viewport = camera.getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        normalvec_shader.bind();

        // sets all uniform variables for the phong shader
        normalvec_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
        normalvec_shader.setUniform("modelMatrix", mesh.getModelMatrix());
        normalvec_shader.setUniform("viewMatrix", camera.getViewMatrix());

        mesh.setAttributeLocation(normalvec_shader);

        glEnable(GL_DEPTH_TEST);
        mesh.render();
        glDisable(GL_DEPTH_TEST);

        normalvec_shader.unbind();
    }

private:

    /// NormalVector Shader
    Tucano::Shader normalvec_shader;
};

}

#endif	// __NORMALVECTOR__
