#include "ofApp.h"


ofVec3f getVertexfromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 0) {  // the alpha value encodes depth, let's remap it to a good depth range
		float z = ofMap(color.a, 0, 255, -480, 480);
		return ofVec3f(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}

ofVec2f get2DVertexfromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 0) { //making sure we get good alpha data..we don't want black points
		return ofVec2f(x - img.getWidth() / 2, y - img.getHeight() / 2);
	} else {
		return ofVec2f(0, 0);
	}
}
	

void ofApp::setup() {
	ofSetVerticalSync(true);
	//ofSetBackgroundAuto(false);
	ofEnableAlphaBlending();
	width = ofGetWidth();
	height = ofGetHeight();
	
	//pulling in the depth image
	img.loadImage("linzer.png");
	
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for (int i=0; i< img.getWidth(); i++) {
		for (int j=0; j<img.getHeight(); j++) {
			ofVec3f pos = getVertexfromImg(img, i, j);
			mesh.addVertex(pos);
		}
	}
	
	// even points can overlap with each other, let's avoid that
	glEnable(GL_DEPTH_TEST);
	
	// make the points bigger, otherwise it's hard to see them
	glPointSize(4);

	
	//---------------------------------------------------the pollen part
	//initiating the points
	int nPoints = 100; // points to draw
	float complexity = 6; // wind complexity
	float maxMass = .8; // max pollen mass
	float timeSpeed = .02; // wind variation speed
	float phase = TWO_PI; // separate u-noise from v-noise
	
	float windSpeed = 40; // wind vector magnitude for debug
	int step = 10; // spatial sampling rate for debug
	
	bool debugMode = false;
	
	float width, height;
	
    points.resize(nPoints);   //initializing the vector array
	pollenMass.resize(nPoints);
	
	for(int i = 0; i < nPoints; i++) {
		points[i] = get2DVertexfromImg(img, ofRandom(0, img.getWidth()), ofRandom(0, img.getHeight())); //get X and Y from the image
		pollenMass[i] = ofRandom(0, maxMass);
	}
	//noiseDetail(14);
	
}

void ofApp::update() {
} 

void ofApp::draw() { 
	float t = ofGetFrameNum() * timeSpeed;
	
	if(debugMode) {
		ofBackground(255);
		ofSetColor(0);
		float s = windSpeed;
		for(int i = 0; i < width; i += step) {
			for(int j = 0; j < height; j += step) {
				float normx = ofMap(i, 0, width, 0, 1);
				float normy = ofMap(j, 0, height, 0, 1);
				float u = ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
				float v = ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
				ofPushMatrix();
				ofTranslate(i, j);
				ofLine(0, 0, ofLerp(-windSpeed, windSpeed, u), ofLerp(-windSpeed, windSpeed, v));
				ofPopMatrix();
			}
		}   
		ofSetColor(255, 0, 0);
	} else {
		ofSetColor(0, 10);
	}
	
	//ofMesh mesh;   //if mesh is initiated here..it gets restarted every loop
	pollenMesh.setMode(OF_PRIMITIVE_POINTS);
	for(int i = 0; i < nPoints; i++) {
		float x = points[i][0];  //the "[]" syntax is the same for vector array and C array
		float y = points[i][1];
		
		float normx = ofNormalize(x, 0, width);
		float normy = ofNormalize(y, 0, height);
		float u = ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
		float v = ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
		float speed = (1 + ofNoise(t, u, v)) / pollenMass[i];
		x += ofLerp(-speed, speed, u);
		y += ofLerp(-speed, speed, v);
		
		if(x < 0 || x > width || y < 0 || y > height) {
			x = ofRandom(0, width);
			y = ofRandom(0, height);
		}
		
		if(debugMode) {
			ofCircle(x, y, 1);
		} else {	
			ofVec3f pollen;
			pollen.x=x;
			pollen.y=y;
			ofVec3f pollen_=getVertexfromImg(img, x, y);
			pollen.z=pollen_.z;
			pollenMesh.addVertex(pollen); 
		}
			
		points[i].x = x;
		points[i].y = y;
	}
	
	mesh.draw();
	ofSetColor(255);
	pollenMesh.draw();
}

void ofApp::mousePressed(int x, int y, int button) {
	setup();
}

void ofApp::keyPressed(int key) {
	debugMode = !debugMode;
	ofBackground(255);
}
