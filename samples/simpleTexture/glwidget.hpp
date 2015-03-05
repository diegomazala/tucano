#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <GL/glew.h>

#include <rendertexture.hpp>
#include <utils/qtplainwidget.hpp>

#include <QImage>

using namespace std;

class GLWidget : public Tucano::QtPlainWidget
{
    Q_OBJECT

public:

    explicit GLWidget(QWidget *parent);
    ~GLWidget (void);
    
    /**
     * @brief Initializes the shader effect
     */
    void initialize (void);

    /**
     * @brief Repaints screen buffer.
     */
    virtual void paintGL (void);

protected:
    
public slots:
    
private:

    /// Render image effect (simply renders a texture)
    Effects::RenderTexture rendertexture;

    /// Texture to hold input image
    Texture image_texture;

};

#endif // GLWIDGET
