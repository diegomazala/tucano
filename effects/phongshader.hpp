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

#ifndef __PHONG__
#define __PHONG__

#include <tucano.hpp>
#include <utils/trackball.hpp>

using namespace Tucano;

namespace Effects
{

/**
 * @brief Renders a mesh using a Phong shader.
 */
class Phong : public Effect
{

private:

    /// Phong Shader
    Shader phong_shader;

	/// Default color
	Eigen::Vector4f default_color;

public:

    /**
     * @brief Default constructor.
     */
    Phong (void)
    {
		default_color << 0.7, 0.7, 0.7, 1.0;
    }

    /**
     * @brief Default destructor
     *
     * No need to delete phong_shader, operation performed by inherited Effect class
     */
    virtual ~Phong (void) {}

    /**
     * @brief Load and initialize shaders
     */
    virtual void initialize (void)
    {
        // searches in default shader directory (/shaders) for shader files phongShader.(vert,frag,geom,comp)
        loadShader(phong_shader, "phongshader") ;
    }

	/**
	* @brief Sets the default color, usually used for meshes without color attribute
	*/
	void setDefaultColor ( Eigen::Vector4f& color )
	{
		default_color = color;
	}

    /** * @brief Render the mesh given a camera and light trackball, using a Phong shader * @param mesh Given mesh
     * @param cameraTrackball Given camera trackball
     * @param lightTrackball Given light trackball
     */
    void render (Tucano::Mesh& mesh, const Tucano::Camera& cameraTrackball, const Tucano::Camera& lightTrackball)
    {

        Eigen::Vector4f viewport = cameraTrackball.getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        phong_shader.bind();

        // sets all uniform variables for the phong shader
        phong_shader.setUniform("projectionMatrix", cameraTrackball.getProjectionMatrix());
        phong_shader.setUniform("modelMatrix", mesh.getModelMatrix());
        phong_shader.setUniform("viewMatrix", cameraTrackball.getViewMatrix());
        phong_shader.setUniform("lightViewMatrix", lightTrackball.getViewMatrix());
        phong_shader.setUniform("has_color", mesh.hasAttribute("in_Color"));
		phong_shader.setUniform("default_color", default_color);

        mesh.setAttributeLocation(phong_shader);

        glEnable(GL_DEPTH_TEST);
        mesh.render();

        phong_shader.unbind();
    }


};

}


#endif
