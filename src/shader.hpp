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

#ifndef __TUCANOSHADER__
#define __TUCANOSHADER__

#include "utils/misc.hpp"

#include <fstream>
#include <vector>
#include <Eigen/Dense>

using namespace std;

namespace Tucano
{

/**
 * @brief A Shader object represents one GLSL program.
 *
 * One object can store either the standard rendering pipeline shaders (vertex, geometry, fragment...) or a group of
 * compute shaders. For convenience, it also stores a user defined name, making it easier to access the shaders from within the main program.
 * The shader's name is the same as the shaders filenames, without the extensions.
 */
class Shader {

private:

    /// Stores an user mainteined identification for the shader. If the shader is created with the autoloader, the name is equal to the filename, without extensions.
    string shaderName;

    /// Stores the path to the vertex shader file.
    string vertexShaderPath;

    /// Stores the path to the geometry shader file.
    string geometryShaderPath;

    /// Stores the path to the fragment shader file.
    string fragmentShaderPath;

    /// Stores the paths to the compute shaders files.
    vector<string> computeShaderPaths;

    /// The Projection Matrix uniform location.
    GLuint projectionMatrixUniformLocation;

    /// The View Matrix uniform location.
    GLuint viewMatrixUniformLocation;

    /// The Model Matrix uniform location.
    GLuint modelMatrixUniformLocation;

    /// Mesh's Color uniform location.
    GLuint colorUniformLocation;

    /// Array that contains all the compute shaders identifications.
    vector<GLuint> computeShaders;

    /// Vertex Shader identification.
    GLuint vertexShader;

    /// Geometry Shader identification.
    GLuint geometryShader;

    /// Fragment Shader identification.
    GLuint fragmentShader;

    /// Shader program identification.
    GLuint shaderProgram;

    /// Debug level for outputing warnings and messages
    int debug_level;

public:

    /**
     * @brief Empty constructor.
     *
     * Usually used to initialize the instance when passing shaders as strings, and not files.
     * @param name The string to be set as shader identification. User mantained.
     */
    Shader (string name)
    {
        shaderName = name;
        vertexShader = 0; fragmentShader = 0; geometryShader = 0; shaderProgram = 0; computeShaders = vector<GLuint>();
    }

    /**
     * This constructor receives the pathes to the compute shaders from a vector. It must receive a vector containing at least one compute shader path. The shader name is defined by the user.
     * @param name The string to be set as shader identification. User manteined.
     * @param compute_shader_paths Vector of strings giving the paths to the external files containing the compute shaders.
     */
    Shader (string name, vector<string> compute_shader_paths)
    {
        shaderName = name;
        computeShaderPaths = compute_shader_paths;

        vertexShader = 0; fragmentShader = 0; geometryShader = 0; shaderProgram = 0; computeShaders = vector<GLuint>();
    }

    /**
     * Default destructor.
     */
    ~Shader (void)
    {
        deleteShaders();
    }

    /**
     * @brief Returns a string with the shader name.
     *
     * If the shader was created with the auto-loader, the name is the same as the shader file name. If not, it's user manteined.
     * @return The shader name.
     */
    string getShaderName (void)
    {
        return shaderName;
    }

    /**
     * @brief Returns the program identification handle.
     * @return Identification handle.
     */
    GLuint getShaderProgram (void)
    {
        return shaderProgram;
    }

    /**
     * @brief Returns a handle to the fragment shader.
     * @return The Fragment Shader identification handle.
     */
    GLuint getFragmentShader (void)
    {
        return fragmentShader;
    }

    /**
     * @brief Returns a handle to the vertex shader.
     * @return The Vertex Shader identification handle.
     */
    GLuint getVertexShader (void)
    {
        return vertexShader;
    }

    /**
     * @brief Returns a handle to the geometry shader.
     * @return Return the Geometry Shader identification handle.
     */
    GLuint getGeometryShader (void)
    {
        return geometryShader;
    }


    /**
   * @return Return the vector of Compute Shaders identification handles.
   **/
    vector<GLuint> getComputeShaders() {
        return computeShaders;
    }


public:


    /**
     * @brief Constructor that receives the path for each shader separately.
     *
     * It must receive a vertex and a fragment shader, and optionally can receive a geometry shader.
     * @param name The string to be set as shader identification. User mantained.
     * @param vertex_shader_path String giving the path to the external file containing the vertex shader.
     * @param fragment_shader_path String giving the path to the external file containing the fragment shader.
     * @param geometry_shader_path String giving the path to the external file containing the geometry shader.
     */
    Shader (string name, string vertex_shader_path, string fragment_shader_path, string geometry_shader_path = "")
    {
        shaderName = name;
        vertexShaderPath = vertex_shader_path;
        geometryShaderPath = geometry_shader_path;
        fragmentShaderPath = fragment_shader_path;
        vertexShader = 0;
        fragmentShader = 0;
        geometryShader = 0;
        shaderProgram = 0;
        computeShaders = vector<GLuint>();
    }

    /**
     * @brief Constructors that searches a given directory for shaders with given name.
     *
     * Receives a directory and a shader name, searches for files with the same name and extesions vert, frag, geom and comp to auto load shaders.
     * @param shader_dir Directory containing shaders.
     * @param name Shader name, must be the same as the files name without extensions.
     */
    Shader (string shader_dir, string name) {

        shaderName = name;

        bool found = false;

        //Vertex:
        string vs_name = shader_dir + name + ".vert";
        ifstream vertex_file(vs_name.c_str());
        if (vertex_file.good())
        {
            vertexShaderPath = vs_name;
            found = true;
        }

        //Geometry:
        string gs_name = shader_dir + name + ".geom";
        ifstream geom_file(gs_name.c_str());
        if (geom_file.good())
        {
            geometryShaderPath = gs_name;
            found = true;
        }

        //Fragment:
        string fs_name = shader_dir + name + ".frag";
        ifstream fragment_file(fs_name.c_str());
        if (fragment_file.good())
        {
            fragmentShaderPath = fs_name;
            found = true;
        }

        //Compute:
        string cs_name = shader_dir + name + ".comp";
        ifstream comp_file(cs_name.c_str());
        if (comp_file.good())
        {
            computeShaderPaths.push_back(cs_name);
            found = true;
        }

        // if no shader was found, emit an warning
        if (!found)
        {
            cerr << "Warning: no shader " << name.c_str() << " file found in directory : " << shader_dir.c_str() << endl;
        }

        vertexShader = 0;
        fragmentShader = 0;
        geometryShader = 0;
        shaderProgram = 0;
        computeShaders = vector<GLuint>();
    }

    /**
     * @brief Link shader program and check for link errors.
     */
    void linkProgram (void)
    {

        glLinkProgram(shaderProgram);

        GLint result = GL_FALSE;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
        if (result != GL_TRUE)
        {
            cerr << "Error linking program : " << shaderName << endl;
            GLchar errorLog[1024] = {0};
            glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
            fprintf(stdout, "%s", &errorLog[0]);
            cerr << endl;
        }
        #ifdef TUCANODEBUG
        else
        {
            cout << " Linked program with no errors : " << shaderName << endl << endl;
        }
        #endif
    }


    /**
     * @brief Initializes shader and prepares it to use Transform Feedback.
     *
     * To use Transform Feedbacks somes parameters must be set before linking the program.
     * These parameters are the number and list of varyings to be set by the TF, and the type of buffer.
     * @param size Number of Transform Feedback varyings.
     * @param varlist List of varyings for TF.
     * @param buffer_mode Can be interleaved attributes or separate attributes (default is GL_INTERLEAVED_ATTRIBS).
     */
    void initializeTF (int size, const char** varlist, GLenum buffer_mode = GL_INTERLEAVED_ATTRIBS)
    {
        createShaders();

        if(!vertexShaderPath.empty())
        {
            readVertexCode();
            // geom shader needs a vertex shader
            if(!geometryShaderPath.empty())
            {
                readGeometryCode();
            }
        }
        if(!fragmentShaderPath.empty())
        {
            readFragmentCode();
        }
        if(computeShaderPaths.size() > 0)
        {
            setComputeShaders();
        }

        glTransformFeedbackVaryings(shaderProgram, size, varlist, buffer_mode);

        linkProgram();

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }

    /**
     * @brief Initializes shader directly from string, no files.
     * @param vertex_code String containing vertex code.
     * @param fragment_code String containing fragment code.
     * @param geometry_code String containing geometry code, this is optional.
     */
    void initializeFromStrings (string vertex_code, string fragment_code, string geometry_code = "")
    {
        //Create Shader Program.
        shaderProgram = glCreateProgram();

        if (vertex_code.empty())
        {
            cerr << "warning: " << shaderName.c_str() << " : empty vertex string code!" << endl;
        }
        else
        {
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            setVertexShader(vertex_code);
            // geometry shader is optional, but need a vertex shader if is set
            if (!geometry_code.empty())
            {
                geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
                setGeometryShader(geometry_code);
            }
        }
        if (fragment_code.empty())
        {
            cerr << "warning: " << shaderName.c_str() << " : empty fragment string code!" << endl;
        }
        else
        {
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            setFragmentShader(fragment_code);
        }

        linkProgram();

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }

    /**
     * @brief Calls all the functions related to the shader initialization, i.e., creates, loads the shaders from the external files and links the shader program.
     */
    void initialize (void)
    {
        createShaders();

        if(!vertexShaderPath.empty())
        {
            readVertexCode();
            // geom shader needs a vertex shader
            if(!geometryShaderPath.empty()) {
                readGeometryCode();
            }
        }
        if(!fragmentShaderPath.empty())
        {
            readFragmentCode();
        }
        if(computeShaderPaths.size() > 0)
        {
            setComputeShaders();
        }        

        linkProgram();

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }


    /**
     * @brief Creates the GLSL shader objects, storing the identification handle for each shader.
     */
    void createShaders (void)
    {
        //Create Shader Program.
        shaderProgram = glCreateProgram();

        if(!fragmentShaderPath.empty())
        {
            //Create Fragment Shader.
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        }

        if(!vertexShaderPath.empty())
        {
            //Create Vertex Shader.
            vertexShader = glCreateShader(GL_VERTEX_SHADER);

            // vertex shader is mandatory when using geom shader
            if(!geometryShaderPath.empty())
            {
                //Create Geometry Shader.
                geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            }
        }

        if(computeShaderPaths.size() > 0)
        {
            //Create Compute Shaders.
            for(unsigned int i = 0; i<computeShaderPaths.size(); i++)
            {
                computeShaders.push_back(glCreateShader(GL_COMPUTE_SHADER));
            }
        }
    }


    /**
     * @brief Loads vertex code into shader program.
     * @param vertexShaderCode String containing code
     */
    void setVertexShader (string &vertexShaderCode)
    {
        GLint result = GL_FALSE;
        int infoLogLength;

        char const * vertexSourcePointer = vertexShaderCode.c_str();
        glShaderSource(vertexShader, 1, &vertexSourcePointer , NULL);
        glCompileShader(vertexShader);

        // Check Vertex Shader
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            // if an error is found, print the log (even if not in debug mode)
            cerr << "Erro compiling vertex shader: " << vertexShaderPath << endl;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            char * vertexShaderErrorMessage = new char[infoLogLength];
            glGetShaderInfoLog(vertexShader, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
            fprintf(stdout, "\n%s", &vertexShaderErrorMessage[0]);
            delete [] vertexShaderErrorMessage;
        }
        #ifdef TUCANODEBUG
        else
        {
            if (vertexShaderPath.empty())
            {
                cout << "Compiled vertex shader from string without errors : " << shaderName.c_str() << endl;
            }
            else
            {
                cout << "Compiled vertex shader without errors : " << vertexShaderPath << endl;
            }
        }
        #endif

        glAttachShader(shaderProgram, vertexShader);

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__, "error loading vertex shader code");
        #endif

    }

    /**
     * @brief Reads the external file containing the vertex shader and loads it into the shader program.
     */
    void readVertexCode (void)
    {
        // Read the Vertex Shader code from the file
        string vertexShaderCode;

        ifstream vertexShaderStream(vertexShaderPath.c_str(), std::ios::in);

        if(vertexShaderStream.is_open())
        {
            string line = "";
            while(getline(vertexShaderStream, line))
            {
                vertexShaderCode += "\n" + line;
            }
            vertexShaderStream.close();
        }
        else
        {
            cout << "warning: no vertex shader file found : " << vertexShaderPath << endl;
        }

        setVertexShader(vertexShaderCode);
    }


    /**
     * @brief Loads geometry code into shader program.
     * @param geometryShaderCode String containing geometry code.
     */
    void setGeometryShader(string &geometryShaderCode)
    {
        GLint result = GL_FALSE;
        int infoLogLength;

        char const * geometrySourcePointer = geometryShaderCode.c_str();
        glShaderSource(geometryShader, 1, &geometrySourcePointer , NULL);
        glCompileShader(geometryShader);

        // Check Geometry Shader
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            // if an error is found, print the log (even if not in debug mode)
            cerr << "Erro compiling geometry shader: " << geometryShaderPath << endl;
            glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            char * geometryShaderErrorMessage = new char[infoLogLength];
            glGetShaderInfoLog(geometryShader, infoLogLength, NULL, &geometryShaderErrorMessage[0]);
            fprintf(stdout, "\n%s", &geometryShaderErrorMessage[0]);
            delete [] geometryShaderErrorMessage;
        }
        #ifdef TUCANODEBUG
        else
        {
            if (geometryShaderPath.empty())
            {
                cout << "Compiled geometry shader from string without errors : " << shaderName.c_str() << endl;
            }
            else
            {
                cout << "Compiled geometry shader without errors : " << geometryShaderPath << endl;
            }
        }
        #endif

        glAttachShader(shaderProgram, geometryShader);

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__, "error loading geometry shader code");
        #endif

    }

    /**
     * @brief Reads the external file containing the geometry shader and loads it into the shader program.
     */
    void readGeometryCode (void)
    {
        // Read the Geometry Shader code from the file
        string geometryShaderCode;

        ifstream geometryShaderStream(geometryShaderPath.c_str(), std::ios::in);

        if(geometryShaderStream.is_open())
        {
            string line = "";
            while(getline(geometryShaderStream, line))
            {
                geometryShaderCode += "\n" + line;
            }
            geometryShaderStream.close();
        }
        else
        {
            cerr << "warning: no geom shader found : " << geometryShaderPath << endl;
        }

        setGeometryShader(geometryShaderCode);
    }

    /**
     * @brief Loads fragment code into shader program.
     * @param fragmentShaderCode String containing fragment code.
     */
    void setFragmentShader (string& fragmentShaderCode)
    {
        GLint result = GL_FALSE;
        int infoLogLength;

        char const * fragmentSourcePointer = fragmentShaderCode.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSourcePointer , NULL);
        glCompileShader(fragmentShader);

        // Check Fragment Shader
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
        if (result != GL_TRUE)
        {
            cerr << "Erro compiling fragment shader: " << fragmentShaderPath << endl;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
            char * fragmentShaderErrorMessage = new char[infoLogLength];
            glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
            fprintf(stdout, "\n%s", &fragmentShaderErrorMessage[0]);
            delete [] fragmentShaderErrorMessage;
        }
        #ifdef TUCANODEBUG
        else
        {
            if (fragmentShaderPath.empty())
            {
                cout << "Compiled fragment shader from string without errors : " << shaderName.c_str() << endl;
            }
            else
            {
                cout << "Compiled fragment shader without errors : " << fragmentShaderPath << endl;
            }
        }
        #endif

        glAttachShader(shaderProgram, fragmentShader);

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__, "error loading fragment shader code");
        #endif

    }

    /**
     * @brief Reads the external file containing the fragment shader and loads it into the shader program.
     */
    void readFragmentCode (void)
    {
        // Read the Fragment Shader code from the file
        string fragmentShaderCode;
        ifstream fragmentShaderStream(fragmentShaderPath.c_str(), std::ios::in);
        if(fragmentShaderStream.is_open())
        {
            string line = "";
            while(getline(fragmentShaderStream, line))
            {
                fragmentShaderCode += "\n" + line;
            }
            fragmentShaderStream.close();
        }
        else
        {
            cerr << "warning: no fragment shader found : " << fragmentShaderPath << endl;
        }

        setFragmentShader(fragmentShaderCode);
    }

    /**
     * @brief Reads the external files containing the compute shaders and loads them into the shader program.
     */
    void setComputeShaders (void)
    {
        int position = 0; //Position of the shader in computeShaders vector.


        for(vector<string>::iterator it = computeShaderPaths.begin(); it != computeShaderPaths.end(); it++)
        {
            // Read the compute shader code from the file
            string computeShaderCode;
            ifstream computeShaderStream(it->c_str(), std::ios::in);
            if(computeShaderStream.is_open())
            {
                string line = "";
                while(getline(computeShaderStream, line))
                {
                    computeShaderCode += "\n" + line;
                }
                computeShaderStream.close();
            }

            GLint result = GL_FALSE;
            int infoLogLength;

            // Compile Compute Shader
            if (debug_level > 0)
                //cout << "Compiling compute shader: " << *it << endl;
                cout << "Compiling compute shader: " << *it;
            char const * computeSourcePointer = computeShaderCode.c_str();
            glShaderSource(computeShaders[position], 1, &computeSourcePointer , NULL);
            glCompileShader(computeShaders[position]);

            // Check Fragment Shader
            glGetShaderiv(computeShaders[position], GL_COMPILE_STATUS, &result);
            if (result != GL_TRUE)
            {
                cerr << "Erro compiling compute shader: " << computeShaderPaths[position] << endl;
                glGetShaderiv(computeShaders[position], GL_INFO_LOG_LENGTH, &infoLogLength);
                char * computeShaderErrorMessage = new char[infoLogLength];
                glGetShaderInfoLog(computeShaders[position], infoLogLength, NULL, &computeShaderErrorMessage[0]);
                fprintf(stdout, "\n%s", &computeShaderErrorMessage[0]);
                delete [] computeShaderErrorMessage;
            }
            #ifdef TUCANODEBUG
            else
            {
                if (computeShaderPaths[position].empty())
                {
                    cout << "Compiled compute shader from string without errors : " << shaderName.c_str() << endl;
                }
                else
                {
                    cout << "Compiled compute shader without errors : " << computeShaderPaths[position] << endl;
                }
            }
            #endif

            glAttachShader(shaderProgram, computeShaders[position]);

            #ifdef TUCANODEBUG
            Misc::errorCheckFunc(__FILE__, __LINE__, "error loading compute shader code");
            #endif

            position++;
        }
    }


    /**
     * @brief Reloads all shaders by reading the files again.
     *
     * This feature enables runtime editing of the shader codes.
     * After saving the text file after editing, the reload applies changes immediately.
     */
    void reloadShaders (void)
    {

        #ifdef TUCANODEBUG
        cout << "reloading shaders" << endl;
        #endif

        if(vertexShader != 0)
        {
            glDetachShader(shaderProgram, vertexShader);
            readVertexCode();
        }
        if(geometryShader != 0)
        {
            glDetachShader(shaderProgram, geometryShader);
            readGeometryCode();
        }
        if(fragmentShader != 0)
        {
            glDetachShader(shaderProgram, fragmentShader);
            readFragmentCode();
        }
        if(computeShaders.size() > 0)
        {
            for(unsigned int i = 0; i<computeShaders.size(); i++)
            {
                glDetachShader(shaderProgram, computeShaders[i]);
            }
            setComputeShaders();
        }

        linkProgram();

        #ifdef TUCANODEBUG
        Misc::errorCheckFunc(__FILE__, __LINE__);
        #endif
    }

    /**
     * @brief Enables the shader program for usage.
     *
     * After enabling a shader any OpenGL draw call will use it for rendering.
     */
    void bind (void)
    {
        glUseProgram(shaderProgram);
    }

    /**
     * @brief Disables the shader program.
     */
    void unbind (void)
    {
        glUseProgram(0);
    }

    /**
     * @brief Detaches and deletes the shaders and the shader program.
     */
    void deleteShaders (void)
    {
        glDetachShader(shaderProgram, fragmentShader);
        glDetachShader(shaderProgram, vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteProgram(shaderProgram);
    }


    /**
     * Given the name of a uniform used inside the shader, returns it's location.
     * @param name Name of the uniform variable in shader.
     * @return The uniform location.
     */
    GLint getUniformLocation (const GLchar* name)
    {
        return glGetUniformLocation(shaderProgram, name);
    }

    /**
     * Returns the location of an attribute, such as a vertex attribute
     * @param name Name of the attribute variable in the shader.
     * @return The attribute location, or -1 if the attribute was not found or has an invalid name.
     */
    GLint getAttributeLocation (const GLchar* name)
    {
        return glGetAttribLocation(shaderProgram, name);
    }

    //============================Uniforms Setters==========================================================


    //============================ Integer ==========================================================

    /**
     * @brief Sets an uniform integer 4D vector (ivec4) given a location and the vector values.
     * @param location Location handle of uniform variable.
     * @param a First value of the ivec4.
     * @param b Second value of the ivec4.
     * @param c Third value of the ivec4.
     * @param d Fourth value of the ivec4.
     */
    void setUniform (GLint location, GLint a, GLint b, GLint c, GLint d)
    {
        glUniform4i(location, a, b, c, d);
    }

    /**
     * @brief Sets an uniform integer 3D vector (ivec3) given a location and the vector values.
     * @param location Location handle of uniform variable.
     * @param a First value of the ivec3.
     * @param b Second value of the ivec3.
     * @param c Third value of the ivec3.
     */
    void setUniform (GLint location, GLint a, GLint b, GLint c)
    {
        glUniform3i(location, a, b, c);
    }

    /**
     * @brief Sets an uniform integer 2D vector (ivec2) given a location and the vector values.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec2.
     * @param b Second value of the vec2.
     */
    void setUniform (GLint location, GLint a, GLint b)
    {
        glUniform2i(location, a, b);
    }

    /**
     * @brief Sets an uniform integer given a location and the integer value.
     * @param location Location handle of uniform variable.
     * @param a Integer value.
     */
    void setUniform (GLint location, GLint a)
    {
        glUniform1i(location, a);
    }

    /**
     * @brief Sets an uniform integer 4D vector (ivec4) given its name in the shader and the vector values.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the ivec4.
     * @param b Second value of the ivec4.
     * @param c Third value of the ivec4.
     * @param d Fourth value of the ivec4.
     */
    void setUniform (const GLchar* name, GLint a, GLint b, GLint c, GLint d)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b, c, d);
    }

    /**
     * @brief Sets an uniform integer 3D vector (ivec3) given its name in the shader and the vector values.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the ivec3.
     * @param b Second value of the ivec3.
     * @param c Third value of the ivec3.
     */
    void setUniform (const GLchar* name, GLint a, GLint b, GLint c)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b, c);
    }

    /**
     * @brief Sets an uniform integer 2D vector (ivec2) given its name in the shader and the vector values.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the ivec2.
     * @param b Second value of the ivec2.
     */
    void setUniform (const GLchar* name, GLint a, GLint b)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b);
    }

    /**
     * @brief Sets an uniform integer given its name in the shader and the integer value.
     * @param name Name of uniform variable in the shader code.
     * @param a Integer value.
     */
    void setUniform (const GLchar* name, GLint a)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a);
    }

    /**
     * @brief Sets an uniform integer 4D vector (ivec4) given a location and the vector with values.
     * @param location Location handle of uniform variable.
     * @param vec 4D integer vector.
     */
    void setUniform (GLint location, const Eigen::Vector4i &vec)
    {
        glUniform4i(location, vec[0], vec[1], vec[2], vec[3]);
    }

    /**
     * @brief Sets an uniform integer 3D vector (ivec3) given a location and the vector with values.
     * @param location Location handle of uniform variable.
     * @param vec 3D integer vector.
     */
    void setUniform (GLint location, const Eigen::Vector3i &vec)
    {
        glUniform3i(location, vec[0], vec[1], vec[2]);
    }

    /**
     * @brief Sets an uniform integer 2D vector (ivec2) given a location and the vector with values.
     * @param location Location handle of uniform variable.
     * @param vec 2D integer vector.
     */
    void setUniform (GLint location, const Eigen::Vector2i &vec)
    {
        glUniform2i(location, vec[0], vec[1]);
    }

    /**
     * @brief Sets an uniform integer 4D vector (ivec4) given its name in the shader and the vector with values.
     * @param name Name of uniform variable in the shader code.
     * @param vec 4D integer vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector4i &vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    /**
     * @brief Sets an uniform integer 3D vector (ivec3) given its name in the shader and the vector with values.
     * @param name Name of uniform variable in the shader code.
     * @param vec 3D integer vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector3i &vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    /**
     * @brief Sets an uniform integer 2D vector (ivec2) given its name in the shader and the vector with values.
     * @param name Name of uniform variable in the shader code.
     * @param vec 2D integer vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector2i &vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    //============================ Float ==========================================================


    /**
     * @brief Sets an uniform float 4D vector (vec4) given a location and the vector values.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec4.
     * @param b Second value of the vec4.
     * @param c Third value of the vec4.
     * @param d Fourth value of the vec4.
     */
    void setUniform (GLint location, GLfloat a, GLfloat b, GLfloat c, GLfloat d)
    {
        glUniform4f(location, a, b, c, d);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given a location and the vector values.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec3.
     * @param b Second value of the vec3.
     * @param c Third value of the vec3.
     */
    void setUniform (GLint location, GLfloat a, GLfloat b, GLfloat c)
    {
        glUniform3f(location, a, b, c);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given a location and the vector values.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec2.
     * @param b Second value of the vec2.
     */
    void setUniform (GLint location, GLfloat a, GLfloat b)
    {
        glUniform2f(location, a, b);
    }

    /**
     * @brief Sets an uniform float given a location and the float value.
     * @param location Location handle of uniform variable.
     * @param a Float value.
     */
    void setUniform (GLint location, GLfloat a)
    {
        glUniform1f(location, a);
    }

    /**
     * @brief Sets an uniform float 4D vector (vec4) given its name in the shader and the vector values.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the vec4.
     * @param b Second value of the vec4.
     * @param c Third value of the vec4.
     * @param d Fourth value of the vec4.
     */
    void setUniform (const GLchar* name, GLfloat a, GLfloat b, GLfloat c, GLfloat d)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b, c, d);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given its name in the shader and the vector values.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the vec3.
     * @param b Second value of the vec3.
     * @param c Third value of the vec3.
     */
    void setUniform (const GLchar* name, GLfloat a, GLfloat b, GLfloat c)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b, c);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec4) given its name in the shader and the vector values.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the vec2.
     * @param b Second value of the vec2.
     */
    void setUniform (const GLchar* name, GLfloat a, GLfloat b)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b);
    }

    /**
     * @brief Sets an uniform float given its name in the shader and the float value.
     * @param name Name of uniform variable in the shader code.
     * @param a Float value.
     */
    void setUniform (const GLchar* name, GLfloat a)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a);
    }

    /**
     * @brief Sets an uniform float 4D vector (vec4) given a location and the vector with values.
     * @param location Location handle of uniform variable.
     * @param vec 4D float vector.
     */
    void setUniform (GLint location, const Eigen::Vector4f &vec)
    {
        glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given a location and the vector with values.
     * @param location Location handle of uniform variable.
     * @param vec 3D float vector.
     */
    void setUniform (GLint location, const Eigen::Vector3f &vec)
    {
        glUniform3f(location, vec[0], vec[1], vec[2]);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given a location and the vector with values.
     * @param location Location handle of uniform variable.
     * @param vec 2D float vector.
     */
    void setUniform (GLint location, const Eigen::Vector2f &vec)
    {
        glUniform2f(location, vec[0], vec[1]);
    }

    /**
     * @brief Sets an uniform float 4D vector (vec4) given its name in the shader and the vector with values.
     * @param name Name of uniform variable in the shader code.
     * @param vec 4D float vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector4f &vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given its name in the shader and the vector with values.
     * @param name Name of uniform variable in the shader code.
     * @param vec 3D float vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector3f &vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given its name in the shader and the vector with values.
     * @param name Name of uniform variable in the shader code.
     * @param vec 2D float vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector2f &vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    //============================ Double ==========================================================

    /**
     * @brief Sets an uniform float 4D vector (vec4) given a location and the vector values as Double.
     * The double values are converted to float.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec4.
     * @param b Second value of the vec4.
     * @param c Third value of the vec4.
     * @param d Fourth value of the vec4.
     */
    void setUniform (GLint location, GLdouble a, GLdouble b, GLdouble c, GLdouble d)
    {
        glUniform4f(location, (GLfloat)a, (GLfloat)b, (GLfloat)c, (GLfloat)d);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given a location and the vector values as Double.
     * The double values are converted to float.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec3.
     * @param b Second value of the vec3.
     * @param c Third value of the vec3.
     */
    void setUniform (GLint location, GLdouble a, GLdouble b, GLdouble c)
    {
        glUniform3f(location, (GLfloat)a, (GLfloat)b, (GLfloat)c);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given a location and the vector values as Double.
     * The double values are converted to float.
     * @param location Location handle of uniform variable.
     * @param a First value of the vec2.
     * @param b Second value of the vec2.
     */
    void setUniform (GLint location, GLdouble a, GLdouble b)
    {
        glUniform2f(location, (GLfloat)a, (GLfloat)b);
    }

    /**
     * @brief Sets an uniform float given a location and a double value that is converted to float.
     * @param location Location handle of uniform variable.
     * @param a Double value.
     */
    void setUniform (GLint location, GLdouble a)
    {
        glUniform1f(location, (GLfloat)a);
    }

    /**
     * @brief Sets an uniform float 4D vector (vec4) given its name in the shader and the vector values as Double.
     *
     * The double values are converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the vec4.
     * @param b Second value of the vec4.
     * @param c Third value of the vec4.
     * @param d Fourth value of the vec4.
     */
    void setUniform (const GLchar* name, GLdouble a, GLdouble b, GLdouble c, GLdouble d)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b, c, d);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given its name in the shader and the vector values as Double.
     *
     * The double values are converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the vec3.
     * @param b Second value of the vec3.
     * @param c Third value of the vec3.
     */
    void setUniform (const GLchar* name, GLdouble a, GLdouble b, GLdouble c)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b, c);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given its name in the shader and the vector values as Double.
     *
     * The double values are converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param a First value of the vec2.
     * @param b Second value of the vec2.
     */
    void setUniform (const GLchar* name, GLdouble a, GLdouble b)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a, b);
    }
    /**
     * @brief Sets an uniform float given its name in the shader and a double value that is converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param a Double value.
     */
    void setUniform (const GLchar* name, GLdouble a)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, a);
    }


    /**
     * @brief Sets an uniform float 4D vector (vec4) given a location and the double vector that is converted to float.
     * @param location Location handle of uniform variable.
     * @param vec 4D double vector.
     */
    void setUniform (GLint location, const Eigen::Vector4d vec)
    {
        glUniform4f(location, (GLfloat)vec[0], (GLfloat)vec[1], (GLfloat)vec[2], (GLfloat)vec[3]);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given a location and the double vector that is converted to float.
     * @param location Location handle of uniform variable.
     * @param vec 3D double vector.
     */
    void setUniform (GLint location, const Eigen::Vector3d vec)
    {
        glUniform3f(location, (GLfloat)vec[0], (GLfloat)vec[1], (GLfloat)vec[2]);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given a location and the double vector that is converted to float.
     * @param location Location handle of uniform variable.
     * @param vec 2D double vector.
     */
    void setUniform (GLint location, const Eigen::Vector2d vec)
    {
        glUniform2f(location, (GLfloat)vec[0], (GLfloat)vec[1]);
    }

    /**
     * @brief Sets an uniform float 4D vector (vec4) given its name in the shader and the double vector that is converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param vec 4D double vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector4d vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    /**
     * @brief Sets an uniform float 3D vector (vec3) given its name in the shader and the double vector that is converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param vec 3D double vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector3d vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }

    /**
     * @brief Sets an uniform float 2D vector (vec2) given its name in the shader and the double vector that is converted to float.
     * @param name Name of uniform variable in the shader code.
     * @param vec 2D double vector.
     */
    void setUniform (const GLchar* name, const Eigen::Vector2d vec)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, vec);
    }


    //============================ Vector from Array ==========================================================

    /**
     * @brief Sets a integer uniform vector value given its location and an array with the values.
     *
     * The vector will be passed as int, ivec2, ivec3, or ivec4 depending on the number of values.
     * @param location Location handle of uniform variable.
     * @param v Integer array holding the values.
     * @param nvalues Number of elements per vector.
     * @param count Number of vectors.
     */
    void setUniform (GLint location, const GLint* v, GLuint nvalues, GLsizei count = 1)
    {
        switch (nvalues)
        {
            case 1: glUniform1iv(location, count, v); break;
            case 2: glUniform2iv(location, count, v); break;
            case 3: glUniform3iv(location, count, v); break;
            case 4: glUniform4iv(location, count, v); break;
        }
    }
    /**
     * @brief Sets a float uniform vector value given its location and an array with the values.
     *
     * The vector will be passed as float, vec2, vec3, or vec4 depending on the number of values.
     * @param location Location handle of uniform variable.
     * @param v Float array holding the values.
     * @param nvalues Number of values in the vector v.
     * @param nvalues Number of elements per vector.
     * @param count Number of vectors.
     */
    void setUniform (GLint location, const GLfloat* v, GLuint nvalues, GLsizei count = 1)
    {
        switch (nvalues)
        {
            case 1: glUniform1fv(location, count, v); break;
            case 2: glUniform2fv(location, count, v); break;
            case 3: glUniform3fv(location, count, v); break;
            case 4: glUniform4fv(location, count, v); break;
        }
    }

    /**
     * @brief Sets a integer uniform vector value given its name in shader and an array with the values.
     *
     * The vector will be passed as int, ivec2, ivec3, or ivec4 depending on the number of values.
     * @param name Name of uniform variable in the shader code.
     * @param v Integer array holding the values.
     * @param nvalues Number of elements per vector.
     * @param count Number of vectors.
     */
    void setUniform (const GLchar* name, const GLint* v, GLuint nvalues, GLsizei count = 1)
    {
        GLint location = getUniformLocation(name);
        setUniform(location,v,nvalues,count);
    }

    /**
     * @brief Sets a float uniform vector value given its name in shader and an array with the values.
     *
     * The vector will be passed as float, vec2, vec3, or vec4 depending on the number of values.
     * @param name Name of uniform variable in the shader code.
     * @param v Float array holding the values.
     * @param nvalues Number of values in the vector v.
     * @param nvalues Number of elements per vector.
     * @param count Number of vectors.
     */
    void setUniform (const GLchar* name, const GLfloat* v, GLuint nvalues, GLsizei count = 1)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, v, nvalues, count);
    }


    //============================ Matrix ==========================================================

    /**
     * @brief Sets a uniform float matrix value given its location.
     *
     * The uniform matrix is assumed to be a mat2, mat3, or mat4 depending on the given dimension.
     * @param location  Location handle of uniform variable.
     * @param m Matrix value with dim^2 values
     * @param dim Dimension of the matrix m.
     * @param transpose If transpose is GL_FALSE, each matrix is assumed to be supplied in
     *					in column major order, otherwise is in row major order.
     * @param count  Number of elements of the uniform matrix array to be modified.
     *				a count of 1 should be used if modifying the value of a single matrix.
     */
    void setUniform (GLint location, const GLfloat* m, GLuint dim, GLboolean transpose = GL_FALSE, GLsizei count = 1)
    {
        switch(dim)
        {
            case 2: glUniformMatrix2fv(location, count, transpose, m); break;
            case 3: glUniformMatrix3fv(location, count, transpose, m); break;
            case 4: glUniformMatrix4fv(location, count, transpose, m); break;
        }
    }

    /**
     * @brief Sets a uniform float matrix value given its name in shader code.
     *
     * The uniform matrix is assumed to be a mat2, mat3, or mat4 depending on the given dimension.
     * @param name Name of uniform variable in the shader code.
     * @param m Matrix value with dim^2 values
     * @param dim Dimension of the matrix m.
     * @param transpose If transpose is GL_FALSE, each matrix is assumed to be supplied in
     *					in column major order, otherwise is in row major order.
     * @param count  Number of elements of the uniform matrix array to be modified.
     *				a count of 1 should be used if modifying the value of a single matrix.
     */
    void setUniform (const GLchar* name, const GLfloat* m, GLuint dim, GLboolean transpose = GL_FALSE, GLsizei count = 1)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, m, dim, transpose, count);
    }

    /**
     * @brief Sets a uniform 4x4 float matrix value given its location and eigen 4x4 matrix.
     * @param location  Location handle of uniform variable.
     * @param matrix 4x4 float matrix.
     */
    void setUniform (GLint location, const Eigen::Matrix4f &matrix)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
    }

    /**
     * @brief Sets a uniform 3x3 float matrix value given its location and eigen 3x3 matrix.
     * @param location  Location handle of uniform variable.
     * @param matrix 3x3 float matrix.
     */
    void setUniform (GLint location, const Eigen::Matrix3f &matrix)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, matrix.data());
    }

    /**
     * @brief Sets a uniform 2x2 float matrix value given its location and eigen 2x2 matrix.
     * @param location  Location handle of uniform variable.
     * @param matrix 2x2 float matrix.
     */
    void setUniform (GLint location, const Eigen::Matrix2f &matrix)
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, matrix.data());
    }

    /**
     * @brief Sets a uniform 4x4 float matrix value given its name in shader and eigen 4x4 matrix.
     * @param name Name of uniform variable in the shader code.
     * @param matrix 4x4 float matrix.
     */
    void setUniform (const GLchar* name, const Eigen::Matrix4f &matrix)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, matrix);
    }

    /**
     * @brief Sets a uniform 3x3 float matrix value given its name in shader and eigen 3x3 matrix.
     * @param name Name of uniform variable in the shader code.
     * @param matrix 3x3 float matrix.
     */
    void setUniform (const GLchar* name, const Eigen::Matrix3f &matrix)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, matrix);
    }

    /**
     * @brief Sets a uniform 2x2 float matrix value given its name in shader and eigen 2x2 matrix.
     * @param name Name of uniform variable in the shader code.
     * @param matrix 2x2 float matrix.
     */
    void setUniform (const GLchar* name, const Eigen::Matrix2f &matrix)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, matrix);
    }

    /**
     * @brief Sets a uniform 4x4 float matrix value given its location and eigen 3x3 affine matrix.
     * @param location Location handle of uniform variable.
     * @param affine_matrix 3x3 float affine matrix.
     */
    void setUniform (GLint location, const Eigen::Affine3f &affine_matrix)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, affine_matrix.matrix().data());
    }

    /**
     * @brief Sets a uniform 3x3 float matrix value given its location and eigen 2x2 affine matrix.
     * @param location Location handle of uniform variable.
     * @param affine_matrix 2x2 float affine matrix.
     */
    void setUniform (GLint location, const Eigen::Affine2f &affine_matrix)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, affine_matrix.matrix().data());
    }

    /**
     * @brief Sets a uniform 4x4 float matrix value given its name in shader and eigen 3x3 affine matrix.
     * @param name Name of uniform variable in the shader code.
     * @param affine_matrix 3x3 float affine matrix.
     */
    void setUniform (const GLchar* name, const Eigen::Affine3f &affine_matrix)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, affine_matrix);
    }

    /**
     * @brief Sets a uniform 3x3 float matrix value given its name in shader and eigen 2x2 affine matrix.
     * @param name Name of uniform variable in the shader code.
     * @param affine_matrix 2x2 float affine matrix.
     */
    void setUniform (const GLchar* name, const Eigen::Affine2f &affine_matrix)
    {
        GLint location = getUniformLocation(name);
        setUniform(location, affine_matrix);
    }


};

}
#endif
