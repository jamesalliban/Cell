#include "ofMain.h"
#include "TestApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( )
{
    ofAppGlutWindow window;
    // can be OF_WINDOW or OF_FULLSCREEN
	//ofSetupOpenGL(&window, 1600, 1000, OF_FULLSCREEN); //1600, 1000, OF_WINDOW);		1600, 1000	// <-------- setup the GL context
	window.setGlutDisplayString("rgb alpha double samples depth");
	//ofSetupOpenGL(&window, 1690, 900, OF_FULLSCREEN);
	ofSetupOpenGL(&window, 1690, 900, OF_FULLSCREEN);
	// also change the viewPort.width and viewPort.height in SceneManager::setupViewports()


	ofRunApp( new TestApp());
}
