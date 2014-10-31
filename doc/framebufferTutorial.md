Using the Framebuffer Classe       {#framebuffer_tutorial}
============================

The Framebuffer Class holds a common and very useful OpenGL FBO.
FBOs are used mainly, but certainly not only, for offscreen rendering,
that is, tunneling the rendered image to a Texture instead to the screen.
This is very useful for multipass effects, where the output of the first render
pass is saved to a FBO Texture Attachment, that is received as input for the second pass.
The second pass might render directly to the screen buffer, or yet to another Texture
Attachment to serve as input for a third pass, and so on.

An FBO can hold up to a maximum number of textures (Attachments), having the restriction
that all of them have the same size.

The Framebuffer Class makes FBO creation and usage very easy:

- hides most of the usual burden of setting up an FBO
- easily bind one or more FBO Texture Attachment as output buffers
- easily bind an FBO Texture for accessing inside Shaders


# Creating an FBO

Creating an FBO with size *(w,h)* and *n* texture attachments.

First method:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Framebuffer *fbo;
fbo = new Tucano::Framebuffer(w, h, n);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Second method:s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Framebuffer *fbo;
fbo = new Tucano::Framebuffer(w, h, n);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Third method:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Framebuffer fbo;
fbo.create(w, h, n);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Reading from an FBO

Usually FBO are accessed as textures from the Shaders. Any Texture attachment can be handled directly as a [Texture](@ref Tucano::Texture) (see the [Texture Tutorial](@ref texture_tutorial) for more details).
It is possible to bind an Attachment and make it accessible in the Shader program with a single call:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
shader.setUniform("textureName", fbo.bindAttachment(attachID));
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In your Shader, the Texture will be available as a sampler. For a 2D texture for example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uniform sampler2D textureName;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

However, sometimes for debugging facility it is useful to read back the pixels from a given attachment. This can be easily achieve using the readPixels methods.


# Rendering to the FBO

To render to one specific Attachment:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
fbo.bindRenderBuffer(attachID);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

where *attachID* is an integer to indicate the ith attachment

To render to two Attachments:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
fbo.bindRenderBuffers(attachID1, attachID2);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are overloads to render up to 8 attachments, or passing a list of attachments as a reference.


