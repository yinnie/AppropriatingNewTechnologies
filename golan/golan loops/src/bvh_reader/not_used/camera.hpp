#ifndef camera_H
#define camera_H

#include "movable.hpp"
#include <string>

/// Hold location, orientation, and mode of the viewer
/**
	Also some experimental modes of movement 
	like following a given object, and following that
	objects every rotation, same with a delay, or just tracking
	the object without rotating with it.
  */
class camera:public movable
{
	public:
	enum mode { FREE, FREEORIENTED, CENTERED, CENTERORIENTED, FOLLOW, DELAY  };

	camera(void);
	
	// prototype in movable prevents lookMode= from working
	camera(string name, mode lookMode = FREE);
	~camera();

	void init();

	void draw(void);
	void update(void);
	void move(int pitch,int turn, int roll,float x, float y, float z);
	void look(void);

	float radius;

	//matrix16f location;

	/// the most recent center locations, the camera follows the last.
	matrix16f delay[20];
	/// extra matrix needed for follow mode
	matrix16f other;
	/// Location for camera to spin around
	matrix16f *center;
	
	vector3f centerPoint;
	
	vector3f angles;
	
	mode lookMode;
	
};

#endif
