
/*
 
 TASKS
 -----
 
 QUICK JOBS TO DO QUICKLY
 ------------------------
 
 MAIN LIST
 ---------
 - Load Chinese characters from server on a thread.
 - - - make sure the name isn't repeated in new xml.
 - - - change start position for new tag. X should be random.
 - If the tag data xml does not exist the app will crash. Fix this. 
 - Add debug bounds draw for cloud
 - look into improving anti-aliasing
 - Use bell curve for line alpha - shouldn't be so strong when far away
 - Use bell curve for field strength - sin(norm(mappedDist) * (PI*2)) (or something?)
 - Control Clients across network.
 - - - Joint Smoothing.
 - - - Delay in removing skeleton.
 - - - Adjust Kinect cam era tilt (if possible with Win Kinects).
 - Make a start on fixing doubled up users - get average line - measure distances and build debug birds eye view
 
 NICE TO HAVE IF TIME
 --------------------
 - When skeleton disappears the lines should fade - The user points keep moving in the direction of the last movement.
 - Add multiple camera angles to 1, 2, 3
 - Add lines that connect the user points. These appear when tags are close to userpoints.
 - Add the following keyboard shortcuts
 - Slowly increase demographic strength for new users over time.
 - Improve no-kinect debug mouse Kinect functionality
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
    
    
    gui.setup();
    
	if (isKinectAttached)
        kinectManager.init();
	
	resourceManager.init();
	sceneManager.init(&kinectManager, &resourceManager);
    
    sceneManager.cloudTagMan.buildCloudTags();
    
    isFirstFrame = false;
	isPaused = false;
    
    frameRate = targetframeRate;
    ofSetFrameRate(frameRate);
    
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
    //if (ofGetFrameRate() < targetframeRate)
    //    frameRate += 0.1;
    //else
    //    frameRate -= 0.1;
    //if (frameRate > targetframeRate * 2) frameRate = targetframeRate * 2;
	//printf("ofGetFrameRate():%f, frameRate:%f, targetFrameRate:%f\n", ofGetFrameRate(), frameRate, targetframeRate);

	//if (ofGetFrameNum() % 30 == 0) 
	//ofSetFrameRate(frameRate);
	ofSetFrameRate(targetframeRate);
    
	if (!isPaused)
	{
		if (isKinectAttached) kinectManager.update(); // kinectManager.update();
		sceneManager.update();
        resourceManager.update();
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
        printf("loaded index data = %s\n", tagIndexData.c_str());
        latestTagTotal = ofToInt(tagIndexXml.getValue(""));
        cout << "loaded xml - latestTagTotal = " << latestTagTotal << ", xml loaded " << timesLoaded << " times" << endl;
        
		if (latestTagTotal != currentTagTotal && !isFirstIndexLoad && !isLoadingXml)
		{
			ofLoadURLAsync("http://192.30.139.232/b10/sites/questionnaire//q-7/" + ofToString(latestTagTotal) + ".xml","tag_data_load");
			//ofLoadURLAsync("https://dl.dropboxusercontent.com/u/1619383/cell/" + ofToString(latestTagTotal) + ".xml","tag_data_load");
			isLoadingXml = true;
		}
		isFirstIndexLoad = false;
		currentTagTotal = latestTagTotal;
    }
    
	if (isNewTagDataXml)
	{
		isNewTagDataXml = false;
		tagDataXml.loadFromBuffer(newTagData);
        latestTag = tagDataXml.getValue("content");
        int question = ofToInt(tagDataXml.getValue("question"));
        printf("latestTag = %s, newTag string length = %li \n", latestTag.c_str(), latestTag.size());
        resourceManager.addNewTag(latestTag, question);
	}
    
    
    
    
    
    
    return;
	if (ofGetFrameNum() % (int)serverCheckFrequncy == 0 && ofGetFrameNum() > 10)
    {
        
#ifdef CELL_ONLINE
		loadTagIndexData();
#else
        testTagIndexXml.load("xml/_newTagTest/index.xml");
        latestTagTotal = ofToInt(testTagIndexXml.getValue(""));
        printf("loaded tag index xml - latestTagTotal = %i, currentTagTotal = %i\n", latestTagTotal, currentTagTotal);
        if (latestTagTotal != currentTagTotal && !isFirstIndexLoad && !isLoadingXml)
		{
            tempTagXml.load("xml/_newTagTest/" + ofToString(latestTagTotal) + ".xml");
            latestTag = tempTagXml.getValue("content");
            int question = ofToInt(tempTagXml.getValue("question"));
            printf("latestTag = %s, newTag string length = %li \n", latestTag.c_str(), latestTag.size());
            resourceManager.addNewTag(latestTag, question);
        }
        isFirstIndexLoad = false;
		currentTagTotal = latestTagTotal;
#endif
    }
    
}



void testApp::draw()
{
	sceneManager.draw();
    
	kinectManager.draw();
    
//    if (isFirstFrame || ofGetFrameNum() == 5)
//    {
//        sceneManager.cloudTagMan.updateTags();
//        isFirstFrame = false;
//    }
	glDisable(GL_DEPTH_TEST);
    ofSetColor(0, 0, 0, 255);
    ofRect(0, 0, ofGetWidth(), topBlockHeight);
    ofRect(0, ofGetHeight(), ofGetWidth(), -topBlockBot);
    ofRect(0, 0, leftBlockW, topBlockHeight);
    ofRect(ofGetWidth() - leftBlockW, 0, ofGetWidth(), topBlockHeight);
    if (gui.getVisible())
    {
        ofSetColor(255);
        ofPushMatrix();
        ofTranslate(400, ofGetHeight() - 200);
        string debugStr = "isLoading: " + ofToString((isLoadingXml) ? "true,  " : "false, ") +
        "latest tag index: " + ofToString(currentTagTotal) + "\n";
		for (int i = 0; i < SKELETON_MAX / 2; i++) debugStr += "client " + ofToString(i) + " connected: " + ((kinectManager.clientConnected[i]) ? "true\n" : "false\n");

        ofDrawBitmapString(debugStr, ofPoint(0, 0));
        ofPopMatrix();
    }
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
		sceneManager.cloudTagMan.updateTags();
    }
    else if (key == 'b')
	{
		sceneManager.cloudTagMan.buildCloudTags();
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





void testApp::mouseMoved(int x, int y)
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
    //TODO:Make sure this doesn't load if new tags are still fading
    if (!isLoadingXml)
	{
		//printf("loading index\n");
        ofLoadURLAsync("http://192.30.139.232/b10/sites/questionnaire//q-7.xml","tag_index_load");
//        ofLoadURLAsync("https://dl.dropboxusercontent.com/u/1619383/cell/index.xml","tag_index_load");
	}
	isLoadingXml = true;
}


void testApp::urlResponse(ofHttpResponse & response)
{
    if (response.status == 200 && response.request.name == "tag_index_load")
    {
		//printf("loaded tag index\n");
        tagIndexData = response.data.getText();
        isNewIndexXml = true;
        isLoadingXml = false;
        ++timesLoaded;
	}
	else if (response.status == 200 && response.request.name == "tag_data_load")
    {
        printf("new tag data xmnl loaded \n");
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
