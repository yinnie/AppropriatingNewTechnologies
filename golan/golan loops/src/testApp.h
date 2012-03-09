#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"

#include "Metaballs.h"
#include "Marchingcubes.h"
#include "bvh.hpp"
#include <stack>



class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

		int 	counter;
		int     counter2;
		float   lastFrameTime;
		float   fps;

        bool    bDrawFPS;
        bool    bDrawSkeleton;
		bool	bMousePressed;
		float   mouseXBlur;
		float   mouseYBlur;
		float   mouseXRotate;

		//---------------------------------------
		char        *handyString;
		CMetaballs  *m_pMetaballs;
		ofPoint     *rawBvhPoints;
		ofPoint     *ballPoints;
		ofPoint     *ballPointsPrev;
		ofPoint     *ballPointsPrev2;
		float       *ballMasses;
		int         *ballLevels;
		int         nPoints;
		int         nHandPoints;

		int         nFloaters;
		float       maxDeceleration;
		float       decelerationThreshPct;
		int         decelerationEventCount;

		void setupForTexturing();
		void setupForNoTexturing();


		//---------------------------------------
		void recursiveDraw (bvhPart* part, int level);
		bvh         *theBvhL;
		bvh         *theBvhR;
		int         partCount;
		matrix16f   theIdentityMatrix;
		stack<matrix16f> orient;

		ofPoint     boundsMin;
		ofPoint     boundsMax;
		ofPoint     boundsAvg;
		float       boundsScaling;
		bool        bIsNewBvhFrame;
		bool        bHaveExaminedAllFrames;
		int         nBvhFrames;
		float       motionBlur;

        float       *screenDepth;   // the fetched depth buffer
		int         nScreenPixels;  // number of pixels in the display device
		int         screenW;        // width of the display device
		int         screenH;        // height of the display device
		ofTexture	shadowTex;      // texture object for displaying the shadow
		ofxCvGrayscaleImage  shadowCvImage;
        int         shadowDivX;     // ratio in X of screenW:shadowW
        int         shadowDivY;
        unsigned char *shadowPixels;
        unsigned char *shadowPixelsLA; // Luminance + Alpha
        int         nShadowPixels;
        int         shadowW;
		int         shadowH;
		void 		huntForBlendFunc (int period, int defaultSid, int defaultDid);


        //---------------------------------------
        GLfloat *materialAmbient;
        GLfloat *materialDiffuse;
        GLfloat *materialSpecular;

};

#endif

