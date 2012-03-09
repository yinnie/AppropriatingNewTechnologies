/*
 *  movie.h
 *  EyeOSC
 *
 *  Created by Yin Liu on 3/8/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "baseScene.h"


class movieScene : public baseScene{
	
public:
	
	void setup();
	void update(float mouseX, float mouseY);
	void draw();
	
   
    ofVideoPlayer movie1, movie2,movie3,movie4,movie5,movie6;
	float xPos;
	float yPos;
	int state;
	
};

