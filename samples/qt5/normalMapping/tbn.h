
#ifndef __TBN_H__
#define __TBN_H__

#include <vector>
#include <Eigen/Dense>

class TBN
{
public:

	static void computeTangentBasis(
		// inputs
		std::vector<Eigen::Vector3f> & vertices,
		std::vector<Eigen::Vector2f> & uvs,
		std::vector<Eigen::Vector3f> & normals,
		// outputs
		std::vector<Eigen::Vector3f> & tangents,
		std::vector<Eigen::Vector3f> & bitangents
		){

		for (unsigned int i = 0; i < vertices.size(); i += 3){

			// Shortcuts for vertices
			Eigen::Vector3f & v0 = vertices[i + 0];
			Eigen::Vector3f & v1 = vertices[i + 1];
			Eigen::Vector3f & v2 = vertices[i + 2];

			// Shortcuts for UVs
			Eigen::Vector2f & uv0 = uvs[i + 0];
			Eigen::Vector2f & uv1 = uvs[i + 1];
			Eigen::Vector2f & uv2 = uvs[i + 2];

			// Edges of the triangle : postion delta
			Eigen::Vector3f deltaPos1 = v1 - v0;
			Eigen::Vector3f deltaPos2 = v2 - v0;

			// UV delta
			Eigen::Vector2f deltaUV1 = uv1 - uv0;
			Eigen::Vector2f deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
			Eigen::Vector3f tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y())*r;
			Eigen::Vector3f bitangent = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x())*r;

			// Set the same tangent for all three vertices of the triangle.
			// They will be merged later, in vboindexer.cpp
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);

			// Same thing for binormals
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);

		}

		// See "Going Further"
		for (unsigned int i = 0; i < vertices.size(); i += 1)
		{
			Eigen::Vector3f & n = normals[i];
			Eigen::Vector3f & t = tangents[i];
			Eigen::Vector3f & b = bitangents[i];

			// Gram-Schmidt orthogonalize
			//t = glm::normalize(t - n * glm::dot(n, t));
			t = (t - n * n.dot(t)).normalized();

			// Calculate handedness
			//if (glm::dot(glm::cross(n, t), b) < 0.0f)
			Eigen::Vector3f c = n.cross(t);
			if (c.dot(b) < 0.0f)
			{
				t = t * -1.0f;
			}

		}
	}


	static void indexVBO(
		std::vector<Eigen::Vector3f> & in_vertices,
		std::vector<Eigen::Vector2f> & in_uvs,
		std::vector<Eigen::Vector3f> & in_normals,
		std::vector<Eigen::Vector3f> & in_tangents,
		std::vector<Eigen::Vector3f> & in_bitangents,

		std::vector<unsigned short> & out_indices,
		std::vector<Eigen::Vector3f> & out_vertices,
		std::vector<Eigen::Vector2f> & out_uvs,
		std::vector<Eigen::Vector3f> & out_normals,
		std::vector<Eigen::Vector3f> & out_tangents,
		std::vector<Eigen::Vector3f> & out_bitangents
		){
		// For each input vertex
		for (unsigned int i = 0; i < in_vertices.size(); i++){

			// Try to find a similar vertex in out_XXXX
			unsigned short index;
			bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

			if (found){ // A similar vertex is already in the VBO, use it instead !
				out_indices.push_back(index);

				// Average the tangents and the bitangents
				out_tangents[index] += in_tangents[i];
				out_bitangents[index] += in_bitangents[i];
			}
			else{ // If not, it needs to be added in the output data.
				out_vertices.push_back(in_vertices[i]);
				out_uvs.push_back(in_uvs[i]);
				out_normals.push_back(in_normals[i]);
				out_tangents.push_back(in_tangents[i]);
				out_bitangents.push_back(in_bitangents[i]);
				out_indices.push_back((unsigned short)out_vertices.size() - 1);
			}
		}
	}

	// Returns true iif v1 can be considered equal to v2
	static bool is_near(float v1, float v2)
	{
		return fabs(v1 - v2) < 0.01f;
	}

	// Searches through all already-exported vertices
	// for a similar one.
	// Similar = same position + same UVs + same normal
	static bool getSimilarVertexIndex(
		Eigen::Vector3f & in_vertex,
		Eigen::Vector2f & in_uv,
		Eigen::Vector3f & in_normal,
		std::vector<Eigen::Vector3f> & out_vertices,
		std::vector<Eigen::Vector2f> & out_uvs,
		std::vector<Eigen::Vector3f> & out_normals,
		unsigned short & result
		)
	{

		// Lame linear search
		for (unsigned int i = 0; i < out_vertices.size(); i++){
			if (
				is_near(in_vertex.x(), out_vertices[i].x()) &&
				is_near(in_vertex.y(), out_vertices[i].y()) &&
				is_near(in_vertex.z(), out_vertices[i].z()) &&
				is_near(in_uv.x(), out_uvs[i].x()) &&
				is_near(in_uv.y(), out_uvs[i].y()) &&
				is_near(in_normal.x(), out_normals[i].x()) &&
				is_near(in_normal.y(), out_normals[i].y()) &&
				is_near(in_normal.z(), out_normals[i].z())
				){
				result = i;
				return true;
			}
		}
		// No other vertex could be used instead.
		// Looks like we'll have to add it to the VBO.
		return false;
	}
};
#endif	// __TBN_H__