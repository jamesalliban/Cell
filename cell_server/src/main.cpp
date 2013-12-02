#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
	
	//ofSetupOpenGL(1600,908, OF_WINDOW);
	//ofSetupOpenGL(3840,1024, OF_WINDOW);
	//ofSetWindowPosition(-1920, 10);
	ofSetupOpenGL(1920, 700, OF_WINDOW);
	ofSetWindowPosition(10, 10);


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
