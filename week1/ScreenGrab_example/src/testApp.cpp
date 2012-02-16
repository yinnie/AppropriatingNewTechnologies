#include "testApp.h"

extern "C" {
#include "macGlutfix.h"
}

//--------------------------------------------------------------
void testApp::setup(){
	
	finder.setup("haarcascade_frontalface_default.xml");
	//CGContextRef cgctx = NULL;
	//ofSetVerticalSync(true);
	//tex.allocate(300,300, GL_RGBA);
	image.allocate(300, 300, OF_IMAGE_COLOR);
	//pixels.allocate(300, 300, OF_IMAGE_COLOR);
	
	ofSetFrameRate(30);

}

//--------------------------------------------------------------
void testApp::update(){
	
	int w = 300;
	int h = 300;
	
	unsigned char * data = pixelsBelowWindow(ofGetWindowPositionX(),ofGetWindowPositionY(),w,h);
	
	// now, let's get the R and B data swapped, so that it's all OK:
	for (int q = 0; q < w*h; q++){
		
		unsigned char alpha = data[q*4]; // each index is in fact referring to only the r channel in a pixel  
		
		data[q*4]   = data[q*4+1];   
		data[q*4+1] = data[q*4+2];   
		data[q*4+2] = data[q*4+3];   
		data[q*4+3] = alpha; 
	}
	
	
	if (data!= NULL) {
		//tex.loadData(data, 300, 300, GL_RGBA);
		//tex.readToPixels(pixels);
		//image = pixels;
		image.setFromPixels(data, 300, 300, OF_IMAGE_COLOR_ALPHA, true);
		image.setImageType(OF_IMAGE_COLOR);
		image.update();
		finder.findHaarObjects(image.getPixelsRef());

		
		for (int x=0; x< w; x++) {
			for (int y=0; y<h; y++) {
				
				int index = x+w*y;
				
						
				for (int i=0; i< finder.blobs.size(); i++) {
					
				
   
					if (x >finder.blobs[i].boundingRect.x && x <(finder.blobs[i].boundingRect.x +finder.blobs[i].boundingRect.width)&& y> finder.blobs[i].boundingRect.y && y < (finder.blobs[i].boundingRect.y +finder.blobs[i].boundingRect.height)) {
					
						/*
						unsigned char r = data[index*4];
						unsigned char  g = data[index*4+1];
						unsigned char  b = data[index*4+2];
						unsigned char  a = data[index*4+3]; 
						 */
						
						int dividingRow1 = finder.blobs[i].boundingRect.y +int(finder.blobs[i].boundingRect.height*0.25);
						int dividingRow2 = finder.blobs[i].boundingRect.y +int(finder.blobs[i].boundingRect.height*0.5);
						int dividingRow3 = finder.blobs[i].boundingRect.y +int(finder.blobs[i].boundingRect.height*0.75);
		
						
						int offset =int (w*int(finder.blobs[i].boundingRect.height*0.25)*4); 
						//this doesn't work..but changing rect.height to absolute number works
						
						//int offset = w*20*4; // width of the ENTIRE image * number of rows * number of channels
						
						
						
					     if (y< dividingRow1){
			
							data[index*4]  = data[index*4+ offset];  
							data[index*4+1]  = data[index*4+ offset +1]; 
							data[index*4+2]  = data[index*4+ offset +2]; 
							data[index*4+3]  = data[index*4+ offset +3]; 
						
						}
						 
					
						   if (y> dividingRow2 &&  y< dividingRow3 ) {
							
						    data[index*4]  = data[index*4-offset];  
							data[index*4+1]  = data[index*4- offset +1]; 
							data[index*4+2]  = data[index*4- offset +2]; 
							data[index*4+3]  = data[index*4- offset +3]; 
							
						}
					/*	
						else if (y >dividingRow3 && y < (finder.blobs[i].boundingRect.height+finder.blobs[i].boundingRect.y) ) { 
							data[index*4]    = data[index*4 - offset*2] ;
							data[index*4+1]  = data[index*4- offset*2+1]; 
							data[index*4+2]  = data[index*4- offset*2+2]; 
							data[index*4+3]  = data[index*4- offset*2+3]; 
							
						}
					 
					 */
				 
					}
				}
			}
		}
	}
	
	image.setFromPixels(data, 300, 300, OF_IMAGE_COLOR_ALPHA, true);
	image.setImageType(OF_IMAGE_COLOR);
	image.update();
	finder.findHaarObjects(image.getPixelsRef());
	
												
				
}

//--------------------------------------------------------------
void testApp::draw(){
	image.draw(0,0, ofGetWidth(), ofGetHeight());
	
	ofNoFill();

	
	//for each face "blob" we found, draw a rectangle around the face
    //#2
	for(int i = 0; i < finder.blobs.size(); i++) {
		ofRect(finder.blobs[i].boundingRect);
//		ofFill(255, 0, 0);
//		ofEllipse((finder.blobs[i].boundingRect.x+0.5*finder.blobs[i].boundingRect.width), (finder.blobs[i].boundingRect.y+0.5*finder.blobs[i].boundingRect.height), 5, 5);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}