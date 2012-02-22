#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	cam.initGrabber(640, 480);
	ofEnableAlphaBlending();
	
	camTracker.setup();
	
	imageCounter =0;
	
	//face tracker on source
	srcTracker.setup();
	srcTracker.setRescale(.5);
	
	srcTracker.setup();
    srcTracker.setIterations(25);
    srcTracker.setAttempts(2);
    srcTracker.setClamp(4);
    srcTracker.setTolerance(.01);
    
	srcImg.loadImage("face.jpeg");
    srcTracker.update(toCv(srcImg));
    srcPoints = srcTracker.getImagePoints();

    	
	
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		camTracker.update(toCv(cam));
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	srcImg.draw(0,0, 100,100); //draw srcimg thumbnail
	
	if(camTracker.getFound()) {
		
		ofMesh camMesh = camTracker.getImageMesh();
		camMesh.clearTexCoords();
		camMesh.addTexCoords(srcPoints);
		
		/*
		ofMesh imageMesh = tracker.getImageMesh();
		leftEye = tracker.getImageFeature(ofxFaceTracker::EYE_LEFT);
		mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
		 */
		
		srcImg.getTextureReference().bind();
		camMesh.draw();
		srcImg.getTextureReference().unbind();
		
		/*
		ofMesh imageMesh = camTracker.getImageMesh(); 
		//move the mesh away
		int x = 100;
		ofPushMatrix();
		ofTranslate(x,0);
		imageMesh.draw();
		ofPopMatrix();
		
		*/
		
	    mouth.setClosed(true);
        ofPushStyle();
        ofFill();
        ofSetColor(60,0,0);
        ofBeginShape();
        for(int i =0; i<mouth.size(); i++){
            ofVertex(mouth[i]);
        }
        ofEndShape(true);
        ofPopStyle();  
		mouth.draw();
	}
	
		
	/* distort the mouth 
	ofSetLineWidth(1);
	tracker.draw();
	
	ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
	ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
	ofPolyline faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
	
	ofSetLineWidth(2);
	ofSetColor(ofColor::red);
	leftEye.draw();
	ofSetColor(ofColor::green);
	rightEye.draw();
	ofSetColor(ofColor::blue);
	faceOutline.draw();
	
	 if(tracker.getFound()) {
	 ofSetColor(255);
	 bool inside = faceOutline.inside(mouseX, mouseY);
	 ofDrawBitmapString(inside ? "inside" : "outside", 10, 40);
	 }

	*/

}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		camTracker.reset();
	}
	
	if (key== ' ') {
		srcImg.loadImage(ofToString(imageCounter-1) + ".png");
		srcTracker.update(toCv(srcImg));
	    srcPoints = srcTracker.getImagePoints();
		
	}
	
	if (key == 's') {
		srcImg.grabScreen(110,0,ofGetWindowWidth(),ofGetWindowHeight());
		srcImg.saveImage(ofToString(imageCounter) + ".png");
		imageCounter ++;
	}
	
	
	
}

