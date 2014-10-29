Tucano                         {#mainpage}
======

### Welcome to Tucano, a Library for rapid prototyping with modern OpenGL and GLSL.

Tucano encapsulates many common OpenGL features in C++ classes, saving a lot of coding and allowing the developer to concentrate on the core of the application and the Shader codes.

By keeping the code **clean** and compact, Tucano also renders the code **less bug prone**.

## Features

* easily create and use Textures, FBOs, Shader Programs, modern OpenGL buffers ...

* common classes used in most applications, such as Mesh, Camera, Trackball, and much more.

* automatic texture unit assignment when binding textures

* uniforms, textures, and attributes are linked with the shaders in a simple and transparente way (no need to manually set locations)

* wrapper for Eigen Geometry, for example, you can pass directly an Eigen Matrix4f to to a shader mat4

* use it with Qt, Glut, Glfw, or any other window manager that supports OpenGL.

## Requirements

Tucano has only two dependencies (make sure you have recent version):

* Eigen : http://eigen.tuxfamily.org/
* GLEW : http://glew.sourceforge.net/
  
** Note that you need an OpenGL driver that supports at least version 4.3

## Using

No need to compile Tucano, just include *tucano.hpp* and be happy!

* use the TUCANODEBUG flag for debugging


## Examples

Check out the Samples page for the [examples](@ref examples).
Check out the Effects Collection [effects](@ref effects).

There are many useful Shader Effects that you can use with your projects, or take a look at as examples.

## Developers

Tucano is maintained by [Computer Graphics Lab](http://www.lcg.ufrj.br) at [UFRJ](http://www.ufrj.br)

Tucano was initially developed by:

- Ricardo Marroquim
- Vitor Andrade

A list of Tucano's contributors:

- Felipe Moura
- Thiago Perrota