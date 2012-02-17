#include "ofApp.h"

ofVec3f ofApp::getField(ofVec3f position) {
	float normx = ofNormalize(position.x, 0, width);
	float normy = ofNormalize(position.y, 0, height);
	float u = ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
	float v = ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
	return ofVec3f(u, v);
}

ofVec3f getVertexfromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 0) {  // the alpha value encodes depth, let's remap it to a good depth range
		float z = ofMap(color.a, 0, 255, -480, 480);
		return ofVec3f(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}



void ofApp::setup() {
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofImage img;
	img.loadImage("out6.png");
	ofSetFrameRate(20);
	
	width = ofGetWidth();
	height = ofGetHeight();
	
	complexity = 8; // wind complexity
	maxMass = 0.8; // max pollen mass
	timeSpeed = 0.02; // wind variation speed
	phase = TWO_PI; // separate u-noise from v-noise
	windSpeed = 40; // wind vector magnitude for debug
	step = 10; // spatial sampling rate for debug
	debugMode = false;
	
	
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for(int y = 0; y < img.getHeight(); y++) {
		for(int x = 0; x < img.getWidth(); x++) {
			ofColor cur = img.getColor(x, y);
			if(cur.a > 180) {
				// the alpha value encodes depth, let's remap it to a good depth range
				float z = ofMap(cur.a, 0, 255, -480, 480);
				cur.a = 255;
				mesh.addColor(cur);
				ofVec3f pos(x - img.getWidth()/2, y - img.getHeight()/2, z);
				
				mesh.addVertex(pos);
			}
		}
	}
	// even points can overlap with each other, let's avoid that
	glEnable(GL_DEPTH_TEST);
	
	// make the points bigger, otherwise it's hard to see them
	glPointSize(1);
	
	meshCopy = mesh; 
	nPoints = mesh.getNumVertices(); // points to draw
	
	points.resize(nPoints);
	pollenMass.resize(nPoints);
	//selectedIndices.resize(nPoints);

			
	for(int i = 0; i < nPoints; i++) {
		//selectedIndices[i] = int(ofRandom(float(numIndices)));
		//points[i]= mesh.getVertex(selectedIndices[i]);
		points[i]= mesh.getVertex(i);
		cout << points[i] <<endl;
		pollenMass[i] = ofRandom(0, maxMass);
	}
	 
	ofBackground(0,0,0);
	//mesh.clear();
}

void ofApp::update() {
	t = ofGetFrameNum() * timeSpeed;
} 

void ofApp::draw() { 	
	if(debugMode) {
		ofBackground(255);
		ofSetColor(0);
		float s = windSpeed;
		for(int i = 0; i < width; i += step) {
			for(int j = 0; j < height; j += step) {
				ofVec2f field = getField(ofVec2f(i, j));
				ofPushMatrix();
				ofTranslate(i, j);
				ofLine(0, 0, ofLerp(-windSpeed, windSpeed, field.x), ofLerp(-windSpeed, windSpeed, field.y));
				ofPopMatrix();
			}
		}   
		ofSetColor(255, 0, 0);
	} else {
		ofSetColor(0, 10);
	}
	

	for(int i = 0; i < nPoints; i++) {
		
		float& x = points[i].x, y = points[i].y, z = points[i].z;
		ofVec3f field = getField(points[i]);
		float speed = (1 + ofNoise(t, field.x, field.y)) / pollenMass[i];
		x += ofLerp(-speed, speed, field.x);
		y += ofLerp(-speed, speed, field.y);
		
		if(x < 0 || x > width || y < 0 || y > height) {
			x = ofRandom(0, width);
			y = ofRandom(0, height);
		}
		
		if(debugMode) {
			ofCircle(x, y, 3);
		} else {
			points[i].x = x;
			points[i].y = y;
		    mesh.setVertex(i, points[i] );

		}			
		
	}
	
	if(!debugMode) {
	
		ofBackground(0);
		cam.begin();
		ofScale(1, -1, 1); // make y point down
		mesh.draw();
		//meshCopy.draw();
		cam.end();
	}	
	
}

/*
void ofApp::mousePressed(int x, int y, int button) {
	setup();
}

void ofApp::keyPressed(int key) {
	debugMode = !debugMode;
	ofBackground(255);
}
*/