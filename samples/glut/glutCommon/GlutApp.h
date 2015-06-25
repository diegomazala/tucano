

#ifndef __GLUT_APPLICATION_H__
#define __GLUT_APPLICATION_H__

#include <freeglut.h>


namespace GlutApp
{
	static void Init(int* argc, char** argv)
	{
		glutInit(argc, argv);
	}

	static void Run()
	{
		glutMainLoop();
	}

	static int Exit()
	{
		std::exit(EXIT_SUCCESS);
	}

};






#endif // __GLUT_APPLICATION_H__