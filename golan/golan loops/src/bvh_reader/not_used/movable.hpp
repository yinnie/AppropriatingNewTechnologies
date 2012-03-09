#ifndef movable_H
#define movable_H

#include <string>
/*
#include <Panel.h>
#include <FrameWindow.h>
#include <CheckBox.h>
#include <ActionMessage.h>
//#include "buttonListeners.h"
*/
#include "misc.hpp"



#define ROTATE_SLOWNESS 8
#define MOVE_INC  1.4f

using namespace std;

/// Mostly virtual class for any entity in the scene
/**
	
	It holds a lot of physical information that would be
	better off in a subclass where it would make sense, as
	lights and camera don't really need such properties.

  */
class movable
{
public:

	movable();
	movable(string name);
	virtual ~movable();

	void init();

	movable& operator= (const movable& other);

	virtual void draw();
	virtual void update();

	void move(int pitch, int turn, int roll, float x, float y, float z);

	void setName(string name);


	/// physical constants


	float mass;
	/// moment of inertia tensor
	matrix9f Ibody;
	/// inverse of moi tensor
	matrix9f IbodyInv; 

	/// state variable
	matrix16f location,		// Rotation + location
			  newLocation;

	/// momentum
	vector3f P,newP, 
	/// angular momentum
			 L,newL; 

	
	// derived
	matrix9f Iinv;
	/// velocity of center of mass
	vector3f velocity,
	/// w angular velocity
			 omega;		

	/// summed forces per timestep 
	vector3f force,
	/// summed torque per timestep
			 torque;


	bool normalize;

	float step;
	

	string name;

	
	//bounding box
	virtual void getBoundingBox();
	void getAABB();
	void drawAABB();
	void drawBoundingBox();

	bool drawBB;

	// has this been completely tested for collisions?
	bool BBtested;

	bool BBcollided;

	vector3f centerBB;
	vector3f edgesBB;
	vector3f boundingBox[8];
	
	vector3f oldCenterAABB;
	vector3f centerAABB;
	vector3f edgesAABB;
	vector3f AABB[8];
	
	//MovableType movableType;

	/*
	friend bool operator< (const movable& other);
	friend bool operator== (const movable& other);
	*/

	// is the object moved normally or is
	// it subject to the physics engine?
	bool physical;

private:	
/*
///////////////////////
// GLG stuff
public:
	
	Panel *movablePanel;
	FrameWindow *movableFrame;

	CheckBox *bbBox;

	CheckBoxListener *bbBoxListener;

*/
};

#endif // movable_H
