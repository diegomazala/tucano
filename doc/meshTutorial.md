Using the Mesh Class                         {#mesh_tutorial}
====================

The Mesh Class contains many basic methods to quickly load and use a mesh.

A Mesh Object contains Vertex Attributes, that are translated into OpenGL Arrays. Vertex Attributes are information per vertex, such as coordinates, normals, color, etc...

---

## Importers

If you have an `OBJ` file, you can simple load it by:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    
#include "utils/objimporter.hpp"

...

Tucano::Mesh * mymesh = new Tucano::Mesh();
Tucano::MeshImporter::loadObjFile(mesh, filename);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    

The PLY importer is under construction...


## Vertex Attributes

    
When reading a file, the importer will look for the following vertex attributes and automatically load them with the following names:

- Vertex coordinates : "in_Position"
- Normal : "in_Normal"
- Color : "in_Color"
- Texture Coordinates : "in_TexCoords"

To add a new attribute to your Mesh, lets say a Secondary Color Attribute, pass the vector with the values and the name of the attribute to the Mesh:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
vector< Eigen::Vector4f > secondary_color;
//  ... fill your vector with the secondary color information ...
mymesh->createAttribute("in_SecondaryColor", secondary_color);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  

and link the Attributes with your Shader:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mesh->setAttributeLocation(myshader);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The setAttributeLocation method receives a pointer to a Shader, and looks for matches by name. If you name your shader *in* variables as:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
in vec4 in_Position;
in vec4 in_SecondaryColor;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
they are automatically linked to the Mesh Vertex Attributes named "in_Position" and "in_SecondaryColor".

If you have different names for the Shader variable and the corresponding Vertex Attribute, you can still link the attribute:
    
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mesh->setAttributeLocation("in_MyAttribute", myshader->getAttributeLocation("myVertexAttribName"));
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that you might need to reset the locations when linking the names manually (`resetLocations` is already called by `setAttributeLocation`):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mesh->resetLocations();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## Model Methods

The Mesh Class extends the Model Class, that basically contains spatial information about the model, i.e, a Model Matrix.
    
To normalize the Mesh coordinates to fit in a unit cube centered at the origin, update the Model Matrix with the following call.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mymesh->normalizeModelMatrix();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
> Note, however, that information such as centroid and scale are computed by the Importer, so if the Mesh was filled manually they have to be computed to use the normalization method.

    
Pass your Model Matrix to your Shader to apply the transformation:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
myshader->setUniform("modelMatrix", mymesh->getModelMatrix());
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
and read it in your *myshader* code:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uniform mat4 modelMatrix;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
