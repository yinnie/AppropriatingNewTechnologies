#ifndef triangle_h
#define triangle_h

#include "misc.hpp"

class triangleInd {
public:
	triangleInd();
	~triangleInd();

	int vertIndices[3];   
	vector3f normal;
	bool collision;  // temp visual debugging member
	
	triangleInd& operator= (const triangleInd &tri);
};

#endif // triangle_h
