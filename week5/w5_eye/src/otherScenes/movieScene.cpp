/*
 *  movie.cpp
 *  EyeOSC
 *
 *  Created by Yin Liu on 3/8/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "movieScene.h"

void movieScene::setup(){
	ofBackground(0);
	movie1.loadMovie("test.mov");
	movie2.loadMovie("test2.move");
	
}


void movieScene::update(float mouseX, float mouseY) {
	xPos = mouseX;
	yPos = mouseY;
	movie1.idleMovie();
	movie2.idleMovie();
	
}


void movieScene::draw() {
	
	
	int top= ofGetWindowHeight()/4;
	int bottom = ofGetWindowHeight()/2;
	int left = ofGetWindowWidth()/4;
	int right = ofGetWindowWidth()/2;
	
	//if(xPos < right && xPos> left && yPos> top && yPos <bottom) {
	if(xPos < right) {

		movie2.play();
		movie2.draw(0,0, 320,240);
	}
	else if (xPos > right) {
		movie1.play();
		movie1.draw(320,0, 320, 240);
	}

	
}


