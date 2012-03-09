#include "movieScene.h"

void movieScene::setup(){
	ofBackground(255,255,255);
	movie1.loadMovie("clip1.mov");
    movie2.loadMovie("clip2.mov");
    movie3.loadMovie("clip3.mov");
    movie4.loadMovie("clip4.mov");
   // movie5.loadMovie("clip5.mov");
//    movie6.loadMovie("clip6.mov");
//	 
	movie1.play();
	movie2.play();
    movie3.play();
    movie4.play();
   // movie5.play();
//    movie6.play();
	
	
}


void movieScene::update(float mouseX, float mouseY) {
	xPos = mouseX;
	yPos = mouseY;
	
	movie1.idleMovie();
	movie2.idleMovie();
    //movie3.idleMovie();
    movie4.idleMovie();
    //movie5.idleMovie();
//    movie6.idleMovie();
//	 
	
	
	if(xPos>0 && xPos< 400 && yPos> ofGetWindowHeight()/2 && yPos < ofGetWindowHeight()) {
		state=1;  //Gabby
	}
	/*
	if (xPos>600 && xPos< 600 && yPos>300 && yPos < 500) {
		state =2;  //Peter
	}
	*/
	if (xPos>760 && xPos< 820 && yPos>300 && yPos < 600) {
		state =3;  //mehan
	}
	/*
	else if (xPos>870 && xPos< 1090 && yPos>300 && yPos < 500) {
		state =4;  //Mick
	}
	else if (xPos>1090 && xPos< 1280 && yPos>300 && yPos < 500) {
		state =5;  //kaitlin
	}
	 
	else {
		state =4;
	}
	 */
		
}


void movieScene::draw() {
	ofSetHexColor(0xFFFFFF);  //this line is important!! otherwise movies dont draw properly
	
	
	
	switch (state) {
		case 1:
			movie2.draw(0,0,1280,800);
			break;
			/*
		case 2:
			movie3.draw(0,0,1280,800);
			break;
			 */
		case 3:
			movie4.draw(0,0,1280,800);
			break;
			/*
		case 4:
			movie5.draw(0,0,1280,800);
			break;
		case 5:
			movie6.draw(0,0,1280,800);
			break;
			 */
		default:
			movie1.draw(0,0,1280,800);
			break;
	}
	
}


