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

#ifndef __MESH__
#define __MESH__

#include <model.hpp>
#include <shader.hpp>

using namespace std;

namespace Tucano
{

/**
 * @brief A vertex attribute of a mesh.
 *
 * Some common attributes (coordinate, color, normal, texCoord) have custom loaders.
 * Otherwise, a generic attribute can be loaded.
 */
class VertexAttribute
{
private:
    /// Attribute's name
    string name;
    /// size of attribute array (usually number of vertices)
    int size;
    /// number of elements per attribute
    int element_size;
    /// this is set by the shader using the attribute
    GLint location;
    /// Vertex array Buffer ID
    GLuint bufferID;
    /// Type of attribute element (ex. GL_FLOAT)
    GLenum type;
    /// Type of attribute array (GL_ARRAY_BUFFER for most cases), indices are GL_ELEMENT_ARRAY_BUFFER
    GLenum array_type;

public:

    VertexAttribute() : name(""), size(0), element_size(0), location(-1), bufferID(0), type(GL_FLOAT), array_type(GL_ARRAY_BUFFER){}

    VertexAttribute(string in_name, int in_num_elements, int in_element_size, GLenum in_type, GLenum in_array_type = GL_ARRAY_BUFFER) :
        name(in_name), size(in_num_elements), element_size(in_element_size), type(in_type), array_type (in_array_type)
    {
        location = -1;

        // create new buffer for attribute if it is not yet set (ex copy constructor)
        glGenBuffers(1, &bufferID);
    }

    /// copy constructor
    VertexAttribute(const VertexAttribute& copy)
    {
        this->name = copy.name;
        this->size = copy.size;
        this->element_size = copy.element_size;
        this->location = copy.location;
        this->bufferID = copy.bufferID;
        this->type = copy.type;
        this->array_type = copy.array_type;
    }

    /// Destructor. Note that deleting the buffer here, will delete a buffer of a original attribute if by chance you copy
    /// the instance and then delete it. So it is better to explicitily destroy it
    ~VertexAttribute() {}

    void destroy(void)
    {
        if (bufferID != 0)
            glDeleteBuffers(1, &bufferID);
    }

    /**
     * @brief Returns the name of the attribute
     * @return Attribute's name
     */
    string getName (void) {return name;}

    /**
     * @brief Returns the number of elements of this attribute (usually number of vertices in VAO)
     * @return Number of elements
     */
    int getSize (void) {return size;}

    /**
     * @brief Returns the number of elements per attribute
     * For example, returns 3 if attribute is of type vec3
     * @return Size of one attribute
     */
    int getElementSize (void) {return element_size;}

    /**
     * @brief Returns the type of the attribute (ex. GL_FLOAT)
     * @return Type of attribute
     */
    GLenum getType (void) {return type;}

    /**
     * @brief Returns the type of array (ex. GL_ARRAY_BUFFER)
     * @return Type of array
     */
    GLenum getArrayType (void) {return array_type;}

    /**
     * @brief Sets the type of array (default is GL_ARRAY_BUFFER)
     * @param at Array type
     */
    void setArrayType (GLenum at) {array_type = at;}

    /**
     * @brief Returns the location of the attribute.
     * The location is usually set by the shader, since it can be used by different
     * shaders in different situations, and we don't want to force the shader layout id to be
     * known beforehand
     * @return Attribute shader location
     */
    GLint getLocation(void) {return location;}

    /**
     * @brief Sets the location of the attribute for a shader.
     * @param loc Attribute shader location
     */
    void setLocation(GLint loc) {location = loc;}

    /**
     * @brief Returns the id of the vertex array of this attribute
     * @return Buffer ID
     */
    GLuint getBufferID (void) {return bufferID;}

    /// Bind the attribute
    void bind(void)
    {
        glBindBuffer(array_type, bufferID);
    }

    /// Bind the attribute to a given location
    void enable(GLint loc)
    {
        setLocation(loc);
        glBindBuffer(array_type, bufferID);
        glVertexAttribPointer(location, element_size, type, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(location);
    }

    /// Binds the attribute to its location
    void enable()
    {
        if (location != -1)
        {
            glBindBuffer(array_type, bufferID);
            glVertexAttribPointer(location, element_size, type, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(location);
        }
    }


    /// Unbind the attribute
    void unbind(void)
    {
        glBindBuffer(array_type, 0);
    }

    /// Disables associated location if attribute has one
    void disable(void)
    {
        if (location != -1)
        {
            glDisableVertexAttribArray(location);
        }
    }


};

/**
 * @brief A common Mesh, usually containing triagles or points.
 *
 * The Mesh class handles the buffers generation and usage. such as VBO. IBO. TBO... It also contains methods for simple meshes generation. (e.g. a quadrilateral or a cube) and a wavefront .obj
 * loader. When a mesh is generated. geometrical computations are performed. detecting the center of the mesh through the axis-aligned bounding box and computing normalizating scale factors.
 * The attribute locations are predefined in this class. being the location 0 used for Vertex Buffer. 1 for Normals Buffer. 2 for Color Buffer and 3 for TexCoord Buffer.
 **/
class Mesh : public Model {

public:

    /**
   * @brief Default Constructor.
   */
    Mesh (void)
    {

        numberOfVertices = 0;
        numberOfNormals = 0;
        numberOfElements = 0;
        numberOfTexCoords = 0;

        radius = 1.0;
        scale = 1.0;

        vao_id = 0;
        index_buffer_id = 0;
    }

    /**
     * @brief Returns the number of elements (primitives such as triangles) of the mesh.
     * @return Number of primitives.
     */
    int getNumberOfElements (void)
    {
        return numberOfElements;
    }

    /**
     * @brief Returns the number of vertices in the mesh.
     * @return Number of vertices.
     */
    int getNumberOfVertices (void)
    {
        return numberOfVertices;
    }

    /**
     * @brief Resets all vertex attributes locations to -1.
     */
    void resetLocations (void)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].setLocation(-1);
        }
    }


    ///Creates a quad. Useful for offscreen rendering.
    //void createQuad(void);

    ///Deletes all the buffers and VAO.
    void deleteBuffers() {

        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].destroy();
        }
        vertex_attributes.clear();
        if (vao_id > 0)
            glDeleteVertexArrays(1, &vao_id);
        if (index_buffer_id > 0)
            glDeleteBuffers(1, &index_buffer_id);

    }

    /**
    * @brief Reset the whole mesh, including deleting buffers and cleaning arrays
    */
    void reset (void)
    {
        deleteBuffers();

        numberOfVertices = 0;
        numberOfNormals = 0;
        numberOfElements = 0;
        numberOfTexCoords = 0;

        radius = 1.0;
        scale = 1.0;
    }

    ///Default Destructor. Deletes the bufferIDs. vertices. normals. colors. indices and texCoords arrays.
    virtual ~Mesh() {
        reset();
    }

protected:

    ///Array of generic attributes
    vector < VertexAttribute > vertex_attributes;

    ///Number of vertices in vertices array.
    unsigned int numberOfVertices;

    ///Number of normals in normals array.
    unsigned int numberOfNormals;

    ///Number of indices in indices array
    unsigned int numberOfElements;

    ///Number of texture coordinates in texCoords array.
    unsigned int numberOfTexCoords;

    ///Number of colors in colors array.
    unsigned int numberOfColors;

    /// Index Buffer
    GLuint index_buffer_id;

    /// Vertex Array Object ID (VAO is just a descriptor, does not contain any data)
    GLuint vao_id;

    ///Function used for openGL error handling.
    //void errorCheckFunc(string file, int line);

public:

    /**
     * @brief Load vertices (x,y,z,w) and creates appropriate vertex attribute.
     * The default attribute name is "in_Position".
     * Computes bounding box and centroid and normalization factors (scale).
     * @param vert Array of vertices.
     */
    void loadVertices (vector<Eigen::Vector4f> &vert)
    {

        numberOfVertices = vert.size();

        // creates new attribute and load vertex coordinates
        createAttribute("in_Position", vert);

        // from now on we are just computing some information about the model such as bounding box, centroid ...

        float xMax = 0; float xMin = 0; float yMax = 0; float yMin = 0; float zMax = 0; float zMin = 0;

        int temp = 0;
        for(unsigned int i = 0; i < numberOfVertices*4; i+=4) {

            //X:
            if(vert[temp][0] > xMax) {
                xMax = vert[temp][0];
            }
            if(vert[temp][0] < xMin) {
                xMin = vert[temp][0];
            }

            //Y:
            if(vert[temp][1] > yMax) {
                yMax = vert[temp][1];
            }
            if(vert[temp][1] < yMin) {
                yMin = vert[temp][1];
            }

            //Z:
            if(vert[temp][2] > zMax) {
                zMax = vert[temp][2];
            }
            if(vert[temp][2] < zMin) {
                zMin = vert[temp][2];
            }

            //W:
            temp++;
        }

        scale = 1.0/max(max(xMax-xMin, yMax-yMin), zMax-zMin);

        float centerX = (xMax+xMin)/2.0;
        float centerY = (yMax+yMin)/2.0;
        float centerZ = (zMax+zMin)/2.0;
        objectCenter = Eigen::Vector3f(centerX, centerY, centerZ);

        // compute the centroid (different from the box center)
        centroid = Eigen::Vector3f::Zero();
        for(unsigned int i = 0; i < numberOfVertices; i++) {
            centroid = centroid + vert[i].head(3);//Eigen::Vector3f(vert[i][0], vert[i][1], vert[i][2]);
        }
        centroid = centroid / numberOfVertices;

        // compute the radius of the bounding sphere
        // most distance point from the centroid
        radius = 0.0;
        for(unsigned int i = 0; i < numberOfVertices; i++) {
            radius = max(radius, ( vert[i].head(3) - centroid ).norm());
        }

        scale = 1.0/radius;

    }

    /**
     * @brief Load normals (x,y,z) as a vertex attribute.
     * @param norm Normals list.
     */
    void loadNormals (vector<Eigen::Vector3f> &norm)
    {
        numberOfNormals = norm.size();

        createAttribute("in_Normal", norm);
    }

    /**
     * @brief Load tex coords (u,v) as a vertex attribute.
     * Optionally normalizes coords in range [0,1]
     * @param tex Texture coordinates array.
     * @param normalize If true normalizes the texcoords in range [0,1], otherwise does not normalize.
     */
    void loadTexCoords (vector<Eigen::Vector2f> &tex, bool normalize = false)
    {
        numberOfTexCoords = tex.size();

        if (normalize)
        {
            vector<Eigen::Vector2f> tex_normalized;

            float texXmax = tex[0][0];
            float texXmin = tex[0][0];
            float texYmax = tex[0][1];
            float texYmin = tex[0][1];

            // compute min and max values for x and y
            for(unsigned int i = 0; i < numberOfTexCoords; i++)
            {
                if (tex[i][0] < texXmin) texXmin = tex[i][0];
                if (tex[i][0] > texXmax) texXmax = tex[i][0];
                if (tex[i][1] < texYmin) texYmin = tex[i][1];
                if (tex[i][1] > texYmax) texYmax = tex[i][1];

            }
            for(unsigned int i = 0; i < numberOfTexCoords; i++)
            {
                tex_normalized.push_back ( Eigen::Vector2f(
                                               (tex[i][0] - texXmin) / (texXmax - texXmin),
                                           (tex[i][1] - texYmin) / (texYmax - texYmin) ) );
            }
            createAttribute("in_TexCoords", tex_normalized);
        }
        else
        {
            createAttribute("in_TexCoords", tex);
        }
    }


    /**
     * @brief Load colors (r,g,b,a) as a vertex attribute.
     * @param clrs Colors array.
     */
    void loadColors (vector<Eigen::Vector4f> &clrs)
    {
        createAttribute("in_Color", clrs);
    }


    /**
     * @brief Load indices into indices array
     * @param ind Indices array.
     */
    void loadIndices (vector<GLuint> &ind)
    {
        numberOfElements = ind.size();

        GLuint *indices = new GLuint[ind.size()];

        //Indices:
        for(unsigned int i = 0; i < ind.size(); i++) {
            indices[i] = ind[i];
        }

        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size()*sizeof(GLuint), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        delete [] indices;
    }

    /**
     * @brief Sets default attribute locations.
     * vertex coords -> location 0
     * normals -> location 1
     * colors -> location 2
     * texCoords -> location 3
     */
    void setDefaultAttribLocations (void)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!vertex_attributes[i].getName().compare("in_Position"))
            {
                vertex_attributes[i].setLocation(0);
            }
            else if (!vertex_attributes[i].getName().compare("in_Normal"))
            {
                vertex_attributes[i].setLocation(1);
            }
            else if (!vertex_attributes[i].getName().compare("in_Color"))
            {
                vertex_attributes[i].setLocation(2);
            }
            else if (!vertex_attributes[i].getName().compare("in_TexCoord"))
            {
                vertex_attributes[i].setLocation(3);
            }
        }
    }

    /**
     * @brief Returns wether an attribute exists or not.
     * @param name Name of attribute to be queried.
     * @return True if attribute exists, false otherwise.
     */
    bool hasAttribute (string name)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!vertex_attributes[i].getName().compare(name))
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Resets all mesh locations and automatically sets the attribute locations for a given Shader.
     *
     * For every mesh attribute, the Shader will be queried for an attribute with the same name,
     * if one is found they will linked.
     * @param shader A pointer to the given shader.
     */
    void setAttributeLocation (Shader *shader)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            GLint loc = shader->getAttributeLocation(vertex_attributes[i].getName().c_str());
            if (loc != -1)
            {
                vertex_attributes[i].setLocation(loc);
            }
        }
    }


    /**
     * @brief Sets the location of a generic vertex attribute.
     * @param name Name of the given attribute
     * @param loc Location of the attribute.
     */
    void setAttributeLocation (string name, GLint loc)
    {
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            if (!name.compare(vertex_attributes[i].getName()))
            {
                vertex_attributes[i].setLocation(loc);
            }
        }
    }


    /**
     * @brief Creates and loads a new mesh attribute of 4 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute (string name, vector<Eigen::Vector4f> &attrib)
    {
        // create new vertex attribute
        VertexAttribute va (name, attrib.size(), 4, GL_FLOAT);
        vertex_attributes.push_back(va);

        float * attrib_array = new float[va.getSize()*va.getElementSize()];
        int temp = 0;
        for(int i = 0; i < va.getSize()*va.getElementSize(); i+=4) {
            attrib_array[i] = attrib[temp][0];
            attrib_array[i+1] = attrib[temp][1];
            attrib_array[i+2] = attrib[temp][2];
            attrib_array[i+3] = attrib[temp][3];
            temp++;
        }

        // fill buffer with attribute data
        va.bind();
        glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*sizeof(va.getType()), attrib_array, GL_STATIC_DRAW);
        va.unbind();

        delete attrib_array;
        return &vertex_attributes.back();

    }

    /**
     * @brief Creates and loads a new mesh attribute of 3 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute(string name, vector<Eigen::Vector3f> &attrib)
    {
        // create new vertex attribute
        VertexAttribute va (name, attrib.size(), 3, GL_FLOAT);
        vertex_attributes.push_back(va);

        float * attrib_array = new float[va.getSize()*va.getElementSize()];

        int temp = 0;
        for(int i = 0; i < va.getSize()*va.getElementSize(); i+=3) {
            attrib_array[i] = attrib[temp][0];
            attrib_array[i+1] = attrib[temp][1];
            attrib_array[i+2] = attrib[temp][2];
            temp++;
        }

        // fill buffer with attribute data
        va.bind();
        glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*sizeof(va.getType()), attrib_array, GL_STATIC_DRAW);
        va.unbind();

        delete attrib_array;
        return &vertex_attributes.back();
    }

    /**
     * @brief Creates and loads a new mesh attribute of 2 floats.
     * @param name Name of the attribute.
     * @param attrib Array with new attribute.
     * @return Pointer to created attribute
     */
    VertexAttribute* createAttribute(string name, vector<Eigen::Vector2f> &attrib)
    {
        // create new vertex attribute
        VertexAttribute va (name, attrib.size(), 2, GL_FLOAT);
        vertex_attributes.push_back(va);

        float * attrib_array = new float[va.getSize()*va.getElementSize()];

        int temp = 0;
        for(int i = 0; i < va.getSize()*va.getElementSize(); i+=2) {
            attrib_array[i] = attrib[temp][0];
            attrib_array[i+1] = attrib[temp][1];
            temp++;
        }

        // fill buffer with attribute data
        va.bind();
        glBufferData(va.getArrayType(), va.getSize()*va.getElementSize()*sizeof(va.getType()), attrib_array, GL_STATIC_DRAW);
        va.unbind();

        delete attrib_array;
        return &vertex_attributes.back();
    }


    /**
     * @brief Binds all buffers.
     *
     * If no Vertex Array has been generated yet, one will be created.
     * If there is a index buffer it will also be binded.
     * Then, binds one buffer for each vertex attribute.
     */
    virtual void bindBuffers (void)
    {

        if (vao_id == 0)
        {
            glGenVertexArrays(1, &vao_id);
        }

        glBindVertexArray(vao_id); //Vertex Array Object

        if (index_buffer_id > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        }

        // bind generic attributes
        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].enable();
        }

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }

    /**
     * @brief Unbinds all buffers.
     */
    virtual void unbindBuffers (void)
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        for (unsigned int i = 0; i < vertex_attributes.size(); ++i)
        {
            vertex_attributes[i].disable();
        }
    }

    /**
     * @brief Call the draw method for rendering triangles.
     * This method requires that a index buffer has been created.
     */
    virtual void renderElements (void)
    {

        glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, (GLvoid*)0);
        //glDrawArrays(GL_TRIANGLES, 0, numberOfElements);
    }

    /**
     * @brief Render the mesh triangles.
     * The method binds the buffers, calls the method to render triangles, and then unbinds all buffers.
     * Note that a index buffer is necessary.
     */
    virtual void render() {
        bindBuffers();
        renderElements();
        unbindBuffers();
    }


    /**
     * @brief Sets the mesh as a Parallelpiped with given dimensions, scales so larger side is equal to 1.
     * @param x Width
     * @param y Height
     * @param z Depth
     */
    void createParallelepiped(float x, float y, float z)
    {

        numberOfVertices = 32;
        numberOfElements = 36;
        //numberOfColors = 32;

        vector<Eigen::Vector4f> vert;
        vector<GLuint> ind;

        //Normalizing the cube coordinates:
        float scale = x;
        if (y > x) scale = y;
        if (z > scale) scale = z;

        vert.push_back( Eigen::Vector4f (-.5f*x/scale, -.5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (-.5f*x/scale,  .5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale,  .5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale, -.5f*y/scale,  .5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (-.5f*x/scale, -.5f*y/scale, -.5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (-.5f*x/scale,  .5f*y/scale, -.5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale,  .5f*y/scale, -.5f*z/scale, 1) );
        vert.push_back( Eigen::Vector4f (.5f*x/scale, -.5f*y/scale, -.5f*z/scale, 1) );

        GLuint tempIndices[36] = {
            0,2,1,  0,3,2,
            4,3,0,  4,7,3,
            4,1,5,  4,0,1,
            3,6,2,  3,7,6,
            1,6,5,  1,2,6,
            7,5,6,  7,4,5
        };

        for(unsigned int i = 0; i < numberOfElements;i++) {
            ind.push_back( tempIndices[i] );
        }

        /*float colors[32] =
    {
        0, 0, 1, 1,
        1, 0, 0, 1,
        0, 1, 0, 1,
        1, 1, 0, 1,
        1, 1, 1, 1,
        1, 0, 0, 1,
        1, 0, 1, 1,
        0, 0, 1, 1
    };*/

        loadVertices(vert);
        loadIndices(ind);

        setDefaultAttribLocations();
    }

    /**
     * @brief Sets the mesh as Unit Quad.
     */
    void createQuad (void)
    {
        vector<Eigen::Vector4f> vert;
        vector<Eigen::Vector2f> texCoord;
        vector<GLuint> elementsVertices;

        vert.push_back ( Eigen::Vector4f(-1.0, -1.0, 0.0, 1.0) );
        vert.push_back ( Eigen::Vector4f( 1.0, -1.0, 0.0, 1.0) );
        vert.push_back ( Eigen::Vector4f( 1.0,  1.0, 0.0, 1.0) );
        vert.push_back ( Eigen::Vector4f(-1.0,  1.0, 0.0, 1.0) );

        elementsVertices.push_back(0);
        elementsVertices.push_back(1);
        elementsVertices.push_back(2);
        elementsVertices.push_back(2);
        elementsVertices.push_back(3);
        elementsVertices.push_back(0);

        texCoord.push_back ( Eigen::Vector2f(0.0, 0.0) );
        texCoord.push_back ( Eigen::Vector2f(1.0, 0.0) );
        texCoord.push_back ( Eigen::Vector2f(1.0, 1.0) );
        texCoord.push_back ( Eigen::Vector2f(0.0, 1.0) );

        loadVertices(vert);
        loadTexCoords(texCoord);
        loadIndices(elementsVertices);

        setDefaultAttribLocations();

    }
};

}
#endif
