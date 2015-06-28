

#include <iostream>
#include <exception>

#include "NormalVectorWindow.h"



int main(int argc, char** argv)
{
	try
	{
		GlutApp::Init(&argc, argv);

		NormalVectorWindow lWin;
		lWin.Create("glut-normalViewer", GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

		GlutApp::Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "<Error> " << e.what() << std::endl;
	}

	return GlutApp::Exit();
}
 
 
 
 
 
 