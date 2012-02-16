#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	cam.initGrabber(640, 480);
	
	tracker.setup();
	tracker.setRescale(.5);
	
	fly.loadImage("fly.png");
	
	ofEnableAlphaBlending();
	
	x=ofGetWidth()/3;
	y=0;
	xoff=0.0;
	xincrement = 0.01;
	interval = 20;
	doMoveFly = false;
}

void testApp::update() {

	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}
	
		if(ofRandom(100.0) < 10.0) {
			doMoveFly = !doMoveFly; 
			//probability that the fly moves at each loop
		}
		if(doMoveFly) {
		    
			y++;
			if (y>ofGetHeight()) {
				y=0;
			}
		
			x= ofNoise(xoff)*ofGetWidth();
			xoff+=xincrement;
			
			if (x<0 || x > ofGetWidth()) {
				x=ofGetWidth()*0.5;
			}
		}
}

void testApp::draw() {
	
    ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	ofSetLineWidth(1);
	tracker.draw();
	
	fly.draw(x, y, 20, 20);
	
	ofMesh morphMesh = tracker.getImageMesh();
	
	if (tracker.getFound()) {
		
		//make an array of features
		int features [] = {ofxFaceTracker::OUTER_MOUTH,ofxFaceTracker::INNER_MOUTH,ofxFaceTracker::EYE_LEFT,ofxFaceTracker::EYE_RIGHT,ofxFaceTracker::MIDDLE,ofxFaceTracker::CHEEK_LEFT,ofxFaceTracker::CHEEK_RIGHT,ofxFaceTracker::NOSE,ofxFaceTracker::CHIN};
			
		//loop through all the features
		for (int i=0; i<features.size(); i++) {
			
			//using ofPolyline.inside(x,y) function to check if the fly is in any of the features
		     if (tracker.getImageFeature(features[i]).inside(x, y)) {
				 
			    //get the indices of that feature polyline
				 vector<int> indexVertices = tracker.getFeatureIndices (features<i>);
				 
				 //looping thorough indices in that feature
				 for (int q=0; q<indexVertices.size(); q++) {
					 int indexF = indexVertices [q];
					 ofVec3f cur = morphMesh.getVertex(indexF);
					 cur.x +=10;
					 cur.y +=10;
					 
					 //set these to be the new values of the mesh at those index positions
					 morphMesh.setVertex(indexF, cur);
				 }
			 }
		}
		
			cam.getTextureReference().bind();
			morphMesh.draw();
			cam.getTextureReference().unbind();
     }
}




void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}

