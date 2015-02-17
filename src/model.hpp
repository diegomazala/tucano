#ifndef __MODEL__
#define __MODEL__

#include <utils/misc.hpp>

#include <vector>
#include <fstream>
#include <Eigen/Dense>

using namespace std;

namespace Tucano
{

/**
 * @brief The Model class is a holder for any kind of model, such as meshes, point clouds, surfaces ... that should inherit the model class.
 *
 * It basically contains only information about its spatial location and orientation, that is, a model matrix.
 **/
class Model {

protected:

    /// Model matrix, holds information about the models location and orientation.
    Eigen::Affine3f modelMatrix;

    /// Center of the mesh object.
    Eigen::Vector3f objectCenter;

    /// Object's centroid (different from center of bounding box)
    Eigen::Vector3f centroid;

    /// Radius of the mesh bounding sphere.
    float radius;

    /// The normalization scale factor, scales the model matrix to fit the model inside a unit cube.
    float scale;

public:

    /**
     * Default Constructor.
     */
    Model(void)
    {
        resetModelMatrix();
        objectCenter = Eigen::Vector3f::Zero();
        centroid = Eigen::Vector3f::Zero();
        radius = 1.0;
        scale = 1.0;
    }

    /**
     * @brief Returns the center of the axis-aligned bounding box.
     * @return The center of the axis-aligned bounding box.
     */
    virtual Eigen::Vector3f getObjectCenter (void)
    {
        return objectCenter;
    }

    /**
     * @brief Returns the centroid of the model.
     * @return The centroid of the mesh object. given by the mean position of all vertices.
     */
    virtual Eigen::Vector3f getCentroid (void)
    {
        return centroid;
    }

    /**
     * Returns the radius of the bounding sphere.
     * The bounding sphere is computed as the distance from the farthest point to the object's centroid.
     * @return Radius of the bounding sphere.
     */
    virtual float getBoundingSphereRadius (void)
    {
        return radius;
    }

    /**
     * @brief Returns the model matrix.
     * @return Model matrix as an Affine 3f matrix.
     */
    Eigen::Affine3f getModelMatrix (void)
    {
        return modelMatrix;
    }

    /**
     * @brief Returns a pointer to the model matrix.
     * @return Pointer to the odel matrix as an Affine 3f matrix.
     */
    Eigen::Affine3f* modelMatrixPtr (void)
    {
        return &modelMatrix;
    }

	/**
	* @brief Sets the model matrix
	* @param m Given new model matrix
	*/
	void setModelMatrix (Eigen::Affine3f &m)
	{
		modelMatrix = m;
	}


    /**
     * @brief Returns the scale factor for fitting the model inside a unit cube.
     * @return Scale factor.
     */
    float getScale (void)
    {
        return scale;
    }

    /**
     * @brief Normalize model matrix to center and scale model.
     * The model matrix will include a translation to place model's centroid
     * at the origin, and scale the model to fit inside a unit sphere.
     */
    void normalizeModelMatrix (void)
    {
        modelMatrix.scale(scale);
        modelMatrix.translate(-centroid);
    }


    /**
     * @brief Resets the model matrix.
     */
    void resetModelMatrix (void)
    {
        modelMatrix = Eigen::Affine3f::Identity();
    }

    /**
     * Default Destructor.
     */
    ~Model (void)
    {}

};

}
#endif
