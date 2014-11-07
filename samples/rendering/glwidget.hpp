#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <GL/glew.h>

#include <ssao.hpp>
#include <phongshader.hpp>
#include <utils/qttrackballwidget.hpp>

using namespace std;

class GLWidget : public Tucano::QtTrackballWidget
{
    Q_OBJECT

public:

    explicit GLWidget(QWidget *parent);
    ~GLWidget();
    
    /**
     * @brief Initializes the shader effect
     */
    void initialize();

    /**
     * Repaints screen buffer.
     */
    virtual void paintGL();


signals:

public slots:

    /**
     * @brief Toggles mean filter flag
     */
    void toggleEffect (int id)
    {
        active_effect = id;
        updateGL();
    }
    

    /**
     * @brief Reload effect shaders.
     */
    void reloadShaders (void)
    {
        ssao->reloadShaders();
        phong->reloadShaders();
        updateGL();
    }

    /**
     * @brief Modifies the SSAO global intensity value.
     * @param value New intensity value.
     */
    void setSSAOIntensity (int value)
    {
        ssao->setIntensity(value);
        updateGL();
    }

    /**
     * @brief Modifies the SSAO global maximum distance value.
     * @param value New max dist value.
     */
    void setSSAOMaxDist (int value)
    {
        ssao->setMaxDist((float)value/100.0);
        updateGL();
    }

private:

    /// Screen-Space Ambient Occlusion Effect
    Effects::SSAO *ssao;

    /// Simple Phong Shader
    Effects::Phong *phong;

    /// ID of active effect
    int active_effect;

};

#endif // GLWIDGET
