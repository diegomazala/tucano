#ifndef __SIMPLE_TEXTURE_WINDOW_H__
#define __SIMPLE_TEXTURE_WINDOW_H__


#include <GL/glew.h>
#include "GlutWindow.h"
#include "rendertexture.hpp"




class SimpleTextureWindow : public GlutWindow
{
public:
	SimpleTextureWindow();
	virtual ~SimpleTextureWindow();

	void SetFileImage(const std::string& filename);

protected:
	virtual void OnRender(void);

	virtual void OnInit(void);

	virtual void OnClose();

	virtual void OnResize(int w, int h);

	virtual void OnKeyDown(int nKey, char cAscii);
	
	void CreateTexture(const char* pFileName);



private:
	std::string fileImage;
	
	/// Render image effect (simply renders a texture)
	Effects::RenderTexture rendertexture;

	/// Texture to hold input image
	Texture image_texture;

};

#endif  // __SIMPLE_TEXTURE_WINDOW_H__


 
 
 