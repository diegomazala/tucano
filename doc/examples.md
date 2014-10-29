Samples                         {#examples}
=======

We have created a few examples to help you get started. Most examples are created with Qt, but we have also created a basic GLUT and GLFW example.

In the *effects* folder you can find a few useful Shader Effects that are used with these examples, and can be reused with your own projects. Note that these Effects don't depend on the window manager.
Check out the 

* a simple Qt mesh viewer with a Phong Shader

* an Qt image processing application that uses an FBO to sequentially apply multiple filters, such as: Mean and Gradient Filter

* SSAO - Screen Space Ambient Occlusion


## Compiling the sampless

The Qt examples are accompanied with a *.pro* file, just run **"qmake examplename.pro"** to compile.
The binaries are placed in the **bin** folder in your base **tucano** folder.

* Note: you might need to change the Eigen and Glew paths in the .pro file if their folders are not at the same directory level as tucanos.