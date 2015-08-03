#ifndef __NORMAL_VIEWER_WINDOW_H__
#define __NORMAL_VIEWER_WINDOW_H__


#include <GL/glew.h>
#include "GlutWindow.h"

#include <phongshader.hpp>
#include <normalmap.hpp>
#include <normalvector.hpp>
#include <utils/flycamera.hpp>
#include <mesh.hpp>
#include <utils/objimporter.hpp>


class NormalVectorWindow : public GlutWindow
{
public:
	NormalVectorWindow();
	virtual ~NormalVectorWindow();


protected:
	virtual void OnRender(void);

	virtual void OnInit(void);

	virtual void OnClose();

	virtual void OnResize(int w, int h);

	virtual void OnKeyDown(int nKey, char cAscii);
	

private:
	// A simple phong shader for rendering meshes
	Effects::Phong phong;
	Effects::NormalVector normalVector;

	// Light represented as a camera
	Tucano::Camera* light;
	Tucano::Camera light2;

	// Default camera
	Tucano::Flycamera* camera;

	// A mesh
	Mesh mesh;

};

#endif  // __NORMAL_VIEWER_WINDOW_H__


 
 
 