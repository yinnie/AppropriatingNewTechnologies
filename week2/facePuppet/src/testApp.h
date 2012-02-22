#pragma once

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker destTracker;
	ofVec2f position;
	float scale;
	ofVec3f orientation;
	ofMatrix4x4 rotationMatrix;
	
	Mat translation, rotation;
	ofMatrix4x4 pose;
	
	ofEasyCam easyCam;

    bool videoThumb;
    
    //src image to modify
    ofImage srcimg;
    ofxFaceTracker srcTracker;
	ofVec2f srcPosition;
	float srcScale;
	ofVec3f srcOrientation;
	ofMatrix4x4 srcRotationMatrix;
    vector<ofVec2f> srcPoints;
    
    ofPolyline mouth;

};
