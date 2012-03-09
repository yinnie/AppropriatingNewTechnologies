#include "testApp.h"

// Todo: droplets fling off

//--------------------------------------------------------------
void testApp::setup(){
	counter = 0;
	counter2 = 0;
	fps = 0;
	motionBlur = 0;
	lastFrameTime = 0;
	handyString = new char[64];
	bDrawFPS = false;
	bDrawSkeleton = false;

    bMousePressed = false;
	mouseXBlur = ofGetScreenWidth() / 2;
	mouseYBlur = ofGetScreenHeight()/ 2;
	mouseXRotate = 0;



	screenW = ofGetScreenWidth();
	screenH = ofGetScreenHeight();
	nScreenPixels = screenW * screenH;
	shadowDivX = 8;
	shadowDivY = 8;
	shadowW = screenW/shadowDivX;
	shadowH = screenH/shadowDivY;
	nShadowPixels = shadowW * shadowH;
	shadowTex.allocate(shadowW,shadowH,GL_LUMINANCE_ALPHA);
	screenDepth  = new float[nScreenPixels];
	for (int i=0; i<nScreenPixels; i++){
	    screenDepth[i] = 0;
	}
	shadowPixels   = new unsigned char[nShadowPixels];
	shadowPixelsLA = new unsigned char[nShadowPixels*2];
	for (int i=0; i<nShadowPixels; i++){
        shadowPixels[i] = 255;
        shadowPixelsLA[i] = 255;
        shadowPixelsLA[i+nShadowPixels] = 255;
	}
	shadowCvImage.allocate(shadowW, shadowH);

    glutSetCursor(GLUT_CURSOR_NONE);
	ofSetWindowTitle("LOOPS");
	ofBackground(198,199,200);

	//ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    //ofSetVerticalSync(true);

    materialAmbient  = new float[4];
    materialDiffuse  = new float[4];
    materialSpecular = new float[4];
    for (int i=0; i<4; i++){
        materialAmbient[i]  = 1.0f;
        materialDiffuse[i]  = 1.0f;
        materialSpecular[i] = 1.0f;
    }

	//-----------------------
    string bvhFileL = ofToDataPath("lwrist.bvh.txt");
	string bvhFileR = ofToDataPath("rwrist.bvh.txt");
    theBvhL = new bvh (bvhFileL);
    theBvhR = new bvh (bvhFileR);

    nBvhFrames  = 819;
    nFloaters   = 0;
    nHandPoints = 21 * 2; // empirically... 2 files, 21 nodes each
	nPoints     = nHandPoints + nFloaters;
	partCount   = 0;

    rawBvhPoints   = new ofPoint[nPoints];
	ballPoints     = new ofPoint[nPoints];
	ballPointsPrev = new ofPoint[nPoints];
	ballPointsPrev2= new ofPoint[nPoints];
	ballMasses     = new float[nPoints];
	ballLevels     = new int[nPoints];
	for (int i=0; i<nPoints; i++){
	    rawBvhPoints[i].set(0.5,0.5,0.5);
	    ballPoints[i].set(0.0,0.0,0.0);
	    ballPointsPrev[i].set(0.0,0.0,0.0);
	    ballPointsPrev2[i].set(0.0,0.0,0.0);
	    ballMasses[i] = 1.0;
	    ballLevels[i] = 0;
	}
	// levels = 0,1,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3

	//-----------------------
	m_pMetaballs = new CMetaballs (nPoints);
	m_pMetaballs->SetGridSize( 120 );
	CMarchingCubes::BuildTables();

	theIdentityMatrix.LoadIdentity();
	orient.push(theIdentityMatrix);

    maxDeceleration = 0;
    decelerationThreshPct = 0.50;
    decelerationEventCount = 0;

	boundsMin.x = boundsMin.y = boundsMin.z =  9999999;
    boundsMax.x = boundsMax.y = boundsMax.z = -9999999;
    boundsAvg.x = boundsAvg.y = boundsAvg.z = 0;
    bHaveExaminedAllFrames = false;

    boundsScaling = 1.0 / 120.0f;

}




//--------------------------------------------------------------
void testApp::recursiveDraw (bvhPart* part, int level){

	matrix16f ident = orient.top() * part->motion[0];
	//if (part == theBvh->root)	part->motion[counter].print();

    if (true){//partCount != 0){
        ident.Translate(
            part->motion[counter].matrix[12],
            part->motion[counter].matrix[13],
            part->motion[counter].matrix[14]);
    }
    ident.Rotate(part->motion[counter]);  //rotate in place
	orient.push(ident);

    float px = ident.matrix[12];
    float py = ident.matrix[13];
    float pz = ident.matrix[14];
    rawBvhPoints[partCount].set(px,py,pz);
    ballLevels[partCount] = level;
    partCount++;

    bool bDoDraw = false;
	for (int i = 0; i < part->child.size(); i++) {
		vector3f offset = part->child[i]->offset;

        if (bDoDraw){
            glPushMatrix();
            glMultMatrixf(ident.matrix);
                glBegin(GL_POINTS);
                glVertex3f( 0,0,0 );
                glEnd();
                glBegin(GL_LINES);
                glVertex3f( 0,0,0 );
                glVertex3f( offset.vertex[0], offset.vertex[1], offset.vertex[2] );
                glEnd();
            glPopMatrix();
        }

		if (part->child[i]->child.size() != 0) {
            recursiveDraw (part->child[i], level+1);
		}
	}
	orient.pop();
}

//--------------------------------------------------------------
void testApp::update(){

    if (ofGetFrameNum()%1 == 0){
        counter2 = (counter2+1)%nBvhFrames;
        counter  = counter2 + 1;
        bIsNewBvhFrame = true;
        if (counter2 == 0){
            bHaveExaminedAllFrames = true;
        }
    } else {
        bIsNewBvhFrame = false;
    }

    // Update the FPS calculator
    float nowf = ofGetElapsedTimef();
    float elapsed = nowf - lastFrameTime;
    if (elapsed > 0){ fps = 0.9*fps + 0.1*(1.0f/elapsed);}
    lastFrameTime = nowf;

    // Update the metaballs' positions using random physics
    // m_pMetaballs->Update(elapsed); // in fractions of a second

    float A = 0.8;
    float B = 1.0-A;
    mouseXBlur = A*mouseXBlur + B*mouseX;
    mouseYBlur = A*mouseYBlur + B*mouseY;
	if (bMousePressed){
        mouseXRotate = 0.5*mouseXRotate + 0.5*((mouseXBlur- 0.5*ofGetScreenWidth())/4.0);
	} else {
	    mouseXRotate *= 0.99;
	    /*
	    mouseXBlur = A*mouseXBlur + B*(ofGetScreenWidth()/2);
        mouseYBlur = A*mouseYBlur + B*(ofGetScreenHeight()/2);
	    SetCursorPos((int)mouseXBlur, (int)mouseYBlur);
	    */
	}
}




//--------------------------------------------------------------
void testApp::draw(){



    glPushMatrix();
    glTranslatef(mouseX, mouseY, 0);
    glPointSize(5.0);
    glScalef(1,1,1);
    glColor3f(0,0,0);

    partCount = 0;
    recursiveDraw (theBvhL->root, 0);
    recursiveDraw (theBvhR->root, 0);
    glPopMatrix();


    //---------------------
    // diagnostic draw of retrieved points.
    if (bDrawSkeleton){
        glPushMatrix();
        glTranslatef(mouseX, mouseY, 0);
        glScalef(6,6,6);
        glColor3f(1,0,0);
        for (int i=0; i<nPoints; i++){
            glBegin(GL_POINTS);
            glVertex3f (rawBvhPoints[i].x, rawBvhPoints[i].y, 0);//rawBvhPoints[i].z);
            glEnd();
            sprintf(handyString, "%d", i);
            ofDrawBitmapString(handyString, rawBvhPoints[i].x-4, rawBvhPoints[i].y);
        }
        glPopMatrix();
    }


    //---------------------
    // update bounds.
    if (!bHaveExaminedAllFrames){
        for (int i=0; i<nHandPoints; i++){
            if (rawBvhPoints[i].x < boundsMin.x){ boundsMin.x = rawBvhPoints[i].x; }
            if (rawBvhPoints[i].y < boundsMin.y){ boundsMin.y = rawBvhPoints[i].y; }
            if (rawBvhPoints[i].z < boundsMin.z){ boundsMin.z = rawBvhPoints[i].z; }
            if (rawBvhPoints[i].x > boundsMax.x){ boundsMax.x = rawBvhPoints[i].x; }
            if (rawBvhPoints[i].y > boundsMax.y){ boundsMax.y = rawBvhPoints[i].y; }
            if (rawBvhPoints[i].z > boundsMax.z){ boundsMax.z = rawBvhPoints[i].z; }

            if (counter <= 1){
                boundsAvg.x = rawBvhPoints[i].x;
                boundsAvg.y = rawBvhPoints[i].y;
                boundsAvg.z = rawBvhPoints[i].z;
            } else {
                boundsAvg.x = 0.999*boundsAvg.x + 0.001*rawBvhPoints[i].x;
                boundsAvg.y = 0.999*boundsAvg.y + 0.001*rawBvhPoints[i].y;
                boundsAvg.z = 0.999*boundsAvg.z + 0.001*rawBvhPoints[i].z;
            }
        }
    }

    //---------------------
    // update metaball point locations
    float bx,by,bz;
    float px,py,pz;
    bx = by = bz = 0.5;


    float frac = (float)counter/(float)nBvhFrames;
    frac = 0.50 + 0.50  * cos(TWO_PI * frac);
    frac = 0.05 + 0.95 * frac;
    frac = pow(frac, 4.0);
    float A = frac;
    float B = 1.0-A;
    motionBlur = A;

    float massBase = 0;
    for (int i=0; i<nHandPoints; i++){
        // compute scaled coordinates
        if ((counter > 1) || bHaveExaminedAllFrames){
            bx = (rawBvhPoints[i].x - boundsAvg.x) * boundsScaling;
            by = (rawBvhPoints[i].y - boundsAvg.y) * boundsScaling;
            bz = (rawBvhPoints[i].z - boundsAvg.z) * boundsScaling;
        }
        // stash the previous coordinates
        px = ballPoints[i].x;
        py = ballPoints[i].y;
        pz = ballPoints[i].z;
        if (bIsNewBvhFrame){
            ballPointsPrev[i].set (px,py,pz);
        }

        // compute blurred new coordinates
        bx = A*px + B*bx;
        by = A*py + B*by;
        bz = A*pz + B*bz;
        ballPoints[i].set(bx,by,bz);

        // compute masses
        massBase           = 0.147; //0.135;
        float massBaseSin  = 0.035 * sin(ofGetElapsedTimeMillis()/4000.0);
        float massLevelSin = 0.010 * sin(ofGetElapsedTimeMillis()/1300.0 - ballLevels[i]);
        ballMasses[i] = massBase + massBaseSin + massLevelSin;
    }



    //---------------------
    //---------------------
    //---------------------
    m_pMetaballs->UpdateBallsFromPointsAndMasses (nPoints, ballPoints, ballMasses);

    //---------------------
    // Render the metaballs
    if (m_pMetaballs->bRenderWithTexture == false){
        setupForNoTexturing();
    } else {
        setupForTexturing();
        glColor3f(1,1,1);
        float sw = ofGetScreenWidth() ;
        float sh = ofGetScreenHeight();
        float is = ofGetScreenWidth() * 1.5;
        (m_pMetaballs->spheremap).draw(sw/2 - is/2, sh/2 - is/2, is, is);
    }

    // Actually draw them
    glPushMatrix();
        float xFrac = 0.5 + (float)mouseX/ofGetWidth();
        float w = ofGetWidth();
        float h = ofGetHeight();
        float sz = 0.75*min(w,h);
        glTranslatef(w/2,h/2,0);
        glScalef(sz,sz,sz);

        glRotatef(mouseXRotate, 0,1,0);
        glRotatef(40*sin(ofGetElapsedTimeMillis()/65432.0), 0,1,0);
        glRotatef(30*sin(ofGetElapsedTimeMillis()/40000.0), 0,0,1);
        m_pMetaballs->Render();
    glPopMatrix();



    //---------------------
    bool bDrawDropShadow = true;
    if (bDrawDropShadow){
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(0,0.002,0.005, 0.17);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_CULL_FACE);
        glDisable(GL_COLOR_MATERIAL);
        glReadPixels(0,0,screenW,screenH, GL_DEPTH_COMPONENT, GL_FLOAT, screenDepth);
        huntForBlendFunc(1000,1,5);//0,2);

        int shadowIndex = 0;
        int screenIndex = 0;
        int row = 0;
        for (int y=0; y<shadowH; y++){
            row = y*shadowDivY*screenW;
            for (int x=0; x<shadowW; x++){
                screenIndex = row + x*shadowDivX;
                shadowPixels[shadowIndex++] = (screenDepth[screenIndex] < 1.0)? 255:0;
            }
        }
        shadowCvImage.setFromPixels(shadowPixels, shadowW, shadowH);
        shadowCvImage.blurGaussian( 21 );
        unsigned char *blurred = shadowCvImage.getPixels();
        for (int i=0; i<nShadowPixels; i++){
            shadowPixelsLA[i*2  ] = blurred[i];//(i/shadowW)%255;
            shadowPixelsLA[i*2+1] = blurred[i];
        }
        shadowTex.loadData(shadowPixelsLA, shadowW,shadowH, GL_LUMINANCE_ALPHA);
        glPushMatrix();
            glTranslatef(0,0,-200);//-200);
            glTranslatef(0.53*screenW, 0.73*screenH, 0);
            glScalef(12,12,1);
            glRotatef(60.0, 1,0,0);
            glTranslatef(-shadowW/2,shadowH/2,0);//-200);
            shadowTex.draw(0, 0, shadowW, 0-shadowH);// shadowW,0-shadowH);
            //shadowTex.draw(0, mouseY, shadowW*3,0-shadowH*3);
        glPopMatrix();
        glPopAttrib();
    }

    //---------------------
    // Draw diagnostic text.
    if (bDrawFPS){
        ofSetColor(0x330000);
        sprintf(handyString, "FPS: %f   Counter: %d", fps, counter);
        ofDrawBitmapString(handyString, 50, 50);

        bool bDrawMoreText = false;
        if (bDrawMoreText){
            sprintf(handyString, "MIN:  %f  %f  %f", boundsMin.x, boundsMin.y, boundsMin.z);
            ofDrawBitmapString(handyString, 50, 70);
            sprintf(handyString, "MAX:  %f  %f  %f", boundsMax.x, boundsMax.y, boundsMax.z);
            ofDrawBitmapString(handyString, 50, 90);
            sprintf(handyString, "DEL:  %f  %f  %f",
                (boundsMax.x - boundsMin.x),
                (boundsMax.y - boundsMin.y),
                (boundsMax.z - boundsMin.z));
            ofDrawBitmapString(handyString, 50,110);
            sprintf(handyString, "AVG:  %f  %f  %f", boundsAvg.x, boundsAvg.y, boundsAvg.z);
            ofDrawBitmapString(handyString, 50,130);
            sprintf(handyString, "BLUR: %f",  motionBlur);
            ofDrawBitmapString(handyString, 50,150);
        }
    }


}




//======================================================================================
void testApp::huntForBlendFunc(int period, int defaultSid, int defaultDid){
	// sets all possible combinations of blend functions,
	// changing modes every [period] milliseconds.

	int sfact[] = {
		GL_ZERO,
		GL_ONE,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_SRC_ALPHA_SATURATE
	};

	int dfact[] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA
	};

	glEnable(GL_BLEND);

	if ((defaultSid == -1) && (defaultDid == -1)) {

		int sid =  (ofGetElapsedTimeMillis()/(8*period))%9;
		int did =  (ofGetElapsedTimeMillis()/period)%8;
		glBlendFunc(sfact[sid], dfact[did]);
		printf("SRC %d	DST %d\n", sid, did);

	} else if (defaultDid == -1){

		int did =  (ofGetElapsedTimeMillis()/period)%8;
		glBlendFunc(sfact[defaultSid], dfact[did]);
		printf("SRC %d	DST %d\n", defaultSid, did);

	} else if (defaultSid == -1){

		int sid =  (ofGetElapsedTimeMillis()/(8*period))%9;
		glBlendFunc(sfact[sid], dfact[defaultDid]);
		printf("SRC %d	DST %d\n", sid, defaultDid);

	} else {

		glBlendFunc(sfact[defaultSid], dfact[defaultDid]);

	}
}



//--------------------------------------------------------------
void testApp::setupForTexturing(){
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glColor3f(0.9, 0.9, 0.9);
}

void testApp::setupForNoTexturing(){
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glDisable(GL_BLEND);
    glPolygonMode(GL_BACK, GL_FILL );
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glShadeModel(GL_SMOOTH);

    glColor3f(0.5, 0.5, 0.5);
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE,on);


    GLfloat shininess[] = {0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shininess);


    GLfloat lightPosition[] = { 0.0f, 0.0f, -100.0, 0.0f };
    GLfloat lightDiffuse[]  = { 1.00, 0.99, 0.98, 1.0};
    GLfloat lightSpecular[] = { 0.10, 0.10, 0.10, 1.0};
    GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0,GL_AMBIENT,  lightAmbient);

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    switch ((char)key){
        case 'S':
            ofSaveFrame();
            break;
        case 'F':
            bDrawFPS = !bDrawFPS;
            break;

        /*
        case 'K':
            bDrawSkeleton = !bDrawSkeleton;
            break;
            */

    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    bMousePressed = false;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    bMousePressed = true;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    bMousePressed = true;
    bDrawFPS = false;
    bDrawSkeleton = false;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bMousePressed = false;
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}



// SCRAP
 //---------------------
    /*
    // handle the floaters separately.
    bool bHaveFloaters = true;
    if (bHaveFloaters){
        if (bIsNewBvhFrame){
            for (int i=0; i<nHandPoints; i++){
                if (ballLevels[i] == 4){
                    float Ax = ballPoints[i].x - ballPointsPrev[i].x;
                    float Ay = ballPoints[i].y - ballPointsPrev[i].y;
                    float Az = ballPoints[i].z - ballPointsPrev[i].z;
                    float Ah = sqrt(Ax*Ax + Ay*Ay + Az*Az);
                    float Bx = ballPointsPrev[i].x - ballPointsPrev2[i].x;
                    float By = ballPointsPrev[i].y - ballPointsPrev2[i].y;
                    float Bz = ballPointsPrev[i].z - ballPointsPrev2[i].z;
                    float Bh = sqrt(Bx*Bx + By*By + Bz*Bz);
                    float dh = Bh - Ah;
                    if (dh > 0){
                        if ((dh > maxDeceleration) && (ofGetFrameNum() > 10)){
                            maxDeceleration = dh;
                        }
                        if (bHaveExaminedAllFrames){
                            if (dh > (decelerationThreshPct*maxDeceleration)){
                                decelerationEventCount++;
                            }
                        }
                    }
                 }
            }
            printf("-----------\n");
        }

        for (int i=nHandPoints; i<nPoints; i++){
            ballPoints[i].set(0,0,0);
            ballMasses[i] = 0.0;
        }

    } else {
        for (int i=nHandPoints; i<nPoints; i++){
            ballPoints[i].set(0,0,0);
            ballMasses[i]  = 0.0;
        }
    }
*/
