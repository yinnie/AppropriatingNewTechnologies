#ifndef rigid_H
#define rigid_H

#include <vector>
#include <string>

#include "movable.hpp"
#include "objloader.hpp"
#include "misc.hpp"
#include "light.hpp"

/// Hold rigid body data, use objloader to load obj's
/**
I thought this would be more memory efficient, to have
specialized classes for loading and holding rigid body data.

*/
class rigid:public movable
{
public:
	rigid();
	rigid(string objFile);
	~rigid();
	
	/// Virtual generic draw member, calls makeList
	void draw(void);
	/// Adjust light intensity per-vertex
	void drawDim(vector<light*> lights);
	/// Does nothing currently, put scripted movements here
	void update(void);

	/// Currently not using display lists, this just draws each triangle
	void makeList(void);
	
	/// Virtual member that finds the extrema of all the vertices
	void getBoundingBox(void);

	/// For scripted movements, unused currently
	int counter;

private:
	///temp
	objloader *theObj;
	
	/// All surface vertices, independent of triangles they are part of.
	std::vector<vector3f> vertices;
	/// Normals for each vertice, same index should be corresponding vertice index
	std::vector<vector3f> normals;
	/// The mtls hold material lighting information as well as indices of vertices in triangles
	std::vector<material*> mtls;
	
	/// Set by findCenterOfMass in objloader
	vector3f centerOfMass;

	/// Which opengl call list, unsupported currently
	int listNum;
	
	/// Toggles drawing of the surface triangles
	bool drawSurface;
	/// This is likely obsolete and should be gotten rid of
	bool translucent;
};
#endif //rigid_h
