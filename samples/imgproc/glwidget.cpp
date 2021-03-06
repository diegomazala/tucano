#include "glwidget.hpp"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : Tucano::QtPlainWidget(parent)
{
    apply_mean = false;
    apply_hgradient = false;
    apply_vgradient = false;

}

GLWidget::~GLWidget()
{
}

void GLWidget::initialize (void)
{
    // the default is /shaders from your running dir
    string shaders_dir("../effects/shaders/");

    // initialize the shader effects
    meanfilter.setShadersDir(shaders_dir);
    meanfilter.initialize();

    gradientfilter.setShadersDir(shaders_dir);
    gradientfilter.initialize();

    rendertexture.setShadersDir(shaders_dir);
    rendertexture.initialize();

    // initialize texture
    QImage image ("../samples/images/camelo.jpg");
    QImage glimage = QGLWidget::convertToGLFormat(image);

    // initialize the FBO and filles last attachment with original image
    // the other two attachments are used for multipass
    fbo.create(glimage.width(), glimage.height(), 3);
    fbo.fillTexture(2, glimage.bits());
}

void GLWidget::paintGL (void)
{
    makeCurrent();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    Eigen::Vector2i viewport (this->width(), this->height()) ;

    // sets the read buffer as attachment 2 (original image)
    int write_tex = 0;
    int read_tex = 2;

    // for each filter, switches between read/write buffers
    // note that any number of filters can be attached to this example

    if (apply_mean)
    {
        fbo.bindRenderBuffer(write_tex);
        meanfilter.renderTexture(*fbo.getTexture(read_tex), viewport);
        fbo.unbind();
        read_tex = write_tex;
        write_tex = !write_tex;
    }
    if (apply_hgradient || apply_vgradient)
    {
        fbo.bindRenderBuffer(write_tex);
        gradientfilter.renderTexture(*fbo.getTexture(read_tex), viewport);
        fbo.unbind();
        read_tex = write_tex;
        write_tex = !write_tex;
    }

    // renders the resulting image (or original image if no filter was applied)
    rendertexture.renderTexture(*fbo.getTexture(read_tex), viewport);
}
