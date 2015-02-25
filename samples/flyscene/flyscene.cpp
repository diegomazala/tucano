#include "flyscene.hpp"

Flyscene::Flyscene(void)
{
    phong = 0;
	flycamera = 0;
	light = 0;
	camerapath = 0;
	follow_cam = 0;
	camera_type = 1;
}

Flyscene::~Flyscene()
{
	delete phong;
	delete flycamera;
	delete light;
	delete camerapath;
	delete follow_cam;
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
	// left side of the window (window x dim = 2*width+20)
	flycamera->setViewport(Eigen::Vector2f ((float)width, (float)height));

	camerapath = new CameraPath();

	camerarep = new Shapes::CameraRep();

	follow_cam = new Camera();
	follow_cam->setProjectionMatrix( flycamera->getProjectionMatrix() );
	//right side of window
	follow_cam->setViewport(Eigen::Vector4f ((float)width+20, 0.0, (float)width, (float)height));

	light = new Camera();

	mesh = new Mesh();
	MeshImporter::loadObjFile(mesh, "../samples/models/toy.obj");
	mesh->normalizeModelMatrix();
}
//Eigen::Vector3f last_pos;

void Flyscene::paintGL (void)
{
   	flycamera->updateViewMatrix();
   	Eigen::Vector4f viewport = flycamera->getViewport(); 
	
 	glClearColor(0.9, 0.9, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (phong && mesh)
    {
		if (camerapath->isAnimating())
			camerapath->stepForward();
//		if (camera_type == 1)
		{
		
        	phong->render(mesh, flycamera, light);
			// render coordinate systema at lower right corner
        	flycamera->renderAtCorner();
			// render the camera path and key points
			camerapath->render(flycamera, light);

			Eigen::Affine3f step_cam = camerapath->cameraAtCurrentTime();
			step_cam.scale(0.1);
			camerarep->setModelMatrix(step_cam);

			// render the animated camera
			camerarep->render(flycamera, light);
			Eigen::Affine3f path_cam_view = camerapath->cameraAtCurrentTime();
			follow_cam->setViewMatrix (path_cam_view.inverse());

			phong->render(mesh, follow_cam, light);
			
			float anim_time = camerapath->animTime();
			float anim_speed = camerapath->animSpeed();
			Tucano::Shapes::CoordinateAxes axes;
			for (int i = -2; i <= 2; ++i)
			{
				float t = anim_time + 6*i*anim_speed;
					Eigen::Affine3f cam = camerapath->pathAtArcLength(t);
					cam.scale(0.2);
					axes.setModelMatrix(cam);
					axes.render(*flycamera, *light);
					axes.resetModelMatrix();
					//camerarep->render(flycamera, light);
			}
 			

		}
    }
	// clear central margin black
	glScissor(viewport[2], viewport[1], 20, viewport[1]+viewport[3]);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(0.3, 0.6, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);


}
