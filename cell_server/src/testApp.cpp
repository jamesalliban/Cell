
/*
 
 TASKS
 -----
 
 QUICK JOBS TO DO QUICKLY
 ------------------------
 - add kinect recording path to playback visual
 - hide skeleton timeline on 'D' - separate from user debug
 - lines fade in at different speeds - maybe random, maybe depends on distance - try both
 
 MAIN LIST
 ---------
 - Load Chinese characters from server on a thread.
 - Place newly loaded text in scene and save to xml on a thread - add 3 random demographics
 - Make a start on fixing doubled up users - get average line
 - Add debug bounds draw for skeleton area and cloud
 - Tags should rise up faster when below (and above??) the cloud
 - If tag moves to opposite side (after going OOB) and is low, fix y val
 - Redo ambient cloud tag animation. Make it more wavy and flowing - perlin up
 - look into improving anti-aliasing
 - If skel data freezes (due to network error), remove skeleton after 10 frames.
 - Use bell curve for line alpha - shouldn't be so strong when far away
 - Use bell curve for field strength - sin(norm(mappedDist) * (PI*2)) (or something?)
 - Improve no-kinect debug mouse Kinect functionality
 
 
 NICE TO HAVE IF TIME
 --------------------
 - When skeleton disappears the lines should fade - The user points keep moving in the direction of the last movement.
 - Add multiple camera angles to 1, 2, 3
 - Add lines that connect the user points. These appear when tags are close to userpoints.
 - Add the following keyboard shortcuts
 - Slowly increase demographic strength for new users over time.
 - - 'T' - toggle tag animation and calculations
 - extract record/playback code to ofxSkeletonRecorder
 
 
 Optimisation
 ------------
 - Use power of 2 for tag texturing
 - Perform tag animation calculations on a separate thread
 - Go through each class and only import the necessary OF classes
 
 */

#include "testApp.h"

bool testApp::isKinectAttached;
bool testApp::isAllUserDebugVisible;

void testApp::setup()
{
    //ofDisableArbTex();
	//ofEnableArbTex();
    //ofSetFrameRate(60);
    ofSetFullscreen(false);
	ofSetLogLevel(OF_LOG_ERROR); //OF_LOG_WARNING
	ofSetVerticalSync(true);
	ofBackground(30);
	ofDisableSmoothing();
	ofEnableAlphaBlending();
    ofSetCircleResolution(10);
	glEnable(GL_DEPTH_TEST);
    
	isKinectAttached = true;
    
	if (isKinectAttached)
	{
        kinectManager.init();
	}
	
	resourceManager.init();
	sceneManager.init(&kinectManager, &resourceManager);
//	myGui.init();
    gui.setup();
    
    isFirstFrame = false;
	isPaused = false;
}



void testApp::update()
{
    //if (ofGetFrameNum() % 10 == 0)
    //    printf("mouseX:%i, mouseY:%i, ofGetMouseX():%i, ofGetMouseY():%i \n", mouseX, mouseY, ofGetMouseX(), ofGetMouseY());
    //
    
	if (!isPaused)
	{
		if (isKinectAttached) kinectManager.update(); // kinectManager.update();
		sceneManager.update();
	}

//    framesSinceMouseMove++;
//    if (framesSinceMouseMove > 100)
//        ofHideCursor();
//    else
//        ofShowCursor();
}



void testApp::draw()
{
	sceneManager.draw();

	kinectManager.draw();

    if (isFirstFrame || ofGetFrameNum() == 5)
    {
        for (int i = 0; i < SKELETON_MAX; i++)
        {
            KinectSkeletonData* skeletonData = &kinectManager.trackedSkeletons[i];
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////// NOTE: This is crashing - why? and why is it used
            //skeletonData->dwTrackingID = -1;
        }
        //sceneManager.userManager.deactivateAllUsers();
        sceneManager.isUpdateVars = true;
        isFirstFrame = false;
    }

	glDisable(GL_DEPTH_TEST);
    ofSetColor(0, 0, 0, 255);
    ofRect(0, 0, ofGetWidth(), topBlockHeight);
    ofRect(0, ofGetHeight(), ofGetWidth(), -topBlockBot);
    ofRect(0, 0, leftBlockW, topBlockHeight);
    ofRect(ofGetWidth() - leftBlockW, 0, ofGetWidth(), topBlockHeight);
    glEnable(GL_DEPTH_TEST);
	
    //ofPushStyle();
    //ofSetColor(255);
    //ofDrawBitmapString("fps:" + ofToString(ofGetFrameRate()), 500, 120);
    //ofPopStyle();
}




void testApp::keyPressed(int key)
{
	if (key == 'f')
    {
		ofToggleFullscreen();
	}
    else if (key == 'p')
	{
        isPaused = !isPaused;
    }
    else if (key == 't')
	{
		sceneManager.cloudTagMan.areTagsEnabled = !sceneManager.cloudTagMan.areTagsEnabled;
    }
    else if (key == 'o')
    {
        sceneManager.userManager.isNonKinectUserPaused = !sceneManager.userManager.isNonKinectUserPaused;
    }
    else if (key == 'l')
	{
        sceneManager.cloudTagMan.areLinesEnabled = !sceneManager.cloudTagMan.areLinesEnabled;
    }
    else if (key == 'R')
	{
		kinectManager.startRecording();
    }
    else if (key == 'S')
	{
		kinectManager.stopRecording();
    }
    else if (key == 'E')
	{
		kinectManager.saveRecording();
    }
    else if (key == 'P')
	{
		kinectManager.startPlayback("");
    }
    else if (key == 'C')
	{
		kinectManager.clearPlayback();
    }
    else if (key == 'P')
	{
		kinectManager.startRecording();
    }
    else if (key == 'O')
	{
		kinectManager.isPlaybackPaused = !kinectManager.isPlaybackPaused;
    }
    else if (key == 'm')
	{
		sceneManager.isCamMouseInput = !sceneManager.isCamMouseInput;
    }
    else if (key == 'n')
	{
		sceneManager.isCamMouseInputPaused = !sceneManager.isCamMouseInputPaused;
    }
    else if (key == 'u')
	{
		sceneManager.isUpdateVars = !sceneManager.isUpdateVars;
    }
    else if (key == 'g')
	{
		sceneManager.isGridVisible = !sceneManager.isGridVisible;
    }
    else if (key == 'a')
	{
		sceneManager.fieldMan.isFieldLineVisible = !sceneManager.fieldMan.isFieldLineVisible;
    }
    else if (key == 'd')
	{
		isAllUserDebugVisible = !isAllUserDebugVisible;
    }
}



void testApp::keyReleased(int key)
{
    
}



void testApp::mouseMoved(int x, int y )
{
    framesSinceMouseMove = 0;
}



void testApp::mouseDragged(int x, int y, int button)
{
    kinectManager.skipToFrame(x, y);
}



void testApp::mousePressed(int x, int y, int button)
{
    kinectManager.skipToFrame(x, y);
}



void testApp::mouseReleased(int x, int y, int button)
{
    
}



void testApp::windowResized(int w, int h)
{
    sceneManager.setupViewports();
}



void testApp::gotMessage(ofMessage msg)
{
    
}



void testApp::dragEvent(ofDragInfo dragInfo)
{
    
}


//void testApp::exit()
//{
//    if (nwserver != NULL)
//    {
//        nwserver->Pause();
//    }
//
//	if(kinect != NULL)
//	{
//		kinect->Close();
//
//		delete kinect;
//		kinect = NULL;
//	}
//
//	if( firstDevice != NULL )
//	{
//		delete firstDevice;
//		firstDevice = NULL;
//	}
//
//	if( skeletonDrawer  != NULL )
//	{
//		delete skeletonDrawer;
//		skeletonDrawer = NULL;
//	}
//}
