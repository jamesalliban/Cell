
/*
 
 TASKS
 -----
 
 QUICK JOBS TO DO QUICKLY
 ------------------------
 
 MAIN LIST
 ---------
 - Load Chinese characters from server on a thread.
 - Place newly loaded text in scene and save to xml on a thread - add 3 random demographics
 - Make a start on fixing doubled up users - get average line - measure distances and build debug birds eye view
 - Add debug bounds draw for skeleton area and cloud
 - Tags should rise up faster when below (and above??) the cloud
 - If tag moves to opposite side (after going OOB) and is low, fix y val
 - Redo ambient cloud tag animation. Make it more wavy and flowing - perlin up
 - look into improving anti-aliasing
 - If skel data freezes (due to network error), remove skeleton after 10 frames.
 - Use bell curve for line alpha - shouldn't be so strong when far away
 - Use bell curve for field strength - sin(norm(mappedDist) * (PI*2)) (or something?)
 - Improve no-kinect debug mouse Kinect functionality
 - Control Clients across network.
 - - - Joint Smoothing.
 - - - Delay in removing skeleton.
 - - - Adjust Kinect cam era tilt (if possible with Win Kinects).
 
 NICE TO HAVE IF TIME
 --------------------
 - When skeleton disappears the lines should fade - The user points keep moving in the direction of the last movement.
 - Add multiple camera angles to 1, 2, 3
 - Add lines that connect the user points. These appear when tags are close to userpoints.
 - Add the following keyboard shortcuts
 - Slowly increase demographic strength for new users over time.
 - extract record/playback code to ofxSkeletonRecorder
 
 
 Optimisation
 ------------
 - Use power of 2 for tag texturing
 - Perform tag animation calculations on a separate thread
 - Go through each class and only import the necessary OF classes
 
 */

#include "testApp.h"
#include "CellGlobals.h"

bool testApp::isKinectAttached;
bool testApp::isAllUserDebugVisible;

void testApp::setup()
{
    //ofDisableArbTex();
	//ofEnableArbTex();
    ofSetFrameRate(40);
    ofSetFullscreen(false);
	ofSetLogLevel(OF_LOG_SILENT);
//	ofSetLogLevel(OF_LOG_ERROR);
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	ofBackground(30);
	ofDisableSmoothing();
	ofEnableAlphaBlending();
    ofRegisterURLNotification(this);
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
    
    
    
#ifdef CHINESE_CELL
    isLoadingXml = false;
    isNewIndexXml = false;
	isNewTagDataXml = false;
	ofRegisterURLNotification(this);
	timesLoaded = 0;
	isFirstIndexLoad = true;
#endif
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

    
//    if (++currentEllapsedFrames >= xmlCheckFrameFrequency)
//    {
//        printf("- Should be loading tag index xml \n");
//        currentEllapsedFrames = 0;
//        checkNewTagIndex();
//    }
    
    
    
    if (isNewIndexXml)
	{
        isNewIndexXml = false;
        tagIndexXml.loadFromBuffer(tagIndexData);
        latestTagTotal = ofToInt(tagIndexXml.getValue(""));
        cout << "loaded xml - latestTagTotal = " << latestTagTotal << ", xml loaded " << timesLoaded << " times" << endl;

		if (latestTagTotal != currentTagTotal && !isFirstIndexLoad && !isLoadingXml)
		{
			ofLoadURLAsync("https://dl.dropboxusercontent.com/u/1619383/cell/" + ofToString(latestTagTotal) + ".xml","tag_data_load");
			isLoadingXml = true;
		}
		isFirstIndexLoad = false;
		currentTagTotal = latestTagTotal;
    }

	if (isNewTagDataXml)
	{
		isNewTagDataXml = false;
		tagDataXml.loadFromBuffer(newTagData);
	}
    
	if (ofGetFrameNum() % 90 == 0 && ofGetFrameNum() > 10)
		loadTagIndexData();
}



void testApp::draw()
{
	sceneManager.draw();

	kinectManager.draw();

    if (isFirstFrame || ofGetFrameNum() == 5)
    {
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
    else if (key == 'D')
	{
		kinectManager.isRecordedImageVisible = !kinectManager.isRecordedImageVisible;
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
    else if (key == 'z')
    {
        loadTagIndexData();
    }
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



void testApp::windowResized(int w, int h)
{
    sceneManager.setupViewports();
}



void testApp::gotMessage(ofMessage msg)
{
    
}


//void testApp::checkNewTagIndex()
//{
////    return;
////    printf("- checkNewTagIndex() - isLoadingXml = %i\n", (isLoadingXml ? 1 : 0));
////    if (!isLoadingXml)
////    {
////        isLoadingXml = true;
////        printf("- testApp::checkNewTagIndex() loading tag index \n");
////        //int id = ofLoadURLAsync("http://192.30.139.232/b10/sites/questionnaire//q-7.xml", "async_req");
////        int id = ofLoadURLAsync("https://dl.dropboxusercontent.com/u/1619383/cell/index.xml", "async_tag_index");
////    }
//}

void testApp::loadTagIndexData()
{
    if (!isLoadingXml)
        ofLoadURLAsync("https://dl.dropboxusercontent.com/u/1619383/cell/index.xml","tag_index_load");
	isLoadingXml = true;
}


void testApp::urlResponse(ofHttpResponse & response)
{
    if (response.status == 200 && response.request.name == "tag_index_load")
    {
        tagIndexData = response.data.getText();
        isNewIndexXml = true;
        isLoadingXml = false;
        ++timesLoaded;
	}
	else if (response.status == 200 && response.request.name == "tag_data_load")
    {
        newTagData = response.data.getText();
        isNewTagDataXml = true;
        isLoadingXml = false;
	}
    else
    {
		cout << response.status << " " << response.error << endl;
		if(response.status!=-1) isLoadingXml = false;
	}
    
    
    
    
//    return;
//    
//    printf("- testApp::urlResponse() \n");
//    if (response.status == 200 && response.request.name == "async_tag_index")
//    {
//        printf("- Creating new xml file\n");
//        ofXml totalXML;
//        printf("- Loading buffer `
//        else if (latestTagTotal != tagTotal)
//        {
//            printf("- TAG INDEX IS NEW. LOADING NEW TAG XML \n");
//            //int id = ofLoadURLAsync("http://192.30.139.232/b10/sites/questionnaire//q-7/" + ofToString(latestTagTotal) + ".xml", "async_tag_data");
//            //int id = ofLoadURLAsync("https://dl.dropboxusercontent.com/u/1619383/cell/" + ofToString(latestTagTotal) + ".xml", "async_tag_data");
//            https://dl.dropboxusercontent.com/u/1619383/cell/data.xml
//            tagTotal = latestTagTotal;
//        }
//    }
//    else if (response.status == 200 && response.request.name == "async_tag_data")
//    {
//        ofXml tagXML;
//        tagXML.loadFromBuffer(response.data);
//        string newQuestion = tagXML.getValue("question");
//        string newTag = tagXML.getValue("content");
//        isLoadingXml = false;
//        printf("- Loaded new tag - %s \n", newTag.c_str());
//    }
//    else
//    {
//        printf("- else \n");
//        cout << response.status << " " << response.error << endl;
//        if (response.status != -1) isLoadingXml = false;
//    }
}


void testApp::exit()
{
    ofUnregisterURLNotification(this);
}
