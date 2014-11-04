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

#ifndef __CAMERA__
#define __CAMERA__
#define PI 3.14159265358979323846

#include "shader.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Tucano
{

const string camera_fragment_code = "\n"
        "#version 430\n"
        "in vec4 ex_Color;\n"
        "out vec4 out_Color;\n"
        "in float depth;\n"
        "void main(void)\n"
        "{\n"
        "    out_Color = ex_Color;\n"
        "    gl_FragDepth = depth;\n"
        "}\n";

const string camera_vertex_code = "\n"
        "#version 430\n"
        "layout(location=0) in vec4 in_Position;\n"
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
 * @brief Defines an abstract camera with a projection and view matrices.
 *
 * It also contains a visual representation of the camera for rendering purposes.
 **/
class Camera {

protected:

    /// Projection, or intrinsic, matrix.
    Eigen::Matrix4f projectionMatrix;

    /// View, or extrinsic, matrix.
    Eigen::Affine3f viewMatrix;

    /// Viewport dimensions.
    Eigen::Vector4f viewport;

    /// Near plane used for projection matrix.
    float near_plane;

    /// Far plane used for projection matrix.
    float far_plane;

    /// Frustum left dimension.
    float frustum_left;

    /// Frustum right dimension.
    float frustum_right;

    /// Frustum top dimension.
    float frustum_top;

    /// Frustum bottom dimension.
    float frustum_bottom;

    /// Aspect ratio for projection matrix.
    float aspect_ratio;

    /// Camera's Focal Length.
    float focal_length;

    /// Field of view angle in y axis.
    float fovy;

    /// The vertices for the visual representation of the camera.
    Eigen::Vector4f vertices;

    /// Buffer Objects for camera's representation:
    GLuint* bufferIDs;

    /// Flag to use default shaders hard-coded as consts, or use given shaders from files
    bool use_default_shaders;

    /// Shader to render camera representation
    Shader* cameraShader;

    /// Flag to indicate if using a perspective or othograpic projection.
    bool use_perspective;

public:

    /**
     * @brief Reset view matrix
     */
    void resetViewMatrix(void)
    {
        viewMatrix = Eigen::Affine3f::Identity();
    }

    /**
     * @brief Reset projection matrix
     */
    void resetProjectionMatrix(void)
    {
        projectionMatrix = Eigen::Matrix4f::Identity();
    }

    /**
     * @brief Resets trackball to initial position and orientation
     */
    void reset(void)
    {
        resetViewMatrix();
        resetProjectionMatrix();
    }


    /**
     * @brief Returns the center of the camera in world space.
     * @return Camera center
     */
    Eigen::Vector3f getCenter()
    {
        return viewMatrix.linear().inverse() * (-viewMatrix.translation());
    }

    /**
     * @brief Return the modelview matrix as a GLdouble array.
     * Similar to OpenGL old method using glGet**(GL_MODELVIEW_MATRIX)
     * @param matrix A pointer to a GLdouble of at least 16 elements to fill with view matrix.
     */
    void getViewMatrix(GLdouble *matrix)
    {
        Eigen::Matrix4f mv = viewMatrix.matrix();
        for (int i = 0; i < 16; ++i)
        {
            matrix[i] = mv(i);
        }
    }

    /**
     * @brief Return the projection matrix as a GLdouble array.
     * Similar to OpenGL old method using glGet**(GL_PROJECTION_MATRIX)
     * @param matrix A pointer to a GLdouble of at least 16 elements to fill with projection matrix.
     */
    void getProjectionMatrix(GLdouble *matrix)
    {
        for (int i = 0; i < 16; ++i)
        {
            matrix[i] = projectionMatrix(i);
        }
    }

    /**
     * @brief Returns screen space coordinates of a 3D point.
     * Projects a given point to screen space using given viewport coordinates.
     * @param pt Given point to be projected.
     * @param viewport Vector containing viewport coordinates in following order [minX, minY, maxX, maxY]
     */
    inline Eigen::Vector3f projectPoint (const Eigen::Vector4f& pt, Eigen::Vector4f& viewport)
    {
        Eigen::Vector3f screen_pt = Eigen::Vector3f::Zero();
        Eigen::Vector4f proj = viewMatrix * projectionMatrix * pt;
        if (proj[3] == 0.0)
            return screen_pt;

        proj /= proj[3];

        screen_pt[0] = viewport[0] + viewport[2] * (proj[0] * 0.5 + 0.5);
        screen_pt[1] = viewport[1] + viewport[3] * (proj[1] * 0.5 + 0.5);
        screen_pt[2] = proj[2]*0.5 + 0.5;
        return screen_pt;
    }


    /**
     * @brief Returns the view matrix as an Affine 3x3 matrix
     * @return View Matrix.
     */
    Eigen::Affine3f getViewMatrix (void)
    {
        return viewMatrix;
    }

    /**
     * @brief Returns the view matrix as an 4x4 matrix
     * @return Projection Matrix.
     */
    Eigen::Matrix4f getProjectionMatrix (void)
    {
        return projectionMatrix;
    }

    /**
     * @brief Returns a 3x3 matrix containing only the rotation of the view matrix.
     * @todo re-implement this method without quaternion.
     * @return The rotation part of the view matrix as a 3x3 matrix.
     */
    Eigen::Matrix3f getRotationMatrix (void)
    {
        return viewMatrix.rotation();
    }


    /**
     * @brief Returns the translation part of the view matrix as a vector.
     * @return The translation part of the view matrix.
     */
    Eigen::Vector3f getTranslationMatrix (void)
    {
        return viewMatrix.translation();
    }    

    /**
     * @brief Returns the perspective scale.
     * Usually this is element(1,1) of the projection matrix.
     * @return The perspective scale factor
     */
    float getPerspectiveScale (void)
    {
        return (float)1.0f/tan((fovy/2.0f)*(PI/180.0f));
    }

    /**
     * @brief Returns the viewport coordinates.
     * Viewport vector is as follows [minX, minY, maxX, maxY]
     * @return Viewport coordinates.
     */
    Eigen::Vector4f getViewport (void)
    {
        return viewport;
    }

    /**
     * @brief Sets the viewport coordinates.
     * @param vp Viewport coordinates.
     */
    void setViewport (const Eigen::Vector4f &vp)
    {
        viewport = vp;
    }

    /**
     * @brief Sets the viewport coordinates considering that the minimum coordinates are zero.
     * Sets the new viewport as [0, 0, vp[0], vp[1]]
     * @param vp Viewport coordinates.
     */
    void setViewport (const Eigen::Vector2f &vp)
    {
        viewport = Eigen::Vector4f(0.0, 0.0, vp[0], vp[1]);
    }

    /**
     * @brief Sets the projection matrix from a given 4x4 matrix.
     * @param mat Given 4x4 matrix to set as projection matrix.
     */
    void setProjectionMatrix(const Eigen::Matrix4f& mat)
    {
        projectionMatrix = mat;
    }

    /**
     * @brief Returns near plane value.
     * @return Near plane.
     */
    float getNearPlane (void)
    {
        return near_plane;
    }

    /**
     * @brief Returns far plane value.
     * @return Far plane.
     */
    float getFarPlane (void)
    {
        return far_plane;
    }

    /**
     * @brief Loads Trackball Shader file.
     **/
    void loadShader (void)
    {
        if (use_default_shaders)
            cameraShader->initializeFromStrings(camera_vertex_code, camera_fragment_code);
        else
            cameraShader->initialize();
    }

    /**
     * @brief Default destructor.
     */
    ~Camera()
    {
        //Delete camera shader.
        if(cameraShader) {
            delete cameraShader;
        }
    }

    /**
      * @brief Returns current field of view angle in y axis.
      * @return Field of view angle.
      */
    float getFovy (void)
    {
        return fovy;
    }

public:

    /**
     * @brief Default constructor
     * @param shader_dir Optional directory containing camera shaders, otherwise uses default shaders
     */
    Camera(string shader_dir = "")
    {

        // creates the mesh that will be used to represent the trackball's sphere.
        createCameraRepresentation();

        // initialize the shader used for trackball rendering:
        if (shader_dir.empty())
        {
            cameraShader = new Shader("trackballShader");
            use_default_shaders = true;
        }
        else
        {
            cameraShader = new Shader(shader_dir, "trackballShader");
            use_default_shaders = false;
        }

        // initialize buffers used for trackball rendering:
        bufferIDs = new GLuint[3];

        frustum_left = -1.0;
        frustum_right = 1.0;
        frustum_bottom = -1.0;
        frustum_top = -1.0;
        near_plane = 0.1;
        far_plane = 100.0;

        loadShader();
        reset();
    }


    /**
     * @brief Returns a perspective projection matrix with the given parameters
     * @param fy Vertical field of view angle
     * @param in_aspect_ratio Ratio of width to the height of the viewport
     * @param in_near_plane Near plane
     * @param in_far_plane Far plane
     * @return The created perspective matrix.
     */
    static Eigen::Matrix4f createPerspectiveMatrix(float fy, float in_aspect_ratio, float in_near_plane,float in_far_plane)
    {
        Eigen::Matrix4f out = Eigen::Matrix4f::Zero();

        const float
                y_scale = (float)1.0/tan((fy/2.0)*(PI/180.0)),
                x_scale = y_scale / in_aspect_ratio,
                frustum_length = in_far_plane - in_near_plane;

        out(0,0) = x_scale;
        out(1,1) = y_scale;
        out(2,2) = -((in_far_plane + in_near_plane) / frustum_length);
        out(3,2) = -1.0;
        out(2,3) = -((2 * in_near_plane * in_far_plane) / frustum_length);

        return out;
    }

    /**
     * @brief Sets the projection matrix as a perspective matrix.
     *
     * Creates a perspective matrix with the given parameters and sets as the projection matrix.
     * @param fy Vertical field of view angle
     * @param in_aspect_ratio Ratio of width to the height of the viewport
     * @param in_near_plane Near plane
     * @param in_far_plane Far plane
     * @return The created perspective matrix.
     */
    Eigen::Matrix4f setPerspectiveMatrix(float fy, float in_aspect_ratio, float in_near_plane,float in_far_plane)
    {
        Eigen::Matrix4f proj = createPerspectiveMatrix(fy, in_aspect_ratio, in_near_plane, in_far_plane);
        setProjectionMatrix(proj);
        use_perspective = true;
        return proj;
    }

    /**
     * @brief Changes the fovy and computes new perspective projection matrix.
     * @param new_fovy New value for field of view.
     */
    void changeFovy(float new_fovy)
    {
        fovy = new_fovy;
        if (use_perspective)
        {
            setPerspectiveMatrix(fovy, aspect_ratio, near_plane, far_plane);
        }
    }

    /**
     * @brief Returns an orthographic projection matrix with the given parameters
     * @param left Left plane for orthographic view.
     * @param right Right plane for orthographic view.
     * @param bottom Bottom plane for orthographic view.
     * @param top Top lane for orthographic view.
     * @param near Near plane for orthographic view.
     * @param far Far plane for orthographic view.
     * @return The created orthographic matrix.
     */
    static Eigen::Matrix4f createOrthographicMatrix(float left, float right, float bottom, float top, float near, float far)
    {
        Eigen::Matrix4f out = Eigen::Matrix4f::Zero();

        out(0,0) = 2.0/(right-left);
        out(1,1) = 2.0/(top-bottom);
        out(2,2) = -2.0/(far-near);
        out(3,3) = 1.0;
        out(0,3) = -(right+left)/(right-left);
        out(1,3) = -(top+bottom)/(top-bottom);
        out(2,3) = -(far+near)/(far-near);

        return out;
    }

    /**
     * @brief Sets the projection matrix as a orthographic matrix.
     * Creates an orthographic projection matrix with the given parameters and sets as the projection matrix.
     * @param left Left plane for orthographic view.
     * @param right Right plane for orthographic view.
     * @param bottom Bottom plane for orthographic view.
     * @param top Top lane for orthographic view.
     * @param near Near plane for orthographic view.
     * @param far Far plane for orthographic view.
     * @return The created orthographic matrix.
     */
    Eigen::Matrix4f setOrthographicMatrix(float left, float right, float bottom, float top, float near, float far)
    {
        Eigen::Matrix4f proj = createOrthographicMatrix(left, right, bottom, top, near, far);
        setProjectionMatrix(proj);
        use_perspective = false;
        return proj;
    }

    /**
     * @brief Increases the fov of the perspective matrix by a given increment.
     * @param inc Given increment to increase fov
     */
    void incrementFov(float inc)
    {
        fovy += inc;
        if (use_perspective)
        {
            setPerspectiveMatrix(fovy, aspect_ratio, near_plane, far_plane);
        }
    }

    /**
     * @brief Translates the view matrix by a given vector.
     * @param translation Translation to apply to view matrix.
     */
    void translate(const Eigen::Vector3f& translation)
    {
        viewMatrix.translate(translation);
    }

    /**
     * @brief Rotate the view matrix by a given quaternion.
     * @param rotation Rotation to apply to view matrix.
     */
    void rotate(const Eigen::Quaternion<float>& rotation)
    {
        viewMatrix.rotate(rotation);
    }

    /**
     * @brief Scales the view matrix by given factors.
     * @param scale_factors Scale factors in x, y, and z axis.
     */
    void scale(const Eigen::Vector3f& scale_factors)
    {
        viewMatrix.scale(scale_factors);
    }

    /**
     * @brief Scales the view matrix by given factor in all axis.
     * @param scale_factor Scale factors in all axis.
     */
    void scale(float scale_factor)
    {
        viewMatrix.scale(scale_factor);
    }

    /**
     * @brief Creates a visual representation of the camera.
     * @todo implement this method! Only buffers are set.
     * Represents the camera as a truncated pyramid (frustum) and its coordinate axes.
     */
    void createCameraRepresentation (void)
    {
//        glGenVertexArrays(1, &bufferIDs[0]);
//        glGenBuffers(2, &bufferIDs[1]);
//        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /**
     * @brief Binds buffers for rendering camera's representation.
     */
    void bindBuffers (void)
    {
        //VAO:
        glBindVertexArray(bufferIDs[0]);

        //VBO:
        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
    }

    /**
     * @brief Unbinds render buffers.
     */
    void unbindBuffers (void)
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDisableVertexAttribArray(0);
    }

    /**
     * @brief Renders the camera representation.
     * Renders a truncated pyramid (frustum) to represent the camera.
     * It will be placed and oriented according to the view matrix.
     */
    void render (void)
    {
        cameraShader->bind();

        cameraShader->setUniform("viewMatrix", viewMatrix.data(), 4, GL_FALSE, 1);
        cameraShader->setUniform("projectionMatrix", projectionMatrix.data(), 4 ,GL_FALSE, 1);
        cameraShader->setUniform("nearPlane", near_plane);
        cameraShader->setUniform("farPlane", far_plane);

        bindBuffers();

        //X:
        Eigen::Vector4f colorVector(1.0, 0.0, 0.0, 1.0);
        cameraShader->setUniform("modelMatrix",(Eigen::Affine3f::Identity()*Eigen::AngleAxis<float>(PI/2.0,Eigen::Vector3f(0.0,1.0,0.0))).data(), 4, GL_FALSE, 1);
        cameraShader->setUniform("in_Color",colorVector.data(),4,1);
        glDrawArrays(GL_LINE_LOOP, 0, 200);
        Misc::errorCheckFunc(__FILE__, __LINE__);

        unbindBuffers();

        cameraShader->unbind();
    }

};

}
#endif
