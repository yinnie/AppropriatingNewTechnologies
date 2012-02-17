#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofEasyCam cam;
	ofMesh mesh;
	
	//ofVboMesh mesh;  //this is much faster becoz vbo mesh only gets drawn once
};
