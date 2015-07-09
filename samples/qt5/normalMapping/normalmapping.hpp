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

#ifndef __NORMAL_MAPPING__
#define __NORMAL_MAPPING__

#include <tucano.hpp>
#include <effect.hpp>
#include <mesh.hpp>
#include <camera.hpp>
#include <texture.hpp>

using namespace Tucano;

namespace Effects
{

/**
 * @brief Renders a mesh using a Normal Mapping shader.
 */
class NormalMapping : public Effect
{

private:

    /// Normal Mapping Shader
    Shader normal_mapping_shader;

	/// Default color
	Eigen::Vector4f default_color;

	/// Texture to hold diffuse texture
	Tucano::Texture* diffuse_map;

	/// Texture to hold normal texture
	Tucano::Texture* normal_map;

	/// Texture to hold specular texture
	Tucano::Texture* specular_map;

	float lightIntensity;
public:

    /**
     * @brief Default constructor.
     */
	NormalMapping(void)
    {
		default_color << 0.12, 0.1, 0.1, 1.0;
		lightIntensity = 2.0f;
    }

    /**
     * @brief Default destructor
     */
	virtual ~NormalMapping(void) {}

    /**
     * @brief Load and initialize shaders
     */
    virtual void initialize (void)
    {
		initGL();
        // searches in default shader directory (/shaders) for shader files normalMappingShader.(vert,frag,geom,comp)
        loadShader(normal_mapping_shader, "normalmapping") ;
    }

	/**
	* @brief Sets the three textures used in the effect
	*/
	void setTextures(Tucano::Texture& diffuseMap,
					Tucano::Texture& normalMap,
					Tucano::Texture& specularMap)
	{
		diffuse_map = &diffuseMap;
		normal_map = &normalMap;
		specular_map = &specularMap;
	}



	void bind(Tucano::Mesh& mesh,
		const Tucano::Camera& camera,
		const Tucano::Camera& lightTrackball)
	{
		Eigen::Vector4f viewport = camera.getViewport();
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		normal_mapping_shader.bind();

		normal_mapping_shader.setUniform("in_Texture_Diffuse", diffuse_map->bind());
		normal_mapping_shader.setUniform("in_Texture_Normal", normal_map->bind());
		normal_mapping_shader.setUniform("in_Texture_Specular", specular_map->bind());


		normal_mapping_shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
		//normal_mapping_shader.setUniform("modelMatrix", mesh.getModelMatrix());
		normal_mapping_shader.setUniform("viewMatrix", camera.getViewMatrix());
		normal_mapping_shader.setUniform("lightViewMatrix", lightTrackball.getViewMatrix());
		//normal_mapping_shader.setUniform("lightPositionWorldSpace", lightTrackball.getTranslationMatrix());
		Eigen::Vector3f lightPos = lightTrackball.getTranslationMatrix();  //Eigen::Vector3f(0, 0, 4);
		normal_mapping_shader.setUniform("lightPositionWorldSpace", lightPos.x(), lightPos.y(), -lightPos.z());
		normal_mapping_shader.setUniform("lightIntensity", lightIntensity);


		Eigen::Matrix4f modelMatrix = Eigen::Matrix4f::Identity();
		modelMatrix.col(3).head<3>() << 0.0f, -1.0f, 0.0f;
		Eigen::Matrix4f modelViewMatrix = camera.getViewMatrix() * modelMatrix;
		Eigen::Matrix3f modelView3x3Matrix; // = Eigen::Matrix3f::Identity();	// modelViewMatrix 3x3
		modelView3x3Matrix.col(0).head<3>() = modelViewMatrix.col(0).head<3>();
		modelView3x3Matrix.col(1).head<3>() = modelViewMatrix.col(1).head<3>();
		modelView3x3Matrix.col(2).head<3>() = modelViewMatrix.col(2).head<3>();

		normal_mapping_shader.setUniform("modelMatrix", modelMatrix);
		normal_mapping_shader.setUniform("MV3x3", modelView3x3Matrix);
	}

	void unbind()
	{
		normal_mapping_shader.unbind();

		diffuse_map->unbind();
		normal_map->unbind();
		specular_map->unbind();
	}

	void increaseLightIntensity() { lightIntensity+=0.5f; };
	void decreaseLightIntensity() { lightIntensity-=0.5f; };

};

}


#endif	// __NORMAL_MAPPING__
