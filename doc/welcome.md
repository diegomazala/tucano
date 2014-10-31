Tucano                         {#mainpage}
======

### Welcome to Tucano, a Library for rapid prototyping with modern OpenGL and GLSL.

Tucano encapsulates many common OpenGL/GLSL features in C++ classes, saving a lot of coding and allowing the developer to concentrate on the core of the application and the Shader codes.

By keeping the code **clean** and **compact**, Tucano also renders the code **less bug prone**.

## Features

* easily create and use Textures, FBOs, Shader Programs, Textures, modern OpenGL buffers ...

* common classes used in most applications, such as Mesh, Camera, Trackball, and much more

* automatic texture unit assignment when binding textures, no need to worry about keeping track of ids

* uniforms, textures, and attributes are linked with the shaders in a simple and transparent way (no need to manually set locations)

* wrapper for Eigen Geometry, ex: directly pass an *Eigen Matrix4f* to a GLSL Shader *mat4*

* use it with Qt, Glut, Glfw, or any other window manager that supports OpenGL

* default widgets for Qt, ex. with a few lines of code open a 3D viewer with a trackball to visualize a Mesh

* examples and a collection of common effects (Phong, Image filters, SSAO ...)

## Requirements

Tucano has only two library dependencies (make sure you have recent version):

* Eigen : http://eigen.tuxfamily.org/
* GLEW : http://glew.sourceforge.net/
  
> Note that you need an OpenGL driver that supports at least version 4.3

## Using

No need to compile Tucano, just include *tucano.hpp* and be happy!

* use the TUCANODEBUG flag for debugging

## Examples

Check out the [Samples](@ref examples) page for the examples

Check out the [Effects](@ref effects) Collection

## Developers

Tucano is maintained by the [Computer Graphics Lab](http://www.lcg.ufrj.br) at [UFRJ](http://www.ufrj.br)

Tucano was initially developed by:

- Ricardo Marroquim
- Vitor Andrade

A list of Tucano's contributors:

- Felipe Moura
- Thiago Perrotta