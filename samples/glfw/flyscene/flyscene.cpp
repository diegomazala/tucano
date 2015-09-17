#include "flyscene.hpp"

Flyscene::Flyscene(void)
{
	camera_type = 1;
}

Flyscene::~Flyscene()
{}

void Flyscene::addKeyPoint (void)
{
	camerapath.addKeyPosition(flycamera);
}


void Flyscene::initialize (int width, int height)
{
    // initialize the shader effect
    phong.setShadersDir("../../../effects/shaders/");
    phong.initialize();

	flycamera.setPerspectiveMatrix(60.0, width/(float)height, 0.1f, 100.0f);
	// left side of the window (window x dim = 2*width+20)
	flycamera.setViewport(Eigen::Vector2f ((float)width, (float)height));

	follow_cam.setProjectionMatrix( flycamera.getProjectionMatrix() );
	//right side of window
	follow_cam.setViewport(Eigen::Vector4f ((float)width+20, 0.0, (float)width, (float)height));

	MeshImporter::loadObjFile(&mesh, "../../../samples/models/toy.obj");
	
	mesh.normalizeModelMatrix();
}
//Eigen::Vector3f last_pos;

void Flyscene::paintGL (void)
{
   	flycamera.updateViewMatrix();
   	Eigen::Vector4f viewport = flycamera.getViewport(); 
	
 	glClearColor(0.9, 0.9, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if (camerapath.isAnimating())
		camerapath.stepForward();
	
	phong.render(mesh, flycamera, light);
	// render coordinate system at lower right corner
	flycamera.renderAtCorner();
	// render the camera path and key points
	camerapath.render(flycamera, light);

	// render the animated camera
	Eigen::Affine3f step_cam = camerapath.cameraAtCurrentTime();
	step_cam.scale(0.1);
	camerarep.setModelMatrix(step_cam);
	//camerarep.render(flycamera, light); //trouble here

	Eigen::Affine3f path_cam_view = camerapath.cameraAtCurrentTime();
	follow_cam.setViewMatrix (path_cam_view.inverse());

	phong.render(mesh, follow_cam, light);
	
	// clear central margin black
	glScissor(viewport[2], viewport[1], 20, viewport[1]+viewport[3]);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.3, 0.6, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);


}
