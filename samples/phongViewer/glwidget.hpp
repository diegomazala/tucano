#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <GL/glew.h>

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
     * @todo remove hard coded mesh file!!
     */
    void initialize();

    /**
     * Repaints screen buffer.
     **/
    virtual void paintGL();


signals:
    
public slots:
    
private:

     Phong *phong;

};

#endif // GLWIDGET
