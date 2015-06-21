
#include "GlutWindow.h"


//-----------------------------------------------------------------------------

std::list<GlutWindow*> GlutWindow::msWinInstances;
bool GlutWindow::msLeftMouseDown;

//-----------------------------------------------------------------------------

GlutWindow::GlutWindow(void)
{
   msLeftMouseDown = false;
   msWinInstances.push_back(this);
}

//-----------------------------------------------------------------------------

GlutWindow::~GlutWindow(void)
{
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if (*i == this)
      {
         msWinInstances.erase(i);
         return;
      }  
      
      i++;   
   }
}


void GlutWindow::Create(const char* pTitle,
						unsigned int displayMode,
						int w, int h,
						int x, int y)
{
	//glutInitContextVersion(glVersion[0], glVersion[1]);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(x, y);
	glutInitDisplayMode(displayMode);

	mWindowId	= glutCreateWindow(pTitle);
	mInit		= false;
	mWidth		= w;
	mHeight		= h;

	glutDisplayFunc(sDisplay);
	glutReshapeFunc(sReshape);
	glutMouseFunc(sMouse);
	glutMotionFunc(sMotion);
#ifdef FREEGLUT
	   glutMouseWheelFunc(sMouseWheel);
	   glutCloseFunc(sClose);
#endif   
	glutKeyboardFunc(sKeyDown);
	glutSpecialFunc(sSpecialDown);
	glutKeyboardUpFunc(sKeyUp);
	glutSpecialUpFunc(sSpecialUp);
	glutIdleFunc(sIdle);
	//glutTimerFunc(0, sTimer, 0);
}


//-----------------------------------------------------------------------------

void GlutWindow::OnResize(int w, int h)
{
	mWidth	= w;
	mHeight	= h;
}


void GlutWindow::OnTimer(int time, int value)
{
	if (value != 0)
		mFps = mFrameCount * 4,

	mFrameCount = 0;
	glutTimerFunc(250, sTimer, 1);
}


//-----------------------------------------------------------------------------

void GlutWindow::sClose(void)
{
   int CurrentWindow = glutGetWindow();
   
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnClose();
         //delete (*i);
         return;
      }
      
      i++;
   }
}

//-----------------------------------------------------------------------------

void GlutWindow::sReshape(int w, int h)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnResize(w,h);
      }
      
      i++;
   } 
   
}

//-----------------------------------------------------------------------------

void GlutWindow::sDisplay(void)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if (!(*i)->mInit)
      {
         (*i)->OnInit();
         (*i)->mInit = true;
      }
   
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnRender();
      }
      
      i++;
   }  
}

//-----------------------------------------------------------------------------

void GlutWindow::sMouse(int button, int updown, int x, int y)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         if (updown)
         {
            if (button == 0) 
               msLeftMouseDown = false;
            (*i)->OnMouseUp(button, x, y);
         }
         else
         {
            if (button == 0) 
               msLeftMouseDown = true;
            (*i)->OnMouseDown(button, x, y);   
         }
      }
      
      i++;
   }
}

//-----------------------------------------------------------------------------

void GlutWindow::sMotion(int x, int y)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         if (msLeftMouseDown)
            (*i)->OnLeftMouseDrag(x, y);
            
         (*i)->OnMouseMove(x, y);   
      }
      
       i++;
   }
}

//-----------------------------------------------------------------------------

void GlutWindow::sMouseWheel(int wheel_number, int direction, int x, int y)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnMouseWheel(wheel_number, direction, x, y);
      }
      
      i++;
   }  
}

//-----------------------------------------------------------------------------

void GlutWindow::sJoystick( unsigned int a, int b, int c, int d)
{
}

//-----------------------------------------------------------------------------

void GlutWindow::sSpecialUp(int key, int x, int y)
{
     int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnKeyUp(key, 0);
      }
      
      i++;
   }  
}

//-----------------------------------------------------------------------------

void GlutWindow::sKeyUp(unsigned char key, int x, int y)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnKeyUp(0, key);
      }
      
      i++;
   }  
}

//-----------------------------------------------------------------------------

void GlutWindow::sSpecialDown(int key, int x, int y)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnKeyDown(key, 0);
      }
      
      i++;
   }  
}

//-----------------------------------------------------------------------------

void GlutWindow::sKeyDown(unsigned char key, int x, int y)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnKeyDown(0, key);
      }
      
      i++;
   } 
}

//-----------------------------------------------------------------------------

void GlutWindow::sIdle(void)
{
   int CurrentWindow = glutGetWindow();
   std::list<GlutWindow*>::iterator i = msWinInstances.begin();
   
   while (i!=msWinInstances.end())
   {
      if ((*i)->mWindowId == CurrentWindow)
      {
         (*i)->OnIdle();
      }
      i++;
   } 
}



void GlutWindow::sTimer(int value)
{
	int CurrentWindow = glutGetWindow();
	std::list<GlutWindow*>::iterator i = msWinInstances.begin();

	while (i != msWinInstances.end())
	{
		if ((*i)->mWindowId == CurrentWindow)
		{
			(*i)->OnTimer(250, 1);
		}
		i++;
	}
}


//-----------------------------------------------------------------------------

void GlutWindow::SwapBuffers()
{
	glutSwapBuffers();
}


//-----------------------------------------------------------------------------

void GlutWindow::Repaint()
{
   glutPostWindowRedisplay(mWindowId);
}

//-----------------------------------------------------------------------------

void  GlutWindow::Fullscreen(bool bFullscreen)
{
   if (bFullscreen)
   {
      glutFullScreen();
   }
   else
   {
      int w = glutGet(GLUT_WINDOW_WIDTH);
      int h = glutGet(GLUT_WINDOW_HEIGHT);
      int x = glutGet(GLUT_WINDOW_X);
      int y = glutGet(GLUT_WINDOW_Y);
      
      glutPositionWindow(x,y);
	  glutReshapeWindow(w,h);
   }
}

//-----------------------------------------------------------------------------
   
void GlutWindow::Hide()
{
   glutHideWindow();
}

//-----------------------------------------------------------------------------
   
void GlutWindow::Show()
{
   glutShowWindow();
}   

//-----------------------------------------------------------------------------   
   
void  GlutWindow::Close()
{
   glutDestroyWindow(mWindowId);
}

//-----------------------------------------------------------------------------


