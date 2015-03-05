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

#ifndef __RENDERTEXTURE__
#define __RENDERTEXTURE__

#include <tucano.hpp>

using namespace std;

using namespace Tucano;

namespace Effects
{

/**
 * @brief A simple effect to render a texture.
 **/
class RenderTexture : public Effect
{
public:
    /**
     * @brief Default Constructor.
     */
    RenderTexture (void)
    {
    }

    /**
     * @brief Deafult empty destructor.
     */
    ~RenderTexture (void) {}

    /**
     * @brief Initializes the effect, creating and loading the shader.
     */
    virtual void initialize()
    {
		loadShader(shader, "rendertexture");
        quad.createQuad();
    }

    /**
     * @brief Renders the given texture.
     *
     * Renders the given texture using a proxy geometry, a quad the size of the viewport
     * to hold the texture.
     */
    void renderTexture (Texture& tex, Eigen::Vector2i viewport)
    {
        glViewport(0, 0, viewport[0], viewport[1]);

        shader.bind();
        shader.setUniform("imageTexture", tex.bind());
        shader.setUniform("viewportSize", viewport);
        quad.render();

        shader.unbind();
        tex.unbind();
    }

private:

    /// The mean filter shader.
    Shader shader;

    /// A quad to be rendered forcing one call of the fragment shader per image pixel (its just a proxy geometry)
    Mesh quad;
};

}

#endif
