Using the Texture Classe       {#texture_tutorial}
============================

An OpenGL texture in its simplest form, is an image. But it can also
be though as an array (1D, 2D, or 3D). Each array element is another
array of 1, 2, 3, or 4 elements.

The Texture Class is useful for easily:

* creating and updating Texture data
* changing and querying Texture parameters
* binding and unbinding

## Creating a Texture

Tucano does not read image files (jpg, png, tiff, etc...), you need an external library for this. The Texture receives an array with the image data. For example, with Qt:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QImage image (filename);
QImage glimage = QGLWidget::convertToGLFormat(image);
Tucano::Texture image_texture;
image_texture.create(glimage.width(), glimage.height(), glimage.bits());
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Passing a Texture to a Shader

To access a Texture from a Shader Program, the Texture must be bound to one of the Texture Units of the Graphics Card. This can be done manually by allocating and keeping track of which Texture is bound to which unit, and associating the same slot `id` in the Shader, using the `layout(binding=id)` qualifier, or be setting an Uniform.

Tucano makes everything transparent by using the [TextureManager](@ref Tucano::TextureManager) Class. It is a singleton class that is accessed by all Texture instances. Not that there is no need to include or use the Texture Manager directly. Every time you bind a Texture, the Texture Manager will look for the first empty unit, bind the Texture to that unit and return the unit id. To make things easier, a Texture can be bound and passed to a shader with a single call:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
shader.setUniform("imageTexture", image_texture.bind());
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Texture is readily available in the Shader code as:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uniform sampler2D imageTexture;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Don't forget to `unbind` the Texture after usage, otherwise you might quickly run out of Texture Slots.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
image_texture.unbind();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
