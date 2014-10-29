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

#ifndef __OBJIMPORTER__
#define __OBJIMPORTER__

#include <mesh.hpp>

using namespace std;

namespace Tucano
{

namespace MeshImporter
{

/**
 * @brief Loads a mesh from an OBJ file.
 *
 * @todo check if color really exists or not
 * Loads vertex coordinates and normals, texcoords and color when available.
 * @param mesh Pointer to mesh instance to load file.
 * @param filename Given filename of the OBJ file.
 */
static void loadObjFile (Mesh* mesh, string filename)
{
    vector<Eigen::Vector4f> vert;
    vector<Eigen::Vector3f> norm;
    vector<Eigen::Vector2f> texCoord;
    vector<Eigen::Vector4f> color;
    vector<GLuint> elementsVertices;
    vector<GLuint> elementsNormals;
    vector<GLuint> elementsTexIDs;

    //Opening file:
    cout << "Opening Wavefront obj file " << filename.c_str() << endl << endl;
    ifstream in(filename.c_str(),ios::in);
    if (!in) {
        cerr << "Cannot open " << filename.c_str() << endl; exit(1);
    }

    //Reading file:
    string line;
    while(getline(in,line)) {

        //Vertices reading:
        if(line.substr(0,2) == "v ") {

            istringstream s(line.substr(2));
            Eigen::Vector4f v;
            s >> v[0]; s >> v[1]; s >> v[2]; v[3] = 1.0f;
            vert.push_back(v);

            if(s.rdbuf()->in_avail())
            {
                Eigen::Vector4f c;
                s >> c[0]; s >> c[1]; s >> c[2]; c[3] = 1.0f;
                color.push_back(c);
            }
        }

        //Normals reading:
        else if(line.substr(0,2) == "vn") {
            istringstream s(line.substr(3));
            Eigen::Vector3f vn;
            s >> vn[0]; s >> vn[1]; s >> vn[2];
            norm.push_back(vn);
        }

        //Texture Coordinates reading:
        else if(line.substr(0,2) == "vt") {
            istringstream s(line.substr(2));
            Eigen::Vector2f vt;
            s >> vt[0]; s >> vt[1];
            texCoord.push_back(vt);
        }

        //Elements reading: Elements are given through a string: "f vertexID/TextureID/NormalID". If no texture is given, then the string will be: "vertexID//NormalID".
        else if(line.substr(0,2) == "f ") {
            GLuint vertexID1, normalID1, textureID1;
            istringstream s(line.substr(2));
            while(!s.eof()){
                s >> vertexID1;
                elementsVertices.push_back(vertexID1-1);
                if(s.peek() == '/') {
                    s.get();
                    if(s.peek() == '/') {
                        s.get();
                        s >> normalID1;
                        elementsNormals.push_back(normalID1-1);
                    }
                    else {
                        s >> textureID1;
                        elementsTexIDs.push_back(textureID1-1);
                        if(s.peek() == '/') {
                            s.get();
                            s >> normalID1;
                            elementsNormals.push_back(normalID1-1);
                        }
                    }

                }
            }
        }

        //Ignoring comment lines:
        else if(line[0] == '#') { }

        //Ignoring any other lines:
        else {};
    }
    if(in.is_open()) {
        in.close();
    }

    // load attributes found in file
    if (vert.size() > 0)
        mesh->loadVertices(vert);
    if (norm.size() > 0)
        mesh->loadNormals(norm);
    if (texCoord.size() > 0)
        mesh->loadTexCoords(texCoord);
    if (color.size() > 0)
        mesh->loadColors(color);
    if (elementsVertices.size() > 0)
        mesh->loadIndices(elementsVertices);

    // sets the default locations for accesing attributes in shaders
    mesh->setDefaultAttribLocations();

    //errorCheckFunc(__FILE__, __LINE__);
}

}
}
#endif
