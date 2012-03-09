#ifndef objloader_H
#define objloader_H

#include <vector>
#include <string>

#include "movable.hpp" 
#include "light.hpp"
#include "material.hpp"
#include "triangle.hpp"


#include <GL/glu.h>
//#include "pointMass.hpp"
//#include "spring.hpp"


typedef struct uv {
  float uorv[2];           
};

/// Parse in .obj's 
/**
	I could do more parsers for other formats but .objs fit all my
	needs for now (I'll need my own format most likely).

	Perhaps I should further separate obj loading from the physics operations
	for finding the interior points and so forth.

	Probably this shouldn't be a movable subclass anymore.
 */
class objloader:public movable
{
	enum objMode {NONE,MTLLIB,VERTEX,NORMAL,FACE};
	enum mtlMode {NONEM,NEWMTL,NS,D,ILLUM,KD,KA,KS};
public:

	objloader();
	objloader(string objFile);
	~objloader();

	/// Load .obj file
	bool load(string objFile);
	/// Parse each line of .obj file 
	void process(string line);
	/// Parse .mtl file specified in .obj file
	void loadMtl(string mtlFile);
	/// Parse each line of .mtl file into material objects
	void processMtl(string line, material *mtl);
	/// Match specified material to loaded materials
	bool matchMtl(unsigned &index, string name);
	/// Change or initialize total mass
	void setMass(float newMass);

	void draw(void);
	/// Does nothing currently, put scripted movements here
	void update(float seconds);

	objloader& operator= (const objloader &obj1);

	//void InitTexture(void);

	void getBoundingBox(void);

	bool success;

	/// For scripted movements, unused currently
	int counter;

private:
	///loading variables
	string subdir;
	string mtlFile;
	unsigned mtlIndex;
	triangleInd tempTriangle;
	unsigned kIndex;
	objMode theMode;
	mtlMode theMtlMode;
	vector3f tempVector;
	unsigned tempVectorIndex;
	//vector3f tempFaceIndex;
	bool verbose;

	
public:

	std::vector<vector3f> vertices;
	std::vector<vector3f> normals;
	std::vector<material*> mtls;
	
	/// Which opengl call list
	int listNum;
	
	std::vector<vector3f> iStack;

	/// the parts of iStack that are on the surface
	std::vector<vector3f> surface;

	vector3f centerOfMass;
	bool translucent;
	
	
	/// Used for finding MoI
	std::vector<vector3f> upperI;
	std::vector<vector3f> lowerI;

};


#endif

