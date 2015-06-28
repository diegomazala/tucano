

#include <iostream>
#include <exception>

#include "simpleTextureWindow.h"



int main(int argc, char** argv)
{
	try
	{
		GlutApp::Init(&argc, argv);

		SimpleTextureWindow lWin;
		if (argc > 1)
			lWin.SetFileImage(argv[1]);
		else
			lWin.SetFileImage("../../../tucano.png");
		lWin.Create("glut-simpleTexture", GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

		GlutApp::Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "<Error> " << e.what() << std::endl;
	}

	return GlutApp::Exit();
}
 
 
 
 
 
 