Using the Framebuffer Classe       {#framebuffer_tutorial}
============================

The Framebuffer Class holds a common and very useful OpenGL FBO.
FBOs are used mainly, but certainly not only, for offscreen rendering,
that is, tunneling the rendered image to a Texture instead to the screen.

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

Second method:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Framebuffer *fbo;
fbo = new Tucano::Framebuffer(w, h, n);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Third method:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Framebuffer fbo;
fbo.create(w, h, n);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Attachments

Any Texture attachment can be handled directly as a Texture, see the [Texture Tutorial](@ref texture_tutorial) for more info.

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
