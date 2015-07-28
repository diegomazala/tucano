#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <utils/qttrackballwidget.hpp>
#include <phongshader.hpp>
#include <parallaxmapping.hpp>
#include <texture.hpp>
#include <bumpmesh.hpp>


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
	void onImageBricksToggled(bool toggled);
	void onImageLCGToggled(bool toggled);
	void onImageShapesToggled(bool toggled);
	void onDiffuseMapEnabled(bool enabled);
	void onNormalMapEnabled(bool enabled);
	void onSpecularMapEnabled(bool enabled);
	void onParallaxMapEnabled(bool enabled);
	void onParallaxScaleChanged(double value);
	void onParallaxBiasChanged(double value);
	void onLightIntensityChanged(int value);

private:

	void loadTextures();

	/// Texture to hold diffuse texture
	Tucano::Texture* diffuse_map[3];

	/// Texture to hold normal texture
	Tucano::Texture* normal_map[3];

	/// Texture to hold specular texture
	Tucano::Texture* specular_map[3];

	/// Texture to hold specular texture
	Tucano::Texture* height_map[3];

	/// A simple normal mapping shader for rendering meshes
	Effects::ParallaxMapping parallaxMapping;

	Tucano::BumpMesh bumpMesh;


	GLboolean wireframeEnabled;
	int currentMap;
};

#endif // GLWIDGET
