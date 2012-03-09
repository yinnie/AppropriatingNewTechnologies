#ifndef BVH_HPP
#define BVH_HPP

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "misc.hpp"

using namespace std;

class ChannelError;

// how should the animation info fit in?
// per part, or globally?
class bvhPart
{

public:
	enum channelTypes {Xpos,Ypos,Zpos,Zrot,Xrot,Yrot};

	bvhPart();

	string name;

	vector3f offset;

	vector<matrix16f> motion;



	bvhPart* parent;
	vector<channelTypes> channels;
	vector<bvhPart*> child;
};


class bvh
{
	bool verbose;
	enum mode {NONE,OFFSET,CHANNELS,JOINT,ROOT,End,Site,MOTION,Frames,Frame,Time,MOTIONDATA};

public:
	bvh(string bvhFile);
    void init(string bvhFile);
	int framesNum;

	void recurs (bvhPart* some);
	void process(string line);



	bvhPart *root;
	float frameTime;

private:

	// mostly used just for init/processing- what to do?
	// Have another class with just essential data structures
	// that uses a processing class and then deletes it- more mem efficient
	bvhPart *current;
	vector <bvhPart*> bvhPartsLinear;

       mode theMode;
	int vertIndex;

	unsigned channelIndex;
	unsigned partIndex;



	int data;
	/// the channels vector will store it's own size, so this is just for error checking.
	unsigned channelsNum;


	matrix16f tempMotion;
	matrix16f tempMotionY;
	matrix16f tempMotionX;
	matrix16f tempMotionZ;



};

#endif //BVH_HPP
