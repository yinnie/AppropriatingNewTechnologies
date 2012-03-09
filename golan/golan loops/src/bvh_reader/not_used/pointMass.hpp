#ifndef weight_H
#define weight_H


#include "misc.hpp"

#include <list>

/// Generally attached to any number of springs.
/**

  The springs are updated first based on the masses location, and then the masses
  are updated with the locations set by the springs forces in the last time step.  The
  forces on the spring then are used to compute the newLocations.

  It's possible the springs should take account of the newLocations rather than the current 
  (which will just be updated to the contents of newLocation in the same timestep).  This
  may fix some stability problems at the cost of less springlike behaviour (due to more 
  instantaneous spring action).

 */
class pointMass
{
public:

	pointMass();
	pointMass(float DRAG);
	
	~pointMass();

	float mass;

	/// Summed up force vectors
	vector3f force;	
	/// The particle's current position
	vector3f location;
	/// The particle's current velocity
	vector3f velocity;		
	/// The particle's position in the next time step
	vector3f newLocation;		


	/// Update location based on forces and current velocity
	void update(void);


	//vector3f *matchedVelocity;  //

	// Later there will be more that one possible thing to 
	// collide with, and an overarching system to test collisions
	// but for now just check with map.
	//surface *map;

	void collisionTest(void);


	/// Void pointers (to avoid circular includes) to attached springs 
	std::list<void*> springs; 

	// primary constants

	float DRAG;

	///////////////////

	/// See if a line intersects a plane determine by a normal and a point.
	static float planeTest( vector3f &location, vector3f &newLocation,
					vector3f &triangleVertex, vector3f &surfaceNorm );
	/// See if a point on a plane lies within a triangle on that plane.
	static float triangleTest(vector3f &intersection, 
					vector3f &tri1, vector3f &tri2, vector3f &tri3);

private:

	void init();

	/// React to a collision, bounce, cancel forces, etc.
	void collisionResponse(vector3f *intersection, vector3f *tri3, vector3f *surfaceNorm);
};

#endif

