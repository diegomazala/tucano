		#ifndef __FLYSCENE__
#define __FLYSCENE__

#include <GL/glew.h>

#include <phongshader.hpp>
#include <utils/flycamera.hpp>
#include <utils/camerapath.hpp>
#include <shapes/camerarep.hpp>
#include <mesh.hpp>
#include <utils/objimporter.hpp>

using namespace std;

class Flyscene 
{

public:

    explicit Flyscene(void);
    ~Flyscene();
    
    /**
     * @brief Initializes the shader effect
	 * @param width Window width in pixels
	 * @param height Window height in pixels
     */
    void initialize(int width, int height);

    /**
     * Repaints screen buffer.
     **/
    virtual void paintGL();

	/**
	* Returns the pointer to the flycamera instance
	* @return pointer to flycamera
	**/
	Flycamera* getCamera(void)
	{
		return &flycamera;
	}

	/**
	* Returns a pointer to the camerapath instance
	* @return pointer to camerapath
	*/
	CameraPath* cameraPath(void)
	{
		return &camerapath;
	}

	void addKeyPoint (void);

	void switchFlyCamera (void)
	{
		camera_type = 1;
	}

	void switchPathCamera (void)
	{
		camera_type = 2;
	}

private:

	// A simple phong shader for rendering meshes
    Effects::Phong phong;

	// A fly through camera
	Tucano::Flycamera flycamera;

	// A camera path for smooth camera animation
	Tucano::CameraPath camerapath;

	// A camera representation for animating path
	Tucano::Shapes::CameraRep camerarep;

	// Light represented as a camera
	Tucano::Camera light;

	// Path camera
	Tucano::Camera follow_cam;

	// Camera type to use for rendering (1=FlyCamera, 2=PathCamera)
	int camera_type;
	
	// A mesh
	Mesh mesh;
};

#endif // FLYSCENE
