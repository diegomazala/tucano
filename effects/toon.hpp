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

#include <tucano.hpp>

using namespace Tucano;

namespace Effects
{

/**
 * @brief Renders a mesh using a Toon shader.
 */
class Toon : public Effect
{

private:

    /// Toon shader
    Shader toon_shader;

    /// Number of colors that will be used in color quantization to create the toonish effect.
    float quantization_level;

public:

	/**
     * @brief Default constructor.
     */
    Toon (void)
    {
        quantization_level = 8;
	}

    /**
     * @brief Default destructor
     */
    virtual ~Toon (void) {}

    /**
     * @brief Set the quantization level.
     * @param level Number of quantization levels.
     */
    void setQuantizationLevel(int level)
    {
        quantization_level = level;
    }

    /**
     * @brief Load and initialize shaders
     */
    virtual void initialize (void)
	{
		initGL();
		loadShader(toon_shader, "toonshader");
	}

    /**
     * @brief Render the mesh given a camera and light trackball, using a Toon shader
     * @param mesh Given mesh
     * @param cameraTrackball Given camera trackball
     * @param lightTrackball Given light trackball
     */
    virtual void render (Tucano::Mesh& mesh, const Tucano::Trackball& cameraTrackball, const Tucano::Trackball& lightTrackball)
	{       

        Eigen::Vector4f viewport = cameraTrackball.getViewport();
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        toon_shader.bind();

        toon_shader.setUniform("projectionMatrix", cameraTrackball.getProjectionMatrix());
        toon_shader.setUniform("modelMatrix", mesh.getModelMatrix());
        toon_shader.setUniform("viewMatrix", cameraTrackball.getViewMatrix());
        toon_shader.setUniform("lightViewMatrix", lightTrackball.getViewMatrix());
        toon_shader.setUniform("has_color", mesh.hasAttribute("in_Color"));
        toon_shader.setUniform("quantizationLevel", quantization_level);

        mesh.setAttributeLocation(toon_shader);
		mesh.render();

        toon_shader.unbind();
	}

};

}

#endif
