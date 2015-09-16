#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>

int main() 
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) 
	{
		std::cerr << "ERROR: could not start GLFW3" << std::endl;
		return EXIT_FAILURE;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello GLFW", NULL, NULL);
	if (!window) 
	{
		std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string

	std::cout << "OpenGL   : " << version << std::endl;
	std::cout << "Renderer : " << renderer << std::endl;

	// close GL context and any other GLFW resources
	glfwTerminate();
	
	return EXIT_SUCCESS;
}