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
	flyscene->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	cout << "initialized" << endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);	
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		flyscene->getCamera()->reset();
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->strideLeft();
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->strideRight();
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->moveBack();
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->moveForward();
 	if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->moveDown();
	if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->moveUp();

}

static void mouseButtonCallback (GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		flyscene->getCamera()->startRotation( Eigen::Vector2f (xpos, ypos) );
	}
}
static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		flyscene->getCamera()->rotate(Eigen::Vector2f(xpos, ypos));
	}
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
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

	initialize();

	while (!glfwWindowShouldClose(window))
	{
		// render here
		flyscene->paintGL();

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
