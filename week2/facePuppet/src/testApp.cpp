#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	cam.initGrabber(640, 480);
    videoThumb = false;
	
	destTracker.setup();

    //face tracking on source
    srcTracker.setup();
    srcTracker.setIterations(25);
    srcTracker.setAttempts(2);
    srcTracker.setClamp(4);
    srcTracker.setTolerance(.01);
    
    srcimg.loadImage("0.jpg");
    srcTracker.update(toCv(srcimg));
    srcPosition = srcTracker.getPosition();
    srcScale = srcTracker.getScale();
    srcOrientation = srcTracker.getOrientation();
    srcRotationMatrix = srcTracker.getRotationMatrix();
    srcPoints = srcTracker.getImagePoints();

}

void testApp::update() {
    //face tracking on camera being update every second
	cam.update();
	if(cam.isFrameNew()) {
		destTracker.update(toCv(cam));
		position = destTracker.getPosition();
		scale = destTracker.getScale();
		orientation = destTracker.getOrientation();
		rotationMatrix = destTracker.getRotationMatrix();
	}
}

void testApp::draw() {
	ofSetColor(255);
    srcimg.draw(0,0);
    srcTracker.getObjectMesh().drawWireframe();
    //srcTracker.draw();
    
    
    if(videoThumb){
        cam.draw(0, 0, 128, 96);
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    }
    
    if(destTracker.getFound()) {
        ofSetLineWidth(1);
        destTracker.draw();
        
        
        //get face points and map it onto source image
        ofMesh camMesh = destTracker.getObjectMesh();
        camMesh.clearTexCoords();
        camMesh.addTexCoords(srcPoints);
        
        srcimg.getTextureReference().bind();
        ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
        ofTranslate(srcPosition.x, srcPosition.y);
        applyMatrix(srcRotationMatrix);
        ofScale(srcScale,srcScale,srcScale);
        ofDrawAxis(scale);
        camMesh.draw();
        srcimg.getTextureReference().unbind();
        
        
        //draw inside of mouth
        mouth = destTracker.getObjectFeature(ofxFaceTracker::INNER_MOUTH);
        mouth.setClosed(true);
        ofPushStyle();
        ofFill();
        ofSetColor(60,0,0);
        ofBeginShape();
        for(int i =0; i<mouth.size(); i++){
            ofVertex(mouth[i]);
        }
        ofEndShape(true);
        mouth.draw();
        ofPopStyle();
    }

    
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		destTracker.reset();
	}
    if(key == 't') {
		videoThumb = !videoThumb;
	}
    switch (key) {  
        case '1':  
            srcimg.loadImage("0.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;  
        case '2':  
            srcimg.loadImage("1.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break; 
        case '3':  
            srcimg.loadImage("2.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break; 
        case '4':  
            srcimg.loadImage("3.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;
        case '5':  
            srcimg.loadImage("4.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;
        case '6':  
            srcimg.loadImage("5.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;
        case '7':  
            srcimg.loadImage("6.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;
        case '8':  
            srcimg.loadImage("7.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;
        case '9':  
            srcimg.loadImage("8.jpg");
            srcTracker.update(toCv(srcimg));
            srcPosition = srcTracker.getPosition();
            srcScale = srcTracker.getScale();
            srcOrientation = srcTracker.getOrientation();
            srcRotationMatrix = srcTracker.getRotationMatrix();
            srcPoints = srcTracker.getImagePoints();
            break;
        default:  
            break;  
    }  
}