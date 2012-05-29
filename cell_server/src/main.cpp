#include "ofMain.h"
#include "TestApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( )
{
    ofAppGlutWindow window;
    // can be OF_WINDOW or OF_FULLSCREEN
	//ofSetupOpenGL(&window, 1600, 1000, OF_FULLSCREEN); //1600, 1000, OF_WINDOW);		1600, 1000	// <-------- setup the GL context
	ofSetupOpenGL(&window, 1280, 720, OF_FULLSCREEN);
	// also change the viewPort.width and viewPort.height in SceneManager::setupViewports()


	ofRunApp( new TestApp());
}
