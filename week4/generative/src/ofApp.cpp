#include "ofApp.h"

string buildShader(string bytebeat) {
return "\
#extension GL_EXT_gpu_shader4 : enable\n\
const int width = 512;\n\
void main() {\n\
	int t = int(gl_FragCoord.y) * width + int(gl_FragCoord.x);\n\
	int x = " + bytebeat + ";\n\
	gl_FragColor = vec4(vec3(float(x % 256) / 256.),1.);\n\
}";
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(10);
	soundStream.setup(this, 2, 0, 48000, 256, 1); //can also say ofSetSoundStrem
	fbo.allocate(1280, 800);
	time = 0;
	rateDivider = 8;
	curCount = 0;
	useShader = false;
	
}

void ofApp::update() {
	keyPressed('u');
}

void ofApp::draw() {
	//ofSetColor(255);
	
	//taking slices of image and outputing into audio
	//can draw whatever u 
    if (useShader) {
		fbo.begin();
		shader.begin();
		ofFill();
		ofRect(0, 0, fbo.getWidth(), fbo.getHeight());
		shader.end();
		fbo.end();
		fbo.draw(0, 0);		
	}
	
	if (!useShader) {
		fbo.begin();
		ofSetColor(ofGetMousePressed()? 255 : 0);
		ofCircle(mouseX,mouseY,100);	
		fbo.end();
		ofSetColor(255);
		fbo.draw(0, 0);

	}
		
	fbo.readToPixels(audioPixels); //this takes the fbo on the graphics card and porting them to CPU
}

void ofApp::keyPressed(int key) {
	if(key == '\\') {
		ofSaveImage(audioPixels, ofToString(curCount++) + ".png");
	}
	time = 0;
	//ofSeedRandom(key); //create one sound for every key
	string source = OneLiner::build();
	cout << source << endl;
	shader.unload();
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, buildShader(source));
	shader.linkProgram();
	
	if (key ==' ') {
		!useShader;
	}
}

void ofApp::audioOut(float* output, int n, int channels) {
	unsigned char* pixels = audioPixels.getPixels();
	int wh = audioPixels.getWidth() * audioPixels.getHeight();
	int cwh = audioPixels.getNumChannels() * wh;
	if(cwh > 0) {
		for(int i = 0; i < n; i++) {
			int curTime = (time / rateDivider) % wh;
			int curPixel = 4 * curTime;
			for(int j = 0; j < channels; j++) {
				int cur = pixels[curPixel + j];
				output[i * channels + j] = cur / 128. - 1.;
				output[i * channels + j] *= .05; // make it quiet
			}
			time++;
		}
	}
}