Samples                         {#examples}
=======

We have created a few examples to help you get started. Most examples are created with Qt, but we have also created a basic GLUT and GLFW example.

In the *effects* folder you can find a few useful Shader Effects that are used with these examples, and can be reused with your own projects. Note that these Effects don't depend on the window manager.
Check out them:

- a simple Qt mesh viewer with a Phong Shader

- an Qt image processing application that uses an FBO to sequentially apply multiple filters, such as: Mean and Gradient Filter

- SSAO - Screen Space Ambient Occlusion


## Compiling the samples

There are two ways to do that: by using *qmake* or *cmake*.

### qmake

The Qt examples are accompanied with a **.pro** file, just run `qmake <example_name>.pro` to generate a Makefile, then run `make`.
The binaries are placed in the **bin** folder in your base **tucano** directory.

> Note: you might need to change the Eigen and Glew paths in the .pro file if their folders are not at the same directory level as tucano's.

### cmake

To compile the examples with cmake, first make sure you have a recent version of it (2.8.12+) installed on your system.

Now, create a `build` directory inside the tucano directory, cd into it, then run cmake in the standard way:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cd tucano/ # top-level directory of tucano
mkdir build/
cd build/
cmake ..
make
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will compile all tucano examples at once. They will be located under the `bin/` folder, inside the tucano top-level directory.
