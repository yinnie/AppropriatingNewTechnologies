#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1200, 768, OF_FULLSCREEN);
	ofRunApp(new ofApp());
}
