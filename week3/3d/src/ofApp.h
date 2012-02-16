#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	ofEasyCam cam;
	
	int nPoints; // points to draw
	float complexity; // wind complexity
	float maxMass; // max pollen mass
	float timeSpeed; // wind variation speed
	float phase; // separate u-noise from v-noise
	
	float windSpeed; // wind vector magnitude for debug
	int step; // spatial sampling rate for debug
	
	vector<float> pollenMass; //we only want to work with vector<> arrays in OF
	vector<ofVec2f> points;
	
	bool debugMode;
	
	float width, height;
	
	ofMesh mesh;
	ofMesh pollenMesh;
	
	ofImage img;
	
};
