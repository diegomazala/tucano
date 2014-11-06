#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <GL/glew.h>

#include <ssao.hpp>
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
    
private:

    /// Screen-Space Ambient Occlusion Effect
    Effects::SSAO *ssao;

};

#endif // GLWIDGET
