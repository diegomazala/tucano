#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <utils/qttrackballwidget.hpp>
#include <phongshader.hpp>
#include <rendertexture.hpp>
#include <framebuffer.hpp>


class GLWidget : public Tucano::QtTrackballWidget
{
    Q_OBJECT

public:

    explicit GLWidget(QWidget *parent) : Tucano::QtTrackballWidget(parent) {}
    ~GLWidget() {}
    
    /**
     * @brief Initializes the shader effect
     * @todo remove hard coded mesh file!!
     */
    void initializeGL();

    /**
     * Repaints screen buffer.
     **/
    virtual void paintGL();

public slots:
	
	virtual void cleanup();

private:
    Effects::Phong phong;

	/// Render image effect (simply renders a texture)
	Effects::RenderTexture rendertexture;

	Tucano::Framebuffer* fbo;
};

#endif // GLWIDGET
