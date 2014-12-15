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

#ifndef __PLYIMPORTER__
#define __PLYIMPORTER__

#include <mesh.hpp>
#include <utils/rply.hpp>

using namespace std;

namespace Tucano
{

namespace MeshImporter
{

static int normal_cb( p_ply_argument argument )
{
    static Eigen::Vector3f v;

    void* data;
    long coord;

    ply_get_argument_user_data( argument, &data, &coord );

    vector<Eigen::Vector3f>* vec = static_cast< vector<Eigen::Vector3f>* >( data );

    switch( coord )
    {
        case 0:
        case 1:
            v[coord] = ply_get_argument_value( argument );
            break;

        case 2:
            v[2] = ply_get_argument_value( argument );
            vec->push_back( v );
            break;
    }

    return 1;
}

static int vertex_cb( p_ply_argument argument )
{
    static Eigen::Vector4f v;

    void* data;
    long coord;

    ply_get_argument_user_data( argument, &data, &coord );

    vector<Eigen::Vector4f>* vec = static_cast< vector<Eigen::Vector4f>* >( data );

    switch( coord )
    {
        case 0:
        case 1:
            v[coord] = ply_get_argument_value( argument );
            break;

        case 2:
            v[2] = ply_get_argument_value( argument );
            v[3] = 1.0;
            vec->push_back( v );
            break;
    }

    return 1;
}


/**
 * @brief Loads a mesh from an PLY file.
 *
 * @param mesh Pointer to mesh instance to load file.
 * @param filename Given filename of the PLY file.
 */
static bool loadPlyFile (Mesh *mesh, string filename)
{
    p_ply ply = ply_open( filename.c_str(), NULL, 0, NULL );
    if( !ply || !ply_read_header( ply ) )
    {
        cerr << "Cannot open " << filename.c_str() << endl;
        return false;
    }

    vector<Eigen::Vector4f> vertices;
    vector<Eigen::Vector3f> norm;
    vector<Eigen::Vector2f> texCoord;
    vector<Eigen::Vector4f> color;
    std::vector<uint> indices;

//	long nvertices, ncolors, nnormals;

//	nvertices = ply_set_read_cb( ply, "vertex", "x", vertex_cb, ( void* )&vert, 0 );
    ply_set_read_cb( ply, "vertex", "x", vertex_cb, ( void* )&vertices, 0 );
    ply_set_read_cb( ply, "vertex", "y", vertex_cb, ( void* )&vertices, 1 );
    ply_set_read_cb( ply, "vertex", "z", vertex_cb, ( void* )&vertices, 2 );

//	ncolors = ply_set_read_cb( ply, "vertex", "red", vertex_cb, ( void* )&color, 0 );
    ply_set_read_cb( ply, "vertex", "red", vertex_cb, ( void* )&color, 0 );
    ply_set_read_cb( ply, "vertex", "green", vertex_cb, ( void* )&color, 1 );
    ply_set_read_cb( ply, "vertex", "blue", vertex_cb, ( void* )&color, 2 );

//	nnormals = ply_set_read_cb( ply, "vertex", "nx", normal_cb, ( void* )&norm, 0 );
    ply_set_read_cb( ply, "vertex", "nx", normal_cb, ( void* )&norm, 0 );
    ply_set_read_cb( ply, "vertex", "ny", normal_cb, ( void* )&norm, 1 );
    ply_set_read_cb( ply, "vertex", "nz", normal_cb, ( void* )&norm, 2 );

    if( !ply_read( ply ) )
        return false;

    ply_close( ply );

    if( color.size() > 0 )
    {
        float max = 1.0f;

        for( unsigned int i = 0; i < color.size(); i++ )
        {
            if( color[i][0] > max )
            {
                max = color[i][0];
                break;
            }
        }

        if( max > 1.0f )
        {
            for( unsigned int i = 0; i < color.size(); i++ )
            {
                color[i][0] = color[i][0] / 256.0f;
                color[i][1] = color[i][1] / 256.0f;
                color[i][2] = color[i][2] / 256.0f;
                color[i][3] = color[i][3];
            }
        }
    }
    else
    {
        color.resize( vertices.size() );

        for( unsigned int i = 0; i < color.size(); i++ )
        {
            color[i][0] = 1.0f;
            color[i][1] = 1.0f;
            color[i][2] = 1.0f;
            color[i][3] = 1.0f;
        }
    }

    for( unsigned int i = 0; i < vertices.size(); ++i )
    {
        indices.push_back(i);
    }


    // load attributes found in file
    if (vertices.size() > 0)
        mesh->loadVertices(vertices);
    if (norm.size() > 0)
        mesh->loadNormals(norm);
    if (texCoord.size() > 0)
        mesh->loadTexCoords(texCoord);
    if (color.size() > 0)
        mesh->loadColors(color);
    if (indices.size() > 0)
        mesh->loadIndices(indices);

    // sets the default locations for accesing attributes in shaders
    mesh->setDefaultAttribLocations();

    //errorCheckFunc( __FILE__, __LINE__ );

    return true;
}

}
}
#endif
