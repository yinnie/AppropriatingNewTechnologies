#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	ofImage fly;
	
	float x;
	float y;
	float xoff;
	float xincrement;
	int counter;
	int interval;
	bool inside;
	bool doMoveFly;
		
	
};
