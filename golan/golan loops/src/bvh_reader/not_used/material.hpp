#ifndef material_h
#define material_h

#include <GL/gl.h>
#include <string>
#include <vector>

#include "triangle.hpp"

using namespace std;

/// Holds all vertices and material info for objloader entities
/**

  UVs and textures aren't supported, as I'm not really interested
  in them, being literally superficial in addition to me not having
  any easy-to-use UV mappers (lithunwrap is a pain).

 */
class material{

public:

	material();
	~material();

	material& operator= (const material &mat);

	/// material name defined in the parsed .obj files
	string name;
	//char name[128];

	//int numFaces;
	//int numUVs;

	
	vector<triangleInd> faces;
	//uv		 **uvs;

	// reflectances
	/// shininess exponent 0-128
	GLfloat Se;		
	/// Ambient
	GLfloat Ka[4];
	/// Diffuse	
	GLfloat Kd[4];	
	/// Specular
	GLfloat Ks[4];  
	/// Emission
	GLfloat Ke[4];  

	
	GLfloat opacity;

	// Spring parts 
	// Eventually make into inherited class?

	// Make the material out of springs?
	//bool springy;

	//pointMass **weights;
	//spring **springs;

};

#endif // material_h
