#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtPlainWidget(parent)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initialize (void)
{
    // the default is /shaders from your running dir
    string shaders_dir("../effects/shaders/");

    rendertexture.setShadersDir(shaders_dir);
    rendertexture.initialize();

    // initialize texture with given image
    QImage image ("../samples/images/camelo.jpg");
    QImage glimage = QGLWidget::convertToGLFormat(image);
    image_texture.create(glimage.width(), glimage.height(), glimage.bits());
}


void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // renders the given image, not that we are setting a fixed viewport that follows the widgets size
    // so it may not be scaled correctly with the image's size (just to keep the example simple)
    Eigen::Vector2i viewport (this->width(), this->height());
    rendertexture.renderTexture(image_texture, viewport);
}
