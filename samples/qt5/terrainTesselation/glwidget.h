#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <utils/qtflycamerawidget.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <TerrainMesh.hpp>


class GLWidget : public Tucano::QtFlycameraWidget
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
	void onUpdateMeshButtonPressed();
	void onImageRioDeJaneiroToggled(bool toggled);
	void onImageLCGToggled(bool toggled);
	void onImageShapesToggled(bool toggled);
	void onWidthMeshChanged(int value);
	void onHeightMeshChanged(int value);
	void onDepthLevelChanged(int value);
	void onTessInnerChanged(int value);
	void onTessOuterChanged(int value);
	void onMinTessLevelChanged(int value);
	void onMaxTessLevelChanged(int value);
	void onMinDepthChanged(int value);
	void onMaxDepthChanged(int value);

private:

	void loadTextures();

	/// Texture to hold specular texture
	Tucano::Texture* height_map[3];

	Tucano::Shader shader;

	Tucano::TerrainMesh terrainMesh;

	GLboolean wireframeEnabled;
	int currentMap;

	float tessInnerLevel;
	float tessOuterLevel;
	float depthLevel;
	int terrainWidth;
	int terrainHeight;

	float minTessLevel;
	float maxTessLevel;
	float minDepth;
	float maxDepth;
};

#endif // GLWIDGET
