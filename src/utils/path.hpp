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

#ifndef __PATH__
#define __PATH__

#include "camera.hpp"
#include "mesh.hpp"
#include "misc.hpp"
#include "shapes/sphere.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{


/**
 * @brief Path class, defines control points and a cubic Bezier approximation
 * for defining a smooth path from key frames.
 *
 * Very useful for creating camera paths for example, but it is generic
 * enough to be used for other purposes. Each key frame stores a position
 * and an orientation.
 *
 * Reference: https://www.particleincell.com/2012/bezier-splines/
 *
 * Notation:
 * We refer to time as the curve parameter (analogy to particle moving along path)
 * We refer to curve as the whole path, and a segment as one Cubic Bezíér that composes path
 **/
class Path: public Tucano::Camera {

private:

    /// Movement speed, arc length increment for each animation step
    float anim_speed;

	/// Animation time
	float anim_time;

	/// Start/stop animation
	bool animating;

	/// Flag to render control points
	bool draw_control_points;

	/// Flag to render quaternions at key positions
	bool draw_quaternions;

	/// Camera position at each Key frames
	vector< Eigen::Vector4f > key_positions;

	/// First control point between two key positions
	vector< Eigen::Vector4f > control_points_1;

	/// Second control point between two key positions
	vector< Eigen::Vector4f > control_points_2;

	/// Arc-length approximation splitting each Beziér segment into linear segments 
	vector< vector <float> > arc_lengths;

	/// Total path length
	float path_length;

	/// Rotation state at each key frame
	vector< Eigen::Quaternion<float> > key_quaternions;

    /// Control quaternions for SQUAD
    vector < Eigen::Quaternion<float> > control_quaternions;

	/// Mesh with path's key positions and computed control points for drawing
	/// smooth curve between key positions
	Mesh curve;

	// Mesh containing segments from key points to control points, for visual debug
	Mesh control_segments;

	/// A sphere to visually represent the path's key positions
	Shapes::Sphere sphere;

	/// Coordinate axes for visualizing quaternions at key positions
	Shapes::CoordinateAxes axes;

    /// Path shader, used for rendering the curve
    Shader camerapath_shader;

	/// To render simple lines
	Shader phong_shader;
	
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
		arc_lengths.clear();
		path_length = 0.0;
    }

    ///Default destructor.
    ~Path() 
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
    Path ()
    {
        anim_speed = 0.015;
		anim_time = 0.0;
		animating = true;
		draw_control_points = false;
		draw_quaternions = false;

        initOpenGLMatrices();

        #ifdef TUCANOSHADERDIR
        initialize(stringify(TUCANOSHADERDIR));
        #endif
    }

    /**
     * @brief Initializes shaders
     * @param shader_dir Location of shader files
     */
    void initialize (string shader_dir = "../effects/shaders/")
    {
        camerapath_shader.load("beziercurve", shader_dir);
		camerapath_shader.initialize();

		phong_shader.load("phongshader", shader_dir);
		phong_shader.initialize();
    }

	/**
	* @brief Fill the vertices array
	*/
	void fillVertexData (void)
	{
		computeInnerControlPoints(); 
		computeArcLength();
        computeControlQuaternions();

		curve.reset();
		curve.loadVertices(key_positions);
		curve.createAttribute("in_ControlPoint1", control_points_1);
		curve.createAttribute("in_ControlPoint2", control_points_2);

		control_segments.reset();

		vector < Eigen::Vector4f > control_segs;
		for (unsigned int i = 0; i < key_positions.size()-1; ++i)
		{
			control_segs.push_back(key_positions[i]);
			control_segs.push_back(control_points_1[i]);
			control_segs.push_back(key_positions[i+1]);
			control_segs.push_back(control_points_2[i]);		
		}
		control_segments.loadVertices(control_segs);
		
		#ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
		#endif

	}

	/**
	* @brief Add key position
	* @param camera Camera containing position and orientation in view matrix
	*/
	void addKeyPosition (const Tucano::Camera& camera)
	{
		Eigen::Vector4f center; 
		center << camera.getCenter(), 1.0;
		key_positions.push_back ( center );

		Eigen::Quaternionf quat (camera.getViewMatrix().rotation() );
/*		Eigen::Matrix3f m;
		Eigen::Vector3f rx, ry, rz;
		rz << center.head(3);
		rz.normalize();
		ry << 0.0, 1.0, 0.0;
		rx = ry.cross(rz);
		m.row(0) = rx;
		m.row(1) = ry;
		m.row(2) = rz;
		Eigen::Quaternionf quat(m);
*/

		key_quaternions.push_back (quat.inverse());

		if (key_positions.size() > 1)
			fillVertexData();
	}

	/**
	* @brief Renders smooth path
	* End points for each Beziér is passed as line_strip
	* and control points as vertex attributes
	* Inside geometry shader each Beziér segment is approximate by linear segments
	* @param camera Current camera for viewing scene
	* @param light Camera representing light (position and orientation)
	*/
    void render (const Tucano::Camera& camera, const Tucano::Camera& light)
    {
		glEnable(GL_DEPTH_TEST);
		if (key_positions.size() > 1)
		{

        	camerapath_shader.bind();
        
        	camerapath_shader.setUniform("viewMatrix", camera.getViewMatrix());
        	camerapath_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
        	camerapath_shader.setUniform("nearPlane", camera.getNearPlane());
        	camerapath_shader.setUniform("farPlane", camera.getFarPlane());

        	Eigen::Vector4f color (1.0, 0.0, 0.0, 1.0);
        	camerapath_shader.setUniform("modelMatrix", Eigen::Affine3f::Identity());
        	camerapath_shader.setUniform("in_Color", color);

			curve.setAttributeLocation(camerapath_shader);
			curve.bindBuffers();
        	glDrawArrays(GL_LINE_STRIP, 0, key_positions.size());
			curve.unbindBuffers();

        	camerapath_shader.unbind();

			if (draw_control_points)
			{
				phong_shader.bind();
			
				phong_shader.setUniform("viewMatrix", camera.getViewMatrix());
				phong_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
				phong_shader.setUniform("lightViewMatrix", light.getViewMatrix());

				color << 1.0, 1.0, 0.0, 1.0;
				phong_shader.setUniform("modelMatrix", Eigen::Affine3f::Identity());
				phong_shader.setUniform("default_color", color);
				phong_shader.setUniform("has_color", false);
				control_segments.setAttributeLocation(phong_shader);
				control_segments.bindBuffers();
				glDrawArrays(GL_LINES, 0, control_points_1.size()*4);
				control_segments.unbindBuffers();
				phong_shader.unbind();

				sphere.setColor( Eigen::Vector4f (0.48, 1.0, 0.16, 1.0) );
				// render control points
				for (unsigned int i = 0; i < control_points_1.size(); i++)
				{
					sphere.resetModelMatrix();
					Eigen::Vector3f translation = control_points_1[i].head(3);
					sphere.modelMatrix()->translate( translation );
					sphere.modelMatrix()->scale( 0.03 );
					sphere.render(camera, light);
				}

				sphere.setColor( Eigen::Vector4f (0.48, 0.16, 1.0, 1.0) );
				// render control points
				for (unsigned int i = 0; i < control_points_2.size(); i++)
				{
					sphere.resetModelMatrix();
					Eigen::Vector3f translation = control_points_2[i].head(3);
					sphere.modelMatrix()->translate( translation );
					sphere.modelMatrix()->scale( 0.03 );
					sphere.render(camera, light);
				}

			}

		}

		if (draw_quaternions)
		{
			for (unsigned int i = 0; i < key_quaternions.size(); ++i)
			{
				axes.resetModelMatrix();
				axes.modelMatrix()->rotate(key_quaternions[i]);
				axes.modelMatrix()->scale(0.2);
				axes.render(camera, light);	
			}
		}

		sphere.setColor( Eigen::Vector4f (1.0, 0.48, 0.16, 1.0) );
		// render key positions
		for (unsigned int i = 0; i < key_positions.size(); i++)
		{
			sphere.resetModelMatrix();
			
			Eigen::Vector3f translation = key_positions[i].head(3);
			sphere.modelMatrix()->translate( translation );
			sphere.modelMatrix()->scale( 0.03 );
			sphere.render(camera, light);
		}


        glDisable(GL_DEPTH_TEST);
		
		#ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
		#endif
    }

	/**
	* @brief returns the segment given a time t in [0,1] for the whole path 
	* @return curve segment number
	*/
	int curveSegment (float t)
	{
		if (t < 0 || t > 1.0)
			return 0;

		return int (t * (key_positions.size()-1));
	}

	/**
	* @brief converts global parameter t to a local t inside a segment
	* @return Local parameter t in [0,1] for a single Cubic Beziér segment
	*/
	float toLocalParameter (float t)
	{
		int segment = curveSegment(t);
		float segment_length = 1.0 / (float)(key_positions.size()-1);

		return (t - segment*segment_length)/segment_length;	
	}

	/**
	* @brief Returns a point at time t on a given Beziér segment
	* @param t Beziér segment parameter
	* @param segment Beziér segment on path
	* @return Point on curve
	*/
	Eigen::Vector4f pointOnSegment (float t, int segment)
	{
		Eigen::Vector4f pt;
		pt = pow(1-t,3)*key_positions[segment] + 3.0*pow(1-t,2)*t*control_points_1[segment] + 3.0*(1-t)*pow(t,2)*control_points_2[segment] + pow(t, 3)*key_positions[segment+1];
		return pt;	
	}

	/**
	* @brief compute point on path given t in [0,1]
	*
	* Note that t refers to the whole path, so t=0 is the first key position
	* and t=1 is the last key position
	* @param global_t Global parameter on path in [0,1]
	*/
	Eigen::Vector4f pointOnPath (float global_t)
	{
		float t = toLocalParameter (global_t);
		int segment = curveSegment (global_t);
		return pointOnSegment(t, segment);	
	}	
	
	/**
	* @brief Returns the log of a quaternion log(q)
	* q in the form [cos(a), sin(a) v], log(q) = [0, a v]
	* @param qi Given quaternion
	* @return log(q)
	*/
	Eigen::Quaternionf logQuaternion (const Eigen::Quaternionf& qi)
	{
		Eigen::Quaternionf logq, q;    
        q = qi;

        // change sign to avoid flipping behavior during interpolation
        if (q.w() < 0)
            q.coeffs() *= -1.0;

		logq.w() = 0.0;
		float angle = acos(q.w());
		float sinangle = sin(angle);
		if ( fabs(sinangle) >= 1e-03)
			logq.vec() = angle * q.vec() / sinangle;
		else 
			logq.vec() = q.vec();

//		logq.w() = log ( q.norm() );
//		logq.vec() = q.vec().normalized() * acos ( q.w() / q.norm());

		return logq;
	}

	/**
	* @brief Returns the  exp of a quaternion exp(q)
	* q in the form [0, a v], exp(q) = [cos(a), sin(a) v] with |v|=1
	* @param q Given quaternion
	* @return exp(q)
	*/
	Eigen::Quaternionf expQuaternion (const Eigen::Quaternionf& q)
	{
		Eigen::Quaternionf expq;

        float angle = q.norm();
		expq.w() = cos(angle);
		float sinangle = sin(angle);
		if (fabs(sinangle) >= 1e-03)
			expq.vec() = sin(angle) * q.vec() / angle;
		else
			expq.vec() = q.vec();

//		expq.w() = exp(q.w()) * cos( q.vec().norm() );
//		expq.vec() = exp(q.w()) * q.vec().normalized() * sin( q.vec().norm() );

		return expq;
	}

	/**
	* @brief SQUAD - Spherical and Quadrangle quaternion interpolation
	* @param seg The current Bezièr segment
	* @param t Interpolatio time
	*/
	Eigen::Quaternionf squad (int seg, float t)
	{
		Eigen::Quaternionf s0 = control_quaternions[seg];
		Eigen::Quaternionf s1 = control_quaternions[seg+1];
        Eigen::Quaternionf q0 = key_quaternions[seg];
        Eigen::Quaternionf q1 = key_quaternions[seg+1];
        Eigen::Quaternionf p0, p1, res;
        float dot = 0.0;

        // before each slerp check if we are going the right direction (shortest path)
        // otherwise multiply one of the quaternions by -1
        // Note: this might not be necessary any more, since we are checking during log method

		dot = q0.w()*q1.w()+q0.vec().dot(q1.vec());
		if (dot < 0)
		{
            q1.coeffs() *= -1.0;
		}	
		p0 = q0.slerp(t, q1); 

		dot = s0.w()*s1.w()+s0.vec().dot(s1.vec());
		if (dot < 0)
		{
            s1.coeffs() *= -1.0;
		}	
		p1 = s0.slerp(t, s1);

        p0.normalize();
        p1.normalize();

        dot = p0.w()*p1.w()+p0.vec().dot(p1.vec());
        if (dot < 0)
        {
            p1.coeffs() *= -1.0;
        }
		res = p0.slerp(2.0*t*(1.0-t), p1);
        res.normalize();

		return res;
	}

	
	/**
	* @brief return a view matrix at a given path parameter 
	* @return view matrix at time t of the path
	*/
	Eigen::Affine3f cameraAtTime (float global_t)
	{
		Eigen::Affine3f m = Eigen::Affine3f::Identity();
		
		float t_a = arcLengthToTime(global_t);

		if (t_a == -1.0)
			return m;

		float t = toLocalParameter (t_a);
		int segment = curveSegment (t_a);

		Eigen::Vector4f pt = pointOnSegment(t, segment);

		Eigen::Quaternionf qt;
		qt = squad(segment, t);

		m.rotate(qt);
		m.translation() = pt.head(3);

		return m;
	}

	void printDebugInfo (void)
	{
		float global_t = anim_time;
		float t = toLocalParameter (global_t);
		int segment = curveSegment (global_t);

		Eigen::Vector4f pt = pointOnSegment(t, segment);

		Eigen::Quaternionf qt;
		if (segment >= 0 && key_positions.size() - segment > 2)
			qt = squad(segment, t);
		else		
			qt = key_quaternions[segment].slerp(t, key_quaternions[segment+1]);	

		Eigen::Affine3f m = Eigen::Affine3f::Identity();
		m.rotate(qt);
		m.translation() = pt.head(3);

		cout << "quat : (" << qt.w() << " , " << qt.vec().transpose() << " ) " << endl;
		cout << "t : " << pt.head(3).transpose() << endl;
		cout << "m : " << endl << m.matrix() << endl << endl;
	}

	/**
	* @brief Return the view matrix at current animation time 
	* @return View matrix at current animation time
	*/
	Eigen::Affine3f cameraAtCurrentTime (void)
	{
		return cameraAtTime(anim_time);
	}

	/**
	* @brief Converts from given arc length to time parameter
	* @param s Arc length parameter in [0,1]
	* @return Time at given arc length 
	*/
	float arcLengthToTime (float s)
	{
		if (key_positions.size() < 2)
			return -1.0;
	
		if (s < 0.0)
			return 0;

		if (s > path_length)		
			return path_length;

		float arc_length = s;

		// find in which Beziér segment we should look
		unsigned int segment = 0;
		for (; segment < key_positions.size()-1; ++segment)
		{
			if (arc_lengths[segment+1][0] > arc_length)
				break;
		}	

		// look inside Beziér segment in which linear approximation we should look for 
		unsigned int sub_seg = 0;
		for (; sub_seg < arc_lengths[segment].size()-1; ++sub_seg)
		{
			if (arc_lengths[segment][sub_seg+1] > arc_length)
				break;
		}

		// convert from arc length (s) to time parameter (t)
		// percentage inside sub segment
		float alpha = (arc_length - arc_lengths[segment][sub_seg]) / 
					(arc_lengths[segment][sub_seg+1] - arc_lengths[segment][sub_seg]);

		// find parameter [0,1] inside Beziér segment
		float t_ini = sub_seg / (float)(arc_lengths[segment].size()-1);
		float t_end = (sub_seg+1) / (float)(arc_lengths[segment].size()-1);
		float t_s = t_ini + alpha * (t_end - t_ini);

		// convert to global parameter of the path
		t_s = t_s / (float)(key_positions.size()-1);

		// add time to beginning of segment
		t_s += segment / (float)(key_positions.size()-1);	

		return t_s;
	}

	/**
	* @brief Toggle animation
	*/
	void toggleAnimation (void)
	{
		animating = !animating;
	}

	/**
	* @brief Toggle render control points
	*/
	void toggleDrawControlPoints (void)
	{
		draw_control_points = !draw_control_points;
	}

	/**
	* @brief Toggle render control quaternions 
	*/
	void toggleDrawQuaternions (void)
	{
		draw_quaternions = !draw_quaternions;
	}


	/**
	* @brief Returns if Animation is running or not
	* @return True if animation is running, false otherwise
	*/
	bool isAnimating (void)
	{
		return animating;
	}

	/**
	* @brief Move animation forward one step
	*/
	void stepForward ( void )
	{
		anim_time += anim_speed;

		// restart if necessary
		if (anim_time >= path_length)
			anim_time = anim_time - path_length;
	}

	/**
	* @brief Move animation backward one step
	*/
	void stepBackward ( void )
	{
		anim_time -= anim_speed;

		// restart if necessary
		if (anim_time < 0.0)
			anim_time = anim_time + path_length;
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
	* @brief Return current animation time
	* @return anim time	
	*/
	float animTime ( void )
	{
		return anim_time;
	}

	/**
	* @brief Return animation speed where 1 covers the whole curve in one step
	* @return Animation speed
	*/
	float animSpeed ( void )
	{
		return anim_speed;
	}
	
	/**
	* @brief Sets the animation speed
	* @param as Given animation speed
	*/
	void setAnimSpeed (float as)
	{
		anim_speed = as;
	}

    /**
    * @brief Compute control points for SQUAD
    *
    * Quaternions, Interpolation and Animation, Erik B. Dam, Martin Koch, Martin Lillholm
    */
    void computeControlQuaternions ( void )
    {
        control_quaternions.clear();

		Eigen::Quaternionf s;
        
        // s for first control point is not defined, so we just set as q0
        s = key_quaternions[0];
		control_quaternions.push_back(s);

        for (unsigned int seg = 1; seg < key_quaternions.size()-1; ++seg)
        {
			s = -0.25*(logQuaternion(key_quaternions[seg].inverse()*key_quaternions[seg-1]).coeffs() + logQuaternion(key_quaternions[seg].inverse()*key_quaternions[seg+1]).coeffs());
			s = key_quaternions[seg]* expQuaternion(s);

            control_quaternions.push_back(s);
        }

		// control point for last quaternion not defined since we dont have qi+1
		control_quaternions.push_back(key_quaternions.back());

    }


	/**
	* @brief Compute inner control points from key positions
	*
	* For each pair of subsequent key positions, compute two control points
	* to define a Beziér Spline.
	* Since we are restricting the splines to join with same position, and first
	* two derivatives, the result is a smooth curve passing through all control points
	* See reference at class info for more details.
	*
	* To solve the tridiagonal system we use Thomas Algorithm (see Wikipedia)
	*/
	void computeInnerControlPoints ( void )
	{

		control_points_1.clear();
		control_points_2.clear();
	
		// if there are only two keypoints it is a linear segment
		// just distribute control points equally along segment
		if (key_positions.size() == 2)
		{
			Eigen::Vector4f pt = 0.75*key_positions[0] + 0.25*key_positions[1];	
			control_points_1.push_back(pt);
			control_points_1.push_back(pt);
			pt << 0.25*key_positions[0] + 0.75*key_positions[1];
			control_points_2.push_back(pt);
			control_points_2.push_back(pt);
			return;
		}

		const int n = key_positions.size()-1;

		control_points_1.resize(n+1);
		control_points_2.resize(n+1);

		// in theory this should be a nxn matrix, but since it is a tridiagonal matrix
		// we are just storing the non-zero elements as a, b, and c
		vector <float> a_orig;
		vector <float> b_orig;
		vector <float> c_orig;
		vector <float> a;
		vector <float> b;
		vector <float> c;
		vector <float> d;
		a_orig.resize(n);
		b_orig.resize(n);
		c_orig.resize(n);
		a.resize(n);
		b.resize(n);
		c.resize(n);	
		d.resize(n);

		// build the weight matrix, it is the same for all lines except first and last
		a_orig[0] = 0.0;
		b_orig[0] = 2.0;
		c_orig[0] = 1.0;
		
		for (int i = 1; i < n-1; i++)
		{
			a_orig[i] = 1.0;
			b_orig[i] = 4.0;
			c_orig[i] = 1.0;
		}

		a_orig[n-1] = 2.0;
		b_orig[n-1] = 7.0;
		c_orig[n-1] = 0.0;

		// solve using Thomas Algorithm for tridiagonal matrices
		// solve the system three times, one for each coordinate (x, y, and z)
		// we copy the coefficients each time to avoid recomputing them
		for (int coord = 0; coord < 3; ++coord)
		{
			// build solution vector of linear system
			d[0] = key_positions[0][coord] + 2.0 * key_positions[1][coord];	
			for (int i = 1; i < n-1; i++)
				d[i] = 4.0*key_positions[i][coord] + 2.0*key_positions[i+1][coord];
			d[n-1] = 8.0*key_positions[n-1][coord] + key_positions[n][coord];
	
			// copy coefficients, we are going to reuse them for every coordinate
			a = a_orig;
			b = b_orig;
			c = c_orig;

			// forward sweep
			c[0] = c[0]/b[0];
			d[0] = d[0]/b[0];

			for (int i = 1; i < n; ++i)
			{
				float m = b[i] - a[i]*c[i-1];
				c[i] = c[i] / m;
				d[i] = (d[i] - a[i]*d[i-1]) / m;
			}

			// back substitution
			control_points_1[n-1][coord] = d[n-1];
			for (int i = n-2; i >= 0; --i)
			{
				control_points_1[i][coord] = d[i] - c[i]*control_points_1[i+1][coord];
			}
			// add one last control point to facilitate future operations (same size as key vector)
			control_points_1[n][coord] = control_points_1[n-1][coord];
		}

		// set w = 1 for all control points
		for (int i = 0; i <= n; ++i)
		{
			control_points_1[i][3] = 1.0;
		}

		// control point 2 can be determined directly now
		Eigen::Vector4f pt;
		for (int i = 0; i < n-1; ++i)
		{
			pt = 2.0* key_positions[i+1] - control_points_1[i+1];
			control_points_2[i] = pt;
		}
		pt = (key_positions[n] + control_points_1[n-1])*0.5;
		control_points_2[n-1] = pt;
		control_points_2[n] = pt;
	}

	/**
	* @brief Compute an approximation of the arclength
	*
	* approximates the curve by linear segments
	*/
	void computeArcLength (void)
	{
		float divs = 100.0;
		Eigen::Vector4f p0;
		Eigen::Vector4f p1;
		float dist = 0.0;
		path_length = 0.0;

		arc_lengths.clear();

		p0 = key_positions[0];
		for (unsigned int seg = 0; seg < key_positions.size()-1; ++seg)
		{
			vector <float> seg_lengths;
			for (int i = 0; i < divs; ++i)
			{
				p1 = pointOnSegment( (float)i/divs, seg);
				dist += (p1-p0).norm();
				seg_lengths.push_back(dist);
				p0 = p1;
			}
			// to make life easier when converting from t to s,
			// we also repeat the last sub segment length at the end
			// even though it will be covered by the first sub segment
			// of the next Beziér segment
			// note that we do not increment dist, since it will be done
			// in the next iteration
			p1 = key_positions[seg+1];
			seg_lengths.push_back( dist + (p1-p0).norm() );
	
			arc_lengths.push_back (seg_lengths);
		}	
		// the last segment connecting to the final key position
		// there is no segment starting at this key position, so it
		// must be treated apart
		p1 = key_positions[key_positions.size()-1];
		dist += (p1-p0).norm();
		
		path_length = dist;

		// insert a last vector with only the total distance
		// makes life easier when converting from arc length to time parameter
		vector <float> last_seg;
		last_seg.push_back (dist);
		arc_lengths.push_back(last_seg);

	}

};

}
#endif
