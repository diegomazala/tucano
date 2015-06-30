#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <utils/qttrackballwidget.hpp>
#include <phongshader.hpp>


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

private:
    Effects::Phong phong;
};

#endif // GLWIDGET
