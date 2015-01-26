#include "GLFW/glfw3.h"
#include <iostream>

int main(int argc, char *argv[])
{

	GLFWwindow* window;

	if (!glfwInit())
		return 1;

	window = glfwCreateWindow(640, 480, "TUCANO :: Flythrough Camera", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose (window) )
	{
		// render here

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
