#ifndef __GL_WIDGET_H__
#define __GL_WIDGET_H__

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QImage>

#include "rendertexture.hpp"

class QKeyEvent;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const Q_DECL_OVERRIDE{ return QSize(50, 50); };
	QSize sizeHint() const Q_DECL_OVERRIDE{ return QSize(1024, 578); };

public slots:
	virtual void cleanup();

protected:	// methods

	virtual void initializeGL() Q_DECL_OVERRIDE;
	virtual void paintGL() Q_DECL_OVERRIDE;
	virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

protected : // attributes

	/// Render image effect (simply renders a texture)
	Effects::RenderTexture rendertexture;

	/// Texture to hold input image
	Texture image_texture;

	QImage image;
};


#endif // __GL_WIDGET_H__
