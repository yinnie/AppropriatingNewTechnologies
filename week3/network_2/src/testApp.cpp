#include "testApp.h"

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
}

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
	//addFace(mesh, b, c, d);
}

ofVec3f getVertexFromImg(ofImage& img, int x, int y) {
	ofColor color = img.getColor(x, y);
	if(color.a > 200) {  //adjust to get a good image
		float z = ofMap(color.a, 200, 255, -480, 480); //mapping alpha to a good range
		return ofVec3f(x - img.getWidth() /2, y - img.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}


void testApp::setup(){
	
	//set up img mesh
	ofSetVerticalSync(true);  //?????
	ofImage img;
	img.loadImage("out8.png");
	meshImg.setMode(OF_PRIMITIVE_TRIANGLES);
	
	int skip = 3;	
	int width = img.getWidth();
	int height = img.getHeight();
	ofVec3f zero(0, 0, 0);
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			ofVec3f nw = getVertexFromImg(img, x, y);
			ofVec3f ne = getVertexFromImg(img, x + skip, y);
			ofVec3f sw = getVertexFromImg(img, x, y + skip);
			ofVec3f se = getVertexFromImg(img, x + skip, y + skip);
			if(nw != zero && ne != zero && sw != zero && se != zero) {
				addFace(meshImg, nw, ne, se, sw);
			}
		}
	}	
	// even points can overlap with each other, let's avoid that
	glEnable(GL_DEPTH_TEST);
	
	
	//------------------------------------------------------------------
	//set up network mesh
	meshNetwork.setMode(OF_PRIMITIVE_POINTS);
	numPoints=100;
	numLoops = 100;
	counter=0;
	phase = PI*2;
	
	points.resize(numPoints);  //do this to initiate the vector array
	
	//initiate the points 
	for (int i=0; i< numPoints; i++) {		
		points[i] = getVertexFromImg(img, ofRandom(width), ofRandom(height));
	}
	//cout << ofToString(points) << endl;
}


void testApp::update(){ 
	
	if (counter<=100) {
		
		t = ofGetFrameNum();
		//move all points randomly and save these to new pos
		for (int i=0; i< numPoints; i++) {
			float ofNormalize(points[i].x, 0, width);
			float ofNormalize(points[i].y, 0, height);
			float u = ofNoise 
			
			
			
		}
		
				counter++;
	}
}


void testApp::draw(){
	
	ofBackground(0);
	cam.begin();
	ofScale(1, -1, 1); // make y point down
	//meshNetwork.addColor(ofColor red);
	ofSetColor(255);
	meshNetwork.draw();
	//cout << ofToString(meshNetwork) << endl;
	//meshImg.addColor(ofColor white);
	meshImg.drawWireframe();
	cam.end();
}
