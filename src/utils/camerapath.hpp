/**
 * Tucano - A library for rapid prototying with Modern OpenGL and GLSL
 * Copyright (C) 2014
 * LCG - Laboratório de Computação Gráfica (Computer Graphics Lab) - COPPE
 * UFRJ - Federal University of Rio de Janeiro
 *
 * This file is part of Tucano Library.
 *
 * Tucano Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tucano Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tucano Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CAMERAPATH__
#define __CAMERAPATH__

#include "camera.hpp"
#include "mesh.hpp"
#include "utils/plyimporter.hpp"
#include "phongshader.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

/// Default fragment shader for rendering trackball representation.
const string camerapath_fragment_code = "\n"
        "#version 430\n"
        "in vec4 ex_Color;\n"
        "out vec4 out_Color;\n"
        "in float depth;\n"
        "void main(void)\n"
        "{\n"
        "    out_Color = ex_Color;\n"
        "    gl_FragDepth = depth;\n"
        "}\n";

/// Default vertex shader for rendering trackball representation.
const string camerapath_vertex_code = "\n"
        "#version 430\n"
        "in vec4 in_Position;\n"
        "out vec4 ex_Color;\n"
        "out float depth;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform vec4 in_Color;\n"
        "uniform float nearPlane;\n"
        "uniform float farPlane;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = (viewMatrix * modelMatrix) * in_Position;\n"
        "   depth = (farPlane+nearPlane)/(farPlane-nearPlane) + ( (2*nearPlane*farPlane)/(farPlane-nearPlane) ) * (1/gl_Position[2]);\n"
        "   depth = (depth+1.0)/2.0;\n"
        "   gl_Position = projectionMatrix * gl_Position;\n"
        "   ex_Color = in_Color;\n"
        "}\n";


/**
 * @brief Camera path class, defines control points and a cubic Bezier approximation
 * for defining a smooth camera path from key frames.
 * Reference: https://www.particleincell.com/2012/bezier-splines/
 **/
class CameraPath: public Tucano::Camera {


private:

    /// Movement speed
    float anim_speed;

	/// Animation time
	float anim_time;

	/// Start/stop animation
	bool animating;

	/// Camera position at each Key frames
	vector< Eigen::Vector4f > key_positions;

	/// First control point between two key positions
	vector< Eigen::Vector4f > control_points_1;

	/// Second control point between two key positions
	vector< Eigen::Vector4f > control_points_2;

	/// Rotation state at each key frame
	vector< Eigen::Quaternion<float> > key_quaternions;

	/// Mesh with key positions and computed control points for drawing
	/// smooth curve between key positions
	Mesh curve;

	/// A sphere to visually represent the curves key positions
	Mesh sphere;

	/// Phong shader for drawing spheres
	Effects::Phong *phong;
    
    /// Path shader, used for rendering the curve
    Shader* camerapath_shader;

	

public:

    /**
     * @brief Resets the path 
     */
    void reset (void)
    {
		key_positions.clear();
		key_quaternions.clear();
		control_points_1.clear();
		control_points_2.clear();
    }

    ///Default destructor.
    ~CameraPath() 
	{}

    /**
     * @brief Initializes the view and projection matrices.
     * They are all initialized as Identity matrices, but view is then translated by the defaultTranslation vector.
     */
    void initOpenGLMatrices (void)
    {
        // reset all matrices
        reset();
    }

    /**
     * @brief Default constructor.
     */
    CameraPath ()
    {
        anim_speed = 0.0015;
		anim_time = 0.0;
		animating = true;

        initOpenGLMatrices();
        camerapath_shader = new Shader("../effects/shaders/", "beziercurve");
		camerapath_shader->initialize();

		///@TODO implement sphere by hand and put it in utils
		MeshImporter::loadPlyFile(&sphere, "../samples/models/sphere.ply");
		sphere.normalizeModelMatrix();

		phong = new Effects::Phong();
		phong->setShadersDir("../effects/shaders/");
		phong->initialize();

/*		Eigen::Vector3f pt;
		pt << -10.0, -10.0, 0.0; 
		addKeyPosition(pt);
		pt << -10.0, 10.0, 0.0;
		addKeyPosition( pt );
		pt << 10.0, 10.0, 0.0;
		addKeyPosition( pt );
		pt << 10.0, -10.0, 0.0;             
		addKeyPosition( pt );
		pt << -10.0, -10.0, 0.0;             
		addKeyPosition( pt );
 */
        //camerapath_shader->initializeFromStrings(camerapath_vertex_code, camerapath_fragment_code);
    }

	/**
	* @brief Fill the vertices array
	*/
	void fillVertexData (void)
	{
		computeInnerControlPoints(); 

		curve.reset();
		curve.loadVertices(key_positions);
		curve.createAttribute("in_ControlPoint1", control_points_1);
		curve.createAttribute("in_ControlPoint2", control_points_2);
		
        Misc::errorCheckFunc(__FILE__, __LINE__);
//		curve.createAttribute("p0", key_positionss);

	}

	/**
	* @brief Add key position
	* @param pt New key point
	*/
	void addKeyPosition (Tucano::Camera* camera)
	{
		Eigen::Vector4f center; 
		center << camera->getCenter(), 1.0;
		key_positions.push_back ( center );

		Eigen::Quaternionf quat (camera->getViewMatrix().rotation() );
		key_quaternions.push_back (quat.inverse());

		if (key_positions.size() > 1)
			fillVertexData();
	}

	/**
	* @brief Renders smooth path
	* End points for each Beziér is passed as line_strip
	* and control points as vertex attributes
	* @param camera Current camera for viewing scene
	*/
    void render (Tucano::Camera *camera, Tucano::Camera *light)
    {
		glEnable(GL_DEPTH_TEST);
		if (key_positions.size() > 1)
		{

        	camerapath_shader->bind();
        
        	camerapath_shader->setUniform("viewMatrix", camera->getViewMatrix());
        	camerapath_shader->setUniform("projectionMatrix", camera->getProjectionMatrix());
        	camerapath_shader->setUniform("nearPlane", camera->getNearPlane());
        	camerapath_shader->setUniform("farPlane", camera->getFarPlane());

        	Eigen::Vector4f color (1.0, 0.0, 0.0, 1.0);
        	camerapath_shader->setUniform("modelMatrix", Eigen::Affine3f::Identity());
        	camerapath_shader->setUniform("in_Color", color);

			curve.setAttributeLocation(camerapath_shader);
			curve.bindBuffers();
        	glDrawArrays(GL_LINE_STRIP, 0, key_positions.size());
			curve.unbindBuffers();

        	camerapath_shader->unbind();
		}

		// render key positions
		Eigen::Vector4f color (0.95, 0.38, 0.38, 1.0);
		phong->setDefaultColor( color );
		for (int i = 0; i < key_positions.size(); i++)
		{
			sphere.resetModelMatrix();
			
			Eigen::Vector3f translation = key_positions[i].head(3);
			sphere.modelMatrixPtr()->translate( translation );
			sphere.modelMatrixPtr()->scale( 0.02 );
			phong->render(&sphere, camera, light);
		}
		

        Misc::errorCheckFunc(__FILE__, __LINE__);
    }

	/**
	* @brief Renders the camera represetation on path at given time step
	* @param t Time on curve to place camera
	* @param camera Given current view camera
	* @param light Light camera
	*/
	void renderCameraOnPath ( float t, Tucano::Camera* camera, Tucano::Camera* light )
	{
		if (key_positions.size() > 1)
		{
			// render camera in path
			Eigen::Affine3f m = cameraAtStep(t);
			m.scale(0.07);
			sphere.setModelMatrix(m);

			Eigen::Vector4f color (0.45, 1.0, 0.5, 1.0);
			phong->setDefaultColor (color);
			phong->render(&sphere, camera, light);
		}
		
	}

	/**
	* @brief returns the segment given a time t in [0,1] for the whole curve 
	* @return curve segment number
	*/
	int curveSegment (float t)
	{
		if (t < 0 || t > 1.0)
			return 0;

		return int (t * (key_positions.size()-1));
	}

	/**
	* @brief converts global t to a local t inside a curve segment
	* @return local parameter t in [0,1] for a single Cubic Beziér segment
	*/
	float toLocalStep (float t)
	{
		int segment = curveSegment(t);
		float segment_length = 1.0 / (float)(key_positions.size()-1);

		return (t - segment*segment_length)/segment_length;	
	}

	/**
	* @brief compute point on path from t in [0,1]
	* note that t regards the whole curve, so t=0 is the first key position
	* and t=1 is the last key point
	* 
	* t is first converted into a curve piece (a single Cubic Beziér)
	* and a local t on this piece
	* @param t global t in [0,1]
	*/
	Eigen::Vector4f pointOnPath (float global_t)
	{
		float t = toLocalStep (global_t);
		int segment = curveSegment (global_t);

		// compute point inside curve piece
		Eigen::Vector4f pt;
		pt = pow(1-t,3)*key_positions[segment] + 3.0*pow(1-t,2)*t*control_points_1[segment] + 3.0*(1-t)*pow(t,2)*control_points_2[segment] + pow(t, 3)*key_positions[segment+1];

		return pt;	
	}	
	

	/**
	* @brief return a view matrix at a given path position
	* @return view matrix at time t of the path
	*/
	Eigen::Affine3f cameraAtStep (float global_t)
	{
		Eigen::Affine3f m = Eigen::Affine3f::Identity();
		if (global_t < 0 || global_t > 1)		
			return m;

		if (key_positions.size() < 2)
			return m;
	
		float t = toLocalStep (global_t);
		int segment = curveSegment (global_t);

		// compute point inside curve piece
		Eigen::Vector4f pt;
		pt = pow(1-t,3)*key_positions[segment] + 3.0*pow(1-t,2)*t*control_points_1[segment] + 3.0*(1-t)*pow(t,2)*control_points_2[segment] + pow(t, 3)*key_positions[segment+1];

		Eigen::Quaternionf qt = key_quaternions[segment].slerp(t, key_quaternions[segment+1]);	

		m.rotate(qt);
		m.translation() = pt.head(3);

		return m;
	}

	/**
	* @brief Return the view matrix at current animation step
	* @return View matrix at current animation step
	*/
	Eigen::Affine3f cameraAtCurrentStep (void)
	{
		return cameraAtStep(anim_time);
	}

	/**
	* @brief Move animation forward one step
	*/
	void stepAnimation ( void )
	{
		anim_time += anim_speed;
		if (anim_time >= 1.0)
			anim_time = anim_time - 1.0;
	}

	/**
	* @brief Start animation following camera path
	*/
	void startAnimation ( void )
	{
		animating = true;
	}	

	/**
	* @brief Stop animation
	*/
	void stopAnimation ( void )
	{
		animating = false;
	}

	/**
	* @brief Resets animation to first key position
	*/
	void resetAnimation ( void )
	{
		anim_time = 0.0;
	}

	/**
	* @brief Compute inner control points from key positions
	* For each pair of subsequent key positions, compute two control points
	* to define a Beziér Spline.
	* Since we are restricting the splines to join with same position, and first
	* two derivatives, the result is a smooth curve passing through all control points
	* See reference at class info for more details.
	*/
	void computeInnerControlPoints ( void )
	{

		control_points_1.clear();
		control_points_2.clear();
	
		if (key_positions.size() == 2)
		{
			Eigen::Vector4f pt = (key_positions[0] + key_positions[1])*0.5;	
			control_points_1.push_back(pt);
			control_points_1.push_back(pt);
			control_points_2.push_back(pt);
			control_points_2.push_back(pt);
			return;
		}

		const int n = key_positions.size()-1;
		Eigen::MatrixXf A(n,n);
		A = Eigen::MatrixXf::Zero(n,n);
		Eigen::MatrixXf x = Eigen::MatrixXf::Zero(n,3);
		Eigen::MatrixXf b = Eigen::MatrixXf::Zero(n,3);

		// build the weight matrix, it is the same for all coordinates
		A(0, 0) = 2.0;
		A(0, 1) = 1.0;
		for (int i = 1; i < n-1; i++)
		{
			A(i, i-1) = 1.0;
			A(i, i) = 4.0;
			A(i, i+1) = 1.0;
		}

		A(n-1, n-2) = 2.0;
		A(n-1, n-1) = 7.0;

		// solve the system three times, one for each coordinate (x, y, and z)
		b(0, 0) = key_positions[0][0] + 2.0 * key_positions[1][0];	
		b(0, 1) = key_positions[0][1] + 2.0 * key_positions[1][1];
		b(0, 2) = key_positions[0][2] + 2.0 * key_positions[1][2];

		for (int i = 1; i < n-1; i++)
		{
			b(i, 0) = 4.0*key_positions[i][0] + 2.0*key_positions[i+1][0];
			b(i, 1) = 4.0*key_positions[i][1] + 2.0*key_positions[i+1][1];
			b(i, 2) = 4.0*key_positions[i][2] + 2.0*key_positions[i+1][2];

		}
		b(n-1, 0) = 8.0*key_positions[n-1][0] + key_positions[n][0];
		b(n-1, 1) = 8.0*key_positions[n-1][1] + key_positions[n][1];
		b(n-1, 2) = 8.0*key_positions[n-1][2] + key_positions[n][2];
	
		x.col(0) = A.colPivHouseholderQr().solve(b.col(0));
		x.col(1) = A.colPivHouseholderQr().solve(b.col(1));
		x.col(2) = A.colPivHouseholderQr().solve(b.col(2));

		// now populate the vectors with the first control points
		Eigen::Vector4f pt;
		for (int i = 0; i < n; ++i)
		{
			pt << x.row(i).transpose(), 1.0;
			control_points_1.push_back (pt); 	
		}
		control_points_1.push_back (pt);

		// control point 2 can be determined directly now
		for (int i = 0; i < n-1; ++i)
		{
			pt = 2.0* key_positions[i+1] - control_points_1[i+1];
			control_points_2.push_back (pt);
		}
		pt = (key_positions[n] + control_points_1[n-1])*0.5;
		control_points_2.push_back (pt);
		control_points_2.push_back (pt);
	}

};

}
#endif
