#include "TestApp.h"

/*

 TASKS

 Optimisation
 ------------
 - Use power of 2 for tag texturing
 - CloudTag::performUserAttraction() - interpolates joint is calculated each frame for each tag - this should be done once per user per tag on startup

TASKS
-----
 - Bring back tag tie colour
 - Update Cell_Client projects to OF 0.8.0
 - Sort Cloud Tags based on z depth
 - Dynamically create tags instead of loading images
 - Load Chinese characters
 - Improve no-kinect debuggin system
   - Move no-kinect debug Kinect stuff to KinectManager so we can use UserData, joint spheres etc.
   - Create a system to record/playback animated Kinect users and encode the data to an image. These can be added, removed at will
 - Add a series of keyboard shortcuts
 - Swap UI for ofxUI
 - Where are the colour fields???



// OLD BUT STILL NEED TO INVESTIGATE
- ******* once user is removed (or are too far from tag) animate the tag towards the cloud faster.
- ******* allow for a greater leangthSquaredMin when the joint is below the waist
- ******* Add tags to random points between joints
- ******* Add line to bottom edge of tag
- ******* add another fake user
- ******* work on multi user issues - 2nd user steals tags currently
- ******* behaviour fields framework
  - ******* repel
  - attract
  - ******* scale
  - ******* y position
  - colours
    - 5 colour - only 2 at a time
    - sharp edges - no strength
    - responsive to users


- Make the field strength a bell curve - sin(mappedDist)
- increase demographic strength for new users over time.
- Only show lines if there is a connection
- //make some tags animate to a distance for a diagram look
- add circles to user points
  - alpha is tied to z depth and length of tie. Only appear when length is cloae
  - bit of perlin to animate
- connect user points
- add grid controls to GUI

- try to draw kinect data for debug

------------------
Keyboard shortcuts
------------------

'm' - main camera position
'n' - pause free cam
'u' - update - refresh cloud and can implement changed values - check this
'f' - fullscreen
'o' - pause debug skeleton

'z' - clear client skeletons
'p' - pause clients
'r' - resume clients
'x' - close clients
's' - send smoothing value to clients
'c' - save screeshot
'UP' - tilt client kinect camera up
'DOWN' - tilt client kinect camera down

'g' or 'space' - show GUI
'[' and ']' - toggle through gui pages
'0' - '9' - go to gui page


 */

void TestApp::setup()
{
	
    //ofDisableArbTex();
	//ofEnableArbTex();
    //ofSetFrameRate(60);
    ofSetFullscreen(true);
    //ofEnableNormalizedTexCoords();
	ofSetLogLevel(OF_LOG_ERROR); //OF_LOG_WARNING
	ofSetVerticalSync(true);
	ofBackground(2595, 255, 255);
	ofDisableSmoothing();
	ofEnableAlphaBlending();
	glEnable(GL_DEPTH_TEST);

	isKinectAttached = false;

	if (isKinectAttached)
	{
        kinectManager = new KinectManager();
        kinectManager->init();
	}
	
    ///*
	resourceManager.init();
	printf("resources inited\n");
	sceneManager.init();
	printf("sceneManager inited\n");
	myGui = new MyGui();
    //*/

    isFirstFrame = false;
}



void TestApp::update()
{
    if (isKinectAttached) kinectManager->update(); // kinectManager->update();

    sceneManager.update();

    framesSinceMouseMove++;
    if (framesSinceMouseMove > 100)
        ofHideCursor();
    else
        ofShowCursor();
}



void TestApp::draw()
{
 //   ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
 //   ofCircle(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), ofRandom(2, 100));


    ///*
	sceneManager.draw();

//	glDisable(GL_DEPTH_TEST);
//    kinectManager->draw();
//    glEnable(GL_DEPTH_TEST);

    if (isFirstFrame || ofGetFrameNum() == 5)
    {
        for (int i = 0; i < SKELETON_MAX; i++)
        {
            KinectSkeletonData* skeletonData = &kinectManager->trackedSkeletons[i];
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


    myGui->draw();

    //*/

	//ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
	//ofCircle(400, 400, 100, 100);
}




void TestApp::keyPressed(int key)
{
//    vector<int> d;
//    d[43] = 3;
    ///*
	sceneManager.keyPressed(key);
	//*/
	if (isKinectAttached) kinectManager->keyPressed(key);  // kinectManager->keyPressed(key);
	///*
    myGui->keyPressed(key);
    //*/

	if (key == 'f')
		ofToggleFullscreen();

}



void TestApp::keyReleased(int key)
{

}



void TestApp::mouseMoved(int x, int y )
{
    framesSinceMouseMove = 0;
}



void TestApp::mouseDragged(int x, int y, int button)
{

}



void TestApp::mousePressed(int x, int y, int button)
{

}



void TestApp::mouseReleased(int x, int y, int button)
{

}



void TestApp::windowResized(int w, int h)
{
//	sceneManager.setupViewports();
}



void TestApp::gotMessage(ofMessage msg)
{

}



void TestApp::dragEvent(ofDragInfo dragInfo)
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
