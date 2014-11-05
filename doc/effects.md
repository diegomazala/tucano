Effects Collection                         {#effects}
==================

In the *effects* folder you can find a few useful Shader Effects that can be used directly with your own projects, or used as inspiration or for learning. Note that these Effects don't depend on the window manager.

All *effects* are composed of a simple class that inherits from the [Effect Class](@ref Tucano::Effect), and
contains one or more [Shaders](@ref Tucano::Shader).

* phongShader - a simple Phong Shader that receives a [Mesh](@ref Tucano::Mesh) and two [Trackballs](@ref Tucano::Trackball), one for the camera and one for the light direction.
* renderTexture - a simple shader that renders a given texture using a quad as proxy geometry.

## Using an Effect

The only necessary step is to include the `.hpp` file in your project, and make sure the directory containing the shaders are set corretly. 

For example, lets look at the phongShader effect, first include the file (make sure you have the include path set correctly so your compiler can find the file):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <phongShader.hpp>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
Then just create an instance of the phong effect, here we are creating as a pointer but it is not necessary:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Phong *phong = new Phong();
phong->setShadersDir("../effects/shaders/");
// make sure GLEW is initialized at this moment
phong->initialize();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

> Note that it is not necessary to create your effect variable as a pointer, just make sure the initialize() method is called after GLEW is initialized.

Once the effect is initialized it can be used to render a mesh. The Phong Shader example works by passing a Mesh, a camera Trackball and a light Trackball. A simple application draw method will look like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
glClearColor(1.0, 1.0, 1.0, 0.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

phong->render(mesh, camera_trackball, light_trackball);
camera_trackball->render(); // renders the trackball visual representation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
Some effects have set methods for the parameters, their usage is straightforward.
