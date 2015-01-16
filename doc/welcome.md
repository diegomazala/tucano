Tucano
======

![tucano](tucano.png)

### Welcome to Tucano, a Library for rapid prototyping with modern OpenGL and GLSL.

Tucano encapsulates many common OpenGL/GLSL features in C++ classes, saving a lot of coding and allowing the developer to concentrate on the core of the application and the Shader codes.

By keeping the code **clean** and **compact**, Tucano also renders the code **less bug prone**.

## Features

* easily create and use Textures, FBOs, Shader Programs, Textures, modern OpenGL buffers...

* common classes used in most applications, such as Mesh, Camera, Trackball, and much more

* automatic texture unit assignment when binding textures, no need to worry about keeping track of ids

* uniforms, textures, and attributes are linked with the shaders in a simple and transparent way (no need to manually set locations)

* wrapper for Eigen Geometry, e.g. directly pass an *Eigen Matrix4f* to a GLSL Shader *mat4*

* use it with Qt, Glut, Glfw, or any other window manager that supports OpenGL

* default widgets for Qt, e.g. with a few lines of code open a 3D viewer with a trackball to visualize a Mesh

* examples and a collection of common effects (Phong, Image filters, SSAO...)

## Documentation

Documentation is available at http://lcg-ufrj.github.io/tucano


## Requirements

Tucano has only two library dependencies (make sure you have a recent version of them):

* Eigen : http://eigen.tuxfamily.org/
* GLEW : http://glew.sourceforge.net/
  
> Note that you need an OpenGL driver that supports at least version 4.3.

## Using

No need to compile Tucano, just include *tucano.hpp* in your project and be happy!

* use the `TUCANODEBUG` flag for debugging

## Examples

Tucano comes with several examples and an Effects Collection.

Check out the [Samples](@ref examples) page for the examples.
Check out the [Effects](@ref effects) Collection.

## Development Notice

Even though **Tucano 0.1** is out, there are still some features that are not implemented, such as encapsulation of 
*Tesselation Shaders*, and some *Buffer Objects* types. Some examples are still missing, but will soon be included in Tucano. Documentation is nearly up-to-date, but not 100%, completion is underway.

Please help us improve Tucano using GitHub's [Issue Tracker](https://github.com/LCG-UFRJ/tucano/issues/).

## Developers

Tucano is maintained by the [Computer Graphics Lab](http://www.lcg.ufrj.br) at [UFRJ](http://www.ufrj.br).

Tucano was initially developed by:

- [Ricardo Marroquim](http://www.lcg.ufrj.br/Members/ricardo)
- [Vitor de Andrade](http://www.lcg.ufrj.br/Members/vitorandrade)

A list of Tucano's contributors:

- [Thiago Perrotta](http://www.lcg.ufrj.br/Members/thiago)
- [Felipe Moura de Carvalho](http://www.lcg.ufrj.br/Members/fmc)
