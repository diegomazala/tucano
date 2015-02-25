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
 	if (key == GLFW_KEY_C && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->moveDown();
	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->getCamera()->moveUp();	
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
		flyscene->addKeyPoint();
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		flyscene->switchFlyCamera();
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		flyscene->switchPathCamera();
	if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->cameraPath()->stepBackward();
	if (key == GLFW_KEY_PERIOD && (action == GLFW_PRESS || action == GLFW_REPEAT))
		flyscene->cameraPath()->stepForward();
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		flyscene->cameraPath()->toggleAnimation();
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
	GLFWwindow* main_window;

	if (!glfwInit()) 
	{
    	std::cerr << "Failed to init glfw" << std::endl;
		return 1;
	}

	// double x dimension for splitview and add margin
	main_window = glfwCreateWindow(WINDOW_WIDTH*2 + 20, WINDOW_HEIGHT, "TUCANO :: Flythrough Camera", NULL, NULL);
	if (!main_window)
	{
		std::cerr << "Failed to create the GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(main_window);
	glfwSetKeyCallback(main_window, keyCallback);
	glfwSetMouseButtonCallback(main_window, mouseButtonCallback);
	glfwSetCursorPosCallback(main_window, cursorPosCallback);

	glfwSetInputMode(main_window, GLFW_STICKY_KEYS, true);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	initialize();


	while (!glfwWindowShouldClose(main_window))
	{
		glfwMakeContextCurrent(main_window);
		flyscene->switchFlyCamera();
		flyscene->paintGL();
		glfwSwapBuffers( main_window );

		glfwPollEvents();
	}

	glfwDestroyWindow(main_window);
	glfwTerminate();
	return 0;
}
