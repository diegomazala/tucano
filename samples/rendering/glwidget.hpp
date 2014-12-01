#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <GL/glew.h>

#include <ssao.hpp>
#include <phongshader.hpp>
#include <toon.hpp>
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
        toon->reloadShaders();
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
     * @brief Modifies the SSAO blur range value.
     * @param value New blur range value.
     */
    void setSSAOBlur (int value)
    {
        ssao->setBlurRange(value);
        updateGL();
    }


    /**
     * @brief Modifies the Toon quantization level.
     * @param value New quantization level.
     */
    void setToonQuantLevel (int value)
    {
        toon->setQuantizationLevel(value);
        updateGL();
    }

    /**
     * @brief Toggle draw trackball flag.
     */
    void toggleDrawTrackball (void)
    {
        draw_trackball = !draw_trackball;
        updateGL();
    }


private:

    /// Screen-Space Ambient Occlusion Effect
    Effects::SSAO *ssao;

    /// Simple Phong Shader
    Effects::Phong *phong;

    /// Simple Phong Shader
    Effects::Toon *toon;

    /// ID of active effect
    int active_effect;

    /// Flag to draw or not trackball
    bool draw_trackball;

};

#endif // GLWIDGET
