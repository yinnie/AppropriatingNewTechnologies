#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
	int numPoints;
	float dist; //distance
	int numLoops;
	int counter;
	
	
	vector<ofVec3f> points;
    ofVec3f	movement; //movement every loop
	ofMesh meshNetwork; 
	ofMesh meshImg; 
	ofEasyCam cam;
	
	float phase; 
	
	
	

				
};
