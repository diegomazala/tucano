#ifndef __FLYSCENE__
#define __FLYSCENE__

#include <GL/glew.h>

#include <phongshader.hpp>
#include <utils/flycamera.hpp>
#include <utils/camerapath.hpp>
#include <utils/camerarep.hpp>
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
		return flycamera;
	}

	void addKeyPoint (void);

private:

	// A simple phong shader for rendering meshes
    Effects::Phong *phong;

	// A fly through camera
	Tucano::Flycamera *flycamera;

	// A camera path for smooth camera animation
	Tucano::CameraPath *camerapath;

	// A camera representation for animating path
	Tucano::CameraRep *camerarep;

	// Light represented as a camera
	Tucano::Camera *light;
	
	// A mesh
	Mesh *mesh;
};

#endif // FLYSCENE
