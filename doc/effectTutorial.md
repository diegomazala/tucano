Using the Effect and Shader Classes       {#effect_tutorial}
===================================

The [Effect](@ref Tucano::Effect) and [Shader](@ref Tucano::Shader) Classes hold the core of the OpenGL/GLSL applications.
A typical application has a Class (inheriting from [Effect](@ref Tucano::Effect)) that contains [Shader](@ref Tucano::Shader) objects. Many Effects are multipass and contain many shaders.

It is possible to build an application without using the Effect Class at all, however there a few advantages in using the Effect:

- keeps all shaders belonging to the same effect encapsulated
- keeps all variables for the effect encapsulated
- keeps your application API simple
- enables reloading all the effect's shaders with a single command
- enhances portability, to use the same effect in another application, only need to include one header

# A simple example effect 

Let's look at a simple Effect from Tucano's Collection, the [NormalMap](@ref Effects::NormalMap). 
This effect renders a mesh using normals directions as color (normal map).
We only need one Shader to create this effect.

First we create our class:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "effect.hpp"
class NormalMap : public Tucano::Effect {
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
This class contains a single [Shader](@ref Tucano::Effect):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Shader normalmap_shader;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
We need to overload the initialization method to load the shader. The `loadShader` method will look in the set shader directory for files beginning with "normalmap" and shader extensions, in this case it will find two (.vert and .frag). The method also initializes the shaders and keeps references to them for reloading and deleting.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
virtual void initialize ()
{
    loadShader(normalmap_shader, "normalmap");
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

> Note that the above method assumes that the shader files (.vert, .frag, etc...) are in the "/shaders" directory relative to your executing directory. You may need to change this directory before calling the initialization using the **setShadersDir** method.

> If you are using pointers for the Shaders, you can call *normalmap_shader = loadShader("normalmap");*

Now, all we need is the core of the NormalMap application. We can implement it as a method that receives a [Mesh](@ref Tucano::Mesh) and a camera [Trackball](@ref Tucano::Trackball), and renders the Mesh using the normal directions as colors:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
virtual void render (Tucano::Mesh& mesh, const Tucano::Trackball& cameraTrackball)
{
    Eigen::Vector4f viewport = cameraTrackball.getViewport();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    // enables the shader
    normalmap_shader.bind();

    // passes the transformation matrices to the shader as uniforms
    normalmap_shader.setUniform("projectionMatrix", cameraTrackball.getProjectionMatrix());
    normalmap_shader.setUniform("modelMatrix", mesh.getModelMatrix());
    normalmap_shader.setUniform("viewMatrix", cameraTrackball.getViewMatrix());

    // links the Mesh attributes with the Shader vertex attributes (in this case gl_Position and gl_Normal)
    mesh.setAttributeLocation(phong_shader);

    mesh.render();

    normalmap_shader.unbind();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
And that's it. 
In the application's draw routine, we clear the screen and call the render method above. To see a visual representation of the trackball just render it! Just make sure that depth test is enabled. Here is a sample code for a typical draw method (you can find a similar method in the GLWidget Class of the phongViewer sample):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
glClearColor(1.0, 1.0, 1.0, 0.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

normalmap_shader.render(mesh, camera_trackball);
camera_trackball.render();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here are the *vertex* and *fragment* shader codes for the files "normalmap.vert" and "normalmap.frag" (they are all included in the `effects/shaders` directory):

### normalmap.vert

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#version 430
	 
in vec4 in_Position;
in vec3 in_Normal;    
out vec3 normal;
	 
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
	 
void main(void)
{
    normal = in_Normal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;    
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
### normalmap.frag

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#version 430
	 
in vec3 normal;
out vec4 out_Color;
	 
void main(void)
{
    out_Color = vec4(abs(normal), 1.0);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    

# A more complex example

Let's say we want to create an effect called BlurredNormals. Its only an illustrative example.
This effect uses the normalmap above and then blurs the final resulting image.
It's a two pass method: first pass stores the normalmap result in an FBO; second pass blurs the
resulting image from the first pass. If you are not familiar with FBOs, check out the [Framebuffer Tutorial](@ref framebuffer_tutorial).    
    
We now need two Shaders plus a Quad Mesh and an FBO for the screen space blur:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tucano::Shader normalmap_shader;
Tucano::Shader blur_shader;
Tucano::Mesh quad;
Tucano::Framebuffer fbo;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The initialization method is completed as follows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
virtual void initialize ()
{
    loadShader(normalmap_shader, "normalmap");
    loadShader(blur_shader, "meanfilter");    
    quad.createQuad();
    fbo.create(w, h, 1);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

> Note that you need to pass the size of your viewport to initialize the FBO, we assume we know this and its (w,h). The last parameter of the FBO says that we only need one texture attachment. It's usually a good idea to initialize and re-initialize the FBO every time your window is resized (checkout the [QtTrackballWidget](@ref Tucano::QtTrackballWidget) for an example).

We show another way to set the FBO size below, after this example.

The render method now needs two passes, one to store the result in a FBO, and the second to blur the image:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

virtual void render (Tucano::Mesh& mesh, const Tucano::Trackball& cameraTrackball)
{
    // clears the FBO and sets the FBO first (and only) attachment as output
    fbo.clearAttachments();
    fbo.bindRenderBuffer(0);
    
    // *** this part of the code is identical to the first example ***
    Eigen::Vector4f viewport = cameraTrackball.getViewport();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    // enables the shader
    normalmap_shader.bind();

    // passes the transformation matrices to the shader as uniforms
    normalmap_shader.setUniform("projectionMatrix", cameraTrackball.getProjectionMatrix());
    normalmap_shader.setUniform("modelMatrix", mesh.getModelMatrix());
    normalmap_shader.setUniform("viewMatrix", cameraTrackball.getViewMatrix());

    // links the Mesh attributes with the Shader vertex attributes (in this case gl_Position and gl_Normal)
    mesh.setAttributeLocation(phong_shader);
    mesh.render();
    phong_shader.unbind();
    
    // *** unbind the buffer as output buffer, and applies the blur filter ****
    fbo.unbind(); // automatically returns the draw buffer to GL_BACK
    
    blur_shader.bind();
    // makes the FBO attachment read available from the shader
    blur_shader.setUniform("imageTexture", fbo.bindAttachment(0));
    // sets the blur kernel (higher == more)
    blur_shader.setUniform("kernelsize", 5);

    quad.render();
    blur_shader.unbind();
    fbo.unbindAttachments();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
And that is all!!
The blur shaders files (meanfilter.vert and meanfilter.frag) are in the *effects/shaders* directory of the Tucano Lib.

## Alternative to setting the FBO size during the initialization.
   
During the render method check if the FBO size is the same as the Viewport size, otherwise re-create the FBO (it will be initialized with zero)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Eigen::Vector4f viewport = cameraTrackball.getViewport();
if (fbo.getWidth() != (viewport[2]-viewport[1]) || fbo.getHeight() != (viewport[3]-viewport[1]))
{
    fbo.create(viewport[2]-viewport[1], viewport[3]-viewport[1], 1);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
> Note that usually the first two elements of the viewport vector are zero (ex. [0, 0, w, h]). However, we have left it with as four elements for completeness.
