#include "flyscene.hpp"

Flyscene::Flyscene(void)
{
    phong = 0;
	flycamera = 0;
	light = 0;
	camerapath = 0;
}

Flyscene::~Flyscene()
{
	delete phong;
	delete flycamera;
	delete light;
	delete camerapath;
}

void Flyscene::addKeyPoint (void)
{
	camerapath->addKeyPosition(flycamera);
}


void Flyscene::initialize (int width, int height)
{
    // initialize the shader effect
    phong = new Effects::Phong();
    phong->setShadersDir("../effects/shaders/");
    phong->initialize();

	flycamera = new Flycamera();
	flycamera->setPerspectiveMatrix(60.0, width/(float)height, 0.1f, 100.0f);
	flycamera->setViewport(Eigen::Vector2f ((float)width, (float)height));

	camerapath = new CameraPath();

	camerarep = new CameraRep();

	light = new Camera();

	mesh = new Mesh();
	MeshImporter::loadObjFile(mesh, "../samples/models/toy.obj");
	mesh->normalizeModelMatrix();
}

void Flyscene::paintGL (void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	flycamera->updateViewMatrix();
    if (phong && mesh)
    {
        phong->render(mesh, flycamera, light);
        flycamera->render();
		camerapath->render(flycamera, light);

		Eigen::Affine3f step_cam = camerapath->cameraAtCurrentStep();
		step_cam.scale(0.1);
		camerarep->setModelMatrix(step_cam);
		camerarep->render(flycamera, light);
    }
}
