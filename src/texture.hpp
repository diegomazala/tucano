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

#ifndef __TEXTURE__
#define __TEXTURE__

#include <texturemanager.hpp>
#include <iostream>
#include <GL/glew.h>

using namespace std;

namespace Tucano
{

/**
 * @brief An OpenGL texture.
 * It can be a simple texture or an FBO texture.
 */
class Texture {
private:

    /// ID handler to texture.
    GLuint tex_id;

    /// Type of texture (ex. GL_TEXTURE_2D, GL_TEXTURE_3D ...).
    GLenum tex_type;

    /// Internal format (ex. GL_RGBA, GL_RGBA32F ...).
    GLenum internal_format;

    /// Width in pixels.
    int width;

    /// Height in pixels.
    int height;

    /// Depth in pixels (for 3D texture).
    int depth;

    /// Format of texture channels (ex. GL_RGBA).
    GLenum format;

    /// Type of pixel channel (ex. GL_FLOAT, GL_UNSIGNED_BYTE ...).
    GLenum pixel_type;

    /// Number of lod levels (1 for textures without lod).
    int lod;

    /// Texture unit this texture is occupying (if any).
    int unit;

public:

    /**
     * @brief Default Constructor.
     */
    Texture (void)
    {
        tex_id = 0;
    }

    /**
     * @brief Default Destructor.
     * Deletes the texture.
     */
    ~Texture (void)
    {
        destroy();
    }


    /**
     * @brief Creates a texture object and returns its handler.
     * @param type Type of GL texture (usually GL_TEXTURE_2D)
     * @param int_format Format of texel (usually GL_RGBA or GL_RGBA32F for precision)
     * @param w Width of texture
     * @param h Height of texture
     * @param fmt Format of texel channels (usually GL_RGBA)
     * @param pix_type Type of one channel of a texel (usually GL_FLOAT or GL_USIGNED_BYTE)
     * @param data Pointer to data to fill the texture
     * @param dpt
     * @return Texture ID (handler for OpenGL)
     */
    GLuint create (GLenum type, GLenum int_format, int w, int h, GLenum fmt, GLenum pix_type, const GLvoid* data = NULL, int dpt = 256)
    {
        tex_type = type;
        internal_format = int_format;
        width = w;
        height = h;
        format = fmt;
        pixel_type = pix_type;
        lod = 0;
        depth = dpt;

        if (tex_id != 0)
            glDeleteTextures(1, &tex_id);

        glGenTextures(1, &tex_id);        

        glBindTexture(tex_type, tex_id);
        if(tex_type == GL_TEXTURE_2D || tex_type == GL_TEXTURE_RECTANGLE)
        {            
            glTexImage2D(tex_type, lod, internal_format, width, height, 0, format, pixel_type, data);           
        }
        else if (tex_type == GL_TEXTURE_3D)
        {
            glTexImage3D(tex_type, lod, internal_format, width, height, depth, 0, format, pixel_type, data);
        }
        else if (tex_type == GL_TEXTURE_1D)
        {
            glTexImage1D(tex_type, lod, internal_format, width, 0, format, pixel_type, data);
        }

        // default parameters
        setTexParameters();

        glBindTexture(tex_type, 0);
        return tex_id;
    }

    /**
     * @brief Creates a texture object with typical default parameters and returns its handler.
     *
     * The default parameters are texture type (GL_TEXTURE_2D), internal format (GL_RGBA32F), format (GL_RGBA), pixel type (GL_UNSIGNED_BYTE)
     * @param w Width of texture
     * @param h Height of texture
     * @param data Pointer to data to fill the texture
     * @return Texture ID (handler for OpenGL)
     */
    GLuint create (int w, int h, const GLvoid* data = NULL)
    {
        return create (GL_TEXTURE_2D, GL_RGBA32F, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data, 0);
    }


    /**
     * @brief Deletes the texture.
     */
    void destroy (void)
    {
        if (tex_id != 0) {
            glDeleteTextures(1, &tex_id);
        }
        tex_id = 0;
    }

    /**
     * @brief Sets texture parameters.
     * Sets the Wrap S and T, and Min and Mag filter parameters.
     * @param wraps Wrap S
     * @param wrapt Wrap T
     * @param magfilter Mag Filter
     * @param minfilter Min Filter
     */
    void setTexParameters (GLenum wraps = GL_CLAMP, GLenum wrapt = GL_CLAMP, GLenum magfilter = GL_NEAREST, GLenum minfilter = GL_NEAREST)
    {
        glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, wraps);
        glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, wrapt);
        glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, magfilter);
        glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, minfilter);
    }

    /**
     * @brief Sets texture parameters.
     * Sets the Wrap S and T, Min and Mag filter, and MipMap parameters.
     * Also generates the MipMap at the end.
     * @param maxlevel Max Mipmap level
     * @param baselevel Mipmap Base Level
     * @param wraps Wrap S
     * @param wrapt Wrap T
     * @param magfilter Mag Filter
     * @param minfilter Min Filter
     */
    void setTexParametersMipMap (int maxlevel, int baselevel = 0, GLenum wraps = GL_CLAMP, GLenum wrapt = GL_CLAMP, GLenum magfilter = GL_NEAREST, GLenum minfilter = GL_NEAREST_MIPMAP_NEAREST)
    {
        glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, minfilter);
        glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, magfilter);
        glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, wraps);
        glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, wrapt);
        glTexParameteri(tex_type, GL_TEXTURE_BASE_LEVEL, baselevel );
        glTexParameteri(tex_type, GL_TEXTURE_MAX_LEVEL, maxlevel );

        glGenerateMipmap(tex_type);
    }


    /**
     * @brief Updates the data of the texture mantaining all other parameters.
     * @param data Pointer to data to fill the texture.
    **/
    void update (const GLvoid* data)
    {

        glBindTexture(tex_type, tex_id);
        if(tex_type == GL_TEXTURE_2D || tex_type == GL_TEXTURE_RECTANGLE) {
            glTexImage2D(tex_type, lod, internal_format, width, height, 0, format,pixel_type,data);
        }
        else if (tex_type == GL_TEXTURE_3D) {
            glTexImage3D(tex_type, lod, internal_format,width, height, depth, 0,format,pixel_type,data);
        }
        else if (tex_type == GL_TEXTURE_1D) {
            glTexImage1D(tex_type, lod, internal_format,width,0,format,pixel_type,data);
        }
        glBindTexture(tex_type,0);

    }

    /**
     * @brief Binds the texture to a given unit.
     * Note that if there is another texture already binded to this unit,
     * no warning is given, and this texture will replace it.
     * @param texture_unit Given texture unit.
    **/
    void bind (int texture_unit)
    {
        unit = texture_unit;
        texManager.bindTexture(tex_type, tex_id, texture_unit);
    }

    /**
     * @brief Binds the texture to the first free texture unit and returns the unit used.
     * Gets the first free unit from the texture manager to bind the texture.
     * @return Unit the texture was bound to, or -1 if no unit available.
     */
    int bind (void)
    {
        unit = texManager.bindTexture(tex_type, tex_id);
        return unit;
    }

    /**
     * @brief Binds the texture as an image texture.
     * Image textures are used for example for atomic counters, compute shaders, and other uses.
     * Binds with READ and WRITE access using the current internal format.
     * Gets the first free unit from the texture manager to bind the texture.
     * @return Unit the texture was bound to, or -1 if no unit available.
     */
    int bindImageRW (void)
    {
        unit = texManager.bindTexture(tex_type, tex_id);
        if (unit != -1)
        {
            glBindImageTexture(unit, tex_id, 0, false, 0,  GL_READ_WRITE, internal_format);
        }
        return unit;
    }

    /**
     * @brief Binds the texture as an image texture to a given unit.
     * Image textures are used for example for atomic counters, compute shaders, and other uses.
     * Binds with READ and WRITE access.
     * If another texture is using the given unit it will be replaced by the current one.
     * @param texture_unit Texture unit to bound.
     */
    void bindImageRW (int texture_unit)
    {
        unit = texture_unit;
        texManager.bindTexture(tex_type, tex_id, texture_unit);
        glBindImageTexture(texture_unit, tex_id, 0, false, 0,  GL_READ_WRITE, internal_format);
    }

    /**
     * @brief Binds the texture as an image texture with a given format.
     * Binds with READ and WRITE access with a given format that might be different from texture internal format.
     * @param fmt Internal elements format (ex. GL_R32UI).
     * @return Unit the texture was bound to, or -1 if no unit available.
     */
    int bindImageFormatRW (GLenum fmt)
    {
        unit = texManager.bindTexture(tex_type, tex_id);

        if (unit != -1)
        {
            glBindImageTexture(unit, tex_id, 0, false, 0,  GL_READ_WRITE, fmt);
        }
        return unit;
    }

    /**
     * @brief Binds the texture as an image texture to a given texture unit with a given internal format.
     * Binds with READ and WRITE access with a given format that might be different from texture internal format.
     * If another texture is using the given unit it will be replaced by the current one.
     * @param texture_unit Texture unit to bound.
     * @param fmt Internal elements format (ex. GL_R32UI).
     */
    void bindImageFormatRW (int texture_unit, GLenum fmt)
    {
        unit = texture_unit;
        texManager.bindTexture(tex_type, tex_id, texture_unit);
        glBindImageTexture(texture_unit, tex_id, 0, false, 0,  GL_READ_WRITE, fmt);
    }

    /**
     * @brief Unbinds this texture and frees the texture unit.
     */
    void unbind (void)
    {
        texManager.unbindTextureID(tex_type, tex_id);
        unit = -1;
    }

    /**
     * @brief Returns the texture handle (texture ID).
     * @return Texture ID.
     */
    GLuint texID (void) const
    {
        return tex_id;
    }

    /**
     * @brief Returns the texture unit this texture is bound to.
     * @return The bound texture unit.
     */
    int textureUnit (void) const
    {
        return unit;
    }
};

}

#endif
