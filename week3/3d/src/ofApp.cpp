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
	//ofBackground(0, 0, 0);
	ofEnableAlphaBlending();
	width = ofGetWidth();
	height = ofGetHeight();
	
	//pulling in the depth image
	img.loadImage("linzer.png");
	
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for (int i=0; i< img.getWidth(); i++) {
		for (int j=0; j<img.getHeight(); j++) {
			ofVec3f pos = getVertexfromImg(img, i, j);
			ofVec2f pos2D = get2DVertexfromImg(img, i, j);
			mesh.addVertex(pos);
			pollenMesh.addVertex(pos2D);
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
	
	float width, height;
	
    points.resize(nPoints);   //initializing the vector array
	pollenMass.resize(nPoints);
	
	for(int i = 0; i < nPoints; i++) {
		points[i] = get2DVertexfromImg(img, ofRandom(0, img.getWidth()), ofRandom(0, img.getHeight())); //get X and Y from the image
		cout << points[i]<< endl;
		pollenMass[i] = ofRandom(0, maxMass);
	}
	//noiseDetail(14);
	
}

void ofApp::update() {
} 

void ofApp::draw() { 
	float t = ofGetFrameNum() * timeSpeed;
	
	//ofMesh mesh;   //if mesh is initiated here..it gets restarted every loop
	pollenMesh.setMode(OF_PRIMITIVE_POINTS);
	for(int i = 0; i < nPoints; i++) {
		float x = points[i][0]; 
		//cout << x <<endl;
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

		
		    //pollen.x=x;
//			pollen.y=y;
//			ofVec3f pollen_=getVertexfromImg(img, x, y);
//			pollen.z=pollen_.z;
//			pollenMesh.addVertex(pollen); 
//	
			
		points[i].x = x;
		points[i].y = y;
	}
	
	ofBackground(0);
	cam.begin();
	ofScale(1, -1, 1); // make y point down
	//mesh.draw();
	//ofSetColor(255);
	pollenMesh.draw();
	cam.end();
	}

void ofApp::mousePressed(int x, int y, int button) {
	setup();
}


