#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <GL/glew.h>

#include <meanfilter.hpp>
#include <gradientfilter.hpp>
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

    /**
     * @brief Toggles mean filter flag
     */
    void toggleMean (void)
    {
        apply_mean = !apply_mean;
        updateGL();
    }

    /**
     * @brief Sets the kernel size of the mean filter.
     * @param kernel Size of kernel in pixels.
     */
    void setMeanKernel (int kernel)
    {
        meanfilter->setKernel(kernel);
        updateGL();
    }

    /**
     * @brief Toggles horizontal gradient filter flag
     */
    void toggleHGradient (void)
    {
        apply_hgradient = !apply_hgradient;
        gradientfilter->setDirections (apply_hgradient, apply_vgradient);
        updateGL();
    }

    /**
     * @brief Toggles vertical gradient filter flag
     */
    void toggleVGradient (void)
    {
        apply_vgradient = !apply_vgradient;
        gradientfilter->setDirections (apply_hgradient, apply_vgradient);
        updateGL();
    }


    
private:

    /// Mean filter effect.
    Effects::MeanFilter *meanfilter;

    /// Gradient filter effect.
    Effects::GradientFilter *gradientfilter;

    /// Render image effect (simply renders a texture)
    Effects::RenderTexture *rendertexture;

    /// FBO for multipass (enables applying more than one filter in sequence)
    Framebuffer fbo;

    /// Flag to apply mean filter.
    bool apply_mean;

    /// Flag to apply horizontal gradient filter.
    bool apply_hgradient;

    /// Flag to apply vertical gradient filter.
    bool apply_vgradient;

};

#endif // GLWIDGET
