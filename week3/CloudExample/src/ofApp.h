#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam cam;
	ofMesh mesh;
	ofImage img1;
	ofImage img2;
	
	void getMeshfromImage(ofImage img);
	
};
