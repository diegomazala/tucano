#include <iostream>
#include <tucano.hpp>
#include <flyscene.hpp>
#include "GLFW/glfw3.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

Flyscene *flyscene;

void initialize (void)
{
	Misc::initGlew();
	flyscene = new Flyscene();	
}


int main(int argc, char *argv[])
{
	GLFWwindow* window;

	if (!glfwInit()) 
	{
    	std::cerr << "Failed to init glfw" << std::endl;
		return 1;
	}

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TUCANO :: Flythrough Camera", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create the GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		// render here

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
