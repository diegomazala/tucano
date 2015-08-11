#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <utils/qttrackballwidget.hpp>
#include <shapes/icosahedron.hpp>

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
    void initializeGL();

    /**
     * Repaints screen buffer.
     **/
    virtual void paintGL();


public slots:
	void onWireframeToggled(bool toggled);
	void onTessInnerChanged(int value);
	void onTessOuterChanged(int value);

private:


	Tucano::Shapes::Icosahedron* icosahedron;

	GLboolean wireframeEnabled;

	Tucano::Shader tess_shader;

	float tessLevelInner;
	float tessLevelOuter;
};

#endif // GLWIDGET
