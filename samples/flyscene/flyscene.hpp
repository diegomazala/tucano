#ifndef __FLYSCENE__
#define __FLYSCENE__

#include <GL/glew.h>

#include <phongshader.hpp>
#include <utils/flycamera.hpp>
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

private:

	// A simple phong shader for rendering meshes
    Effects::Phong *phong;

	// A fly through camera
	Tucano::Flycamera *flycamera;

	// Light represented as a camera
	Tucano::Camera *light;
	
	// A mesh
	Mesh *mesh;
};

#endif // FLYSCENE
