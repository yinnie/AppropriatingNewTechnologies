#ifndef BODY_HPP
#define BODY_HPP

#include <stack>

#include "movable.hpp"
#include "bvh.hpp"

class body : public movable
{
	/// Display lines, obloids, or .obj limbs?
	enum displayType {LINES,OBLOIDS,OBJ};

	void recursDraw(bvhPart *part);


	stack<matrix16f> orient;
	//stack<vector3f> joint;
public:
	body(string bvhFile);
	~body();

	/// which frame to show
	unsigned counter;
	int crement;

	displayType display;
	bvh *theBvh;
	void draw(void);
	void switchDisplay(void);
	//void drawDim(vector<light*> lights);
	void update(void);
	void update(int counterOffset);

};

#endif //BODY_HPP
