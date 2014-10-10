
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// WARNING /////////////////////////////////////////
////////// THIS APP WILL RUN VERY SLOWLY IF THE NETWORK IS NOT SET UP. TO TEST /////////
///////////////// COMMENT OUT THE OSC SENDER MESSAGE CODE IN UPDATE() //////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#include "ofApp.h"
#include "ofxKinectNuiDraw.h"

void ofApp::setup() {
	ofSetLogLevel(OF_LOG_WARNING);
	ofSetFullscreen(true);
	ofSetFrameRate(30);

	//Netork config txt file is located in C:/ on each client. It should contain the following lines only

	//192.168.0.199
	//11999
	//client id

	//The first line should be the IP address of the server 
	//The second line should be the Port that the server uses to listen on and the clients should connect to
	//The third line is the client ID. This is to let the server differentiate between the clients
	
	ofFile file;
	//set up the client
	string serverIPAddress;
	if(file.open("c:\\ipaddress.txt"))
	{
		ofBuffer buf = file.readToBuffer();
		file.close();
		serverIPAddress = buf.getFirstLine();
		string porOut = buf.getNextLine();
		clientID = ofToInt(buf.getNextLine());

		printf("Loaded ipaddress.txt - clientID = %i \n", clientID);
    }
	
	buildSkeletonDataObjects();

	ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = false;
	initSetting.grabDepth = true;
	initSetting.grabAudio = false;
	initSetting.grabLabel = true;
	initSetting.grabSkeleton = true;
	initSetting.grabCalibratedVideo = false;
	initSetting.grabLabelCv = false;
	initSetting.videoResolution = NUI_IMAGE_RESOLUTION_640x480;
	initSetting.depthResolution = NUI_IMAGE_RESOLUTION_640x480;
	kinect.init(initSetting);
//	kinect.setMirror(false); // if you want to get NOT mirror mode, uncomment here
//	kinect.setNearmode(true); // if you want to set nearmode, uncomment here
	kinect.open();
//	kinect.open(true); // when you want to use near mode

	kinect.addKinectListener(this, &ofApp::kinectPlugged, &ofApp::kinectUnplugged);
	
	ofSetVerticalSync(true);

	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	nearClipping = 0;
	farClipping = kinect.getFarClippingDistance();
	farClipping = 10000;

	ofFbo::Settings settings;
	settings.width = 600;
	settings.height = 500;
	settings.internalformat = GL_RGB;
	displayTextFbo.allocate(settings);

	newSkelDataFbo.allocate(settings);

	bDrawVideo = false;
	bDrawDepthLabel = false;
	bDrawSkeleton = false;
	bDrawCalibratedTexture = false;

	prevSkelCount = 0;
	ofSetFrameRate(40);
	
	calibratedTexture.allocate(320, 240, GL_RGB);

	videoDraw_ = ofxKinectNuiDrawTexture::createTextureForVideo(kinect.getVideoResolution());
	depthDraw_ = ofxKinectNuiDrawTexture::createTextureForDepth(kinect.getDepthResolution());
	labelDraw_ = ofxKinectNuiDrawTexture::createTextureForLabel(kinect.getDepthResolution());
	skeletonDraw_ = new ofxKinectNuiDrawSkeleton();
	kinect.setVideoDrawer(videoDraw_);
	kinect.setDepthDrawer(depthDraw_);
	kinect.setLabelDrawer(labelDraw_);
	kinect.setSkeletonDrawer(skeletonDraw_);


#ifdef OSC_ENABLED
	sender.setup(serverIPAddress, PORT_OUT);
    //receiver.setup(PORT_IN);
#endif
	
	joints.push_back("HIPC");
	joints.push_back("SPIN");
	joints.push_back("SHOC");
	joints.push_back("HEAD");
	joints.push_back("SHOL");
	joints.push_back("ELBL");
	joints.push_back("WRIL");
	joints.push_back("HNDL");
	joints.push_back("SHOR");
	joints.push_back("ELBR");
	joints.push_back("WRIR");
	joints.push_back("HNDR");
	joints.push_back("HIPL");
	joints.push_back("KNEL");
	joints.push_back("ANKL");
	joints.push_back("FOTL");
	joints.push_back("HIPR");
	joints.push_back("KNER");
	joints.push_back("ANKR");
	joints.push_back("FOTR");

	isTesting = false;
	testCount = 0;
}


void ofApp::buildSkeletonDataObjects()
{
	for (int i = 0; i < 2; i++)
	{
        SkeletonData skelData;
		skelData.index = -1;
		skelData.id = -1;
		skelData.resetCount = 0;
		skelData.isActive = false;
		for (int i = 0; i < 20; i++)
			skelData.skelPoints.push_back(ofPoint(-1, -1, -1));
		
		skeletonDataObjects.push_back(skelData);
	}

	prevSkeletonDataObjects = skeletonDataObjects;
}


void ofApp::resetSkeletonData(int index)
{
    SkeletonData* skelData = &skeletonDataObjects[index];

	skelData->index = -1;
	skelData->id = -1;
	skelData->resetCount = 0;
	skelData->isActive = false;
	for (int i = 0; i < 20; i++)
		skelData->skelPoints[i] = ofPoint(-1, -1, -1); // [i] = ofVec3f(-1, -1, -1);
}


void ofApp::populateSkeletonData(vector<ofPoint> points, int newSkelId, int skelDataObjectIndex, bool isSkelNew)
{
	SkeletonData* skelData = &skeletonDataObjects[skelDataObjectIndex];

	if (isSkelNew) 
	{
		skelData->index = skelDataObjectIndex + (clientID * 2);
		skelData->id = newSkelId;
		skelData->isActive = true;
	}
	for (int i = 0; i < 20; i++)
	{
		skelData->skelPoints[i] = points[i];
	}
}


int ofApp::getEmptySkelObjectIndex()
{
	for (int i = 0; i < skeletonDataObjects.size(); i++)
	{
		if (!skeletonDataObjects[i].isActive)
			return i;
	}
	return -1;
}


SkeletonData ofApp::getNewSkelFromId(vector<SkeletonData> newSkelData, int id)
{
	for(int i = 0; i < newSkelData.size(); i++)
	{
		if (newSkelData[i].id == id)
			return newSkelData[i];
	}
}


void ofApp::update() 
{
	if (ofGetElapsedTimeMillis() < 2000) return;
	
	prevSkeletonDataObjects = skeletonDataObjects;
	kinectSource->update();
	
	ofPoint* points[ofxKinectNui::SKELETON_COUNT];
	int skelCount = kinect.getSkeletonPoints(points);
	vector<stringstream> dataStrings;
	newSkelDataFbo.begin();
	ofClear(0, 0, 0, 1);
	newSkelDataFbo.end();
	int foundSkelCount = 0;
	if(!isTesting && kinect.isFoundSkeleton())
	{
		newSkelData.clear();
		for(int i = 0; i < ofxKinectNui::SKELETON_COUNT; i++)
		{
			if(kinect.isTrackedSkeleton(i))
			{
				SkeletonData skeletonData;
				skeletonData.id = i;
				vector<ofPoint> skelPoints;
				for(int j = 0; j < ofxKinectNui::SKELETON_POSITION_COUNT; j++)
				{
					skelPoints.push_back(points[i][j]);
				}
				skeletonData.skelPoints = skelPoints;
				newSkelData.push_back(skeletonData);


				stringstream kinectStream;
				kinectStream << "skel index: " << i << "\n" << endl;
				for (int j = 0; j < 20; j++)
				{
					kinectStream << joints[i] << " ";
					kinectStream << "x: " << points[i][j].x << ", ";
					kinectStream << "y: " << points[i][j].y << ", ";
					kinectStream << "z: " << points[i][j].z << endl;
				}
				newSkelDataFbo.begin();
				ofDrawBitmapString(kinectStream.str(), foundSkelCount * 260, 20);
				newSkelDataFbo.end();

				foundSkelCount++;
			}
		}
	}

	//if (isTesting)
	//{
	//	skelCount = testCount;
	//	for (int i = 0; i < skelCount; i++)
	//	{
	//		stringstream kinectStream;
	//		kinectStream << "skel index: " << newSkelData[i].id << "\n" << endl;
	//		for (int j = 0; j < 20; j++)
	//		{
	//			kinectStream << joints[j] << " ";
	//			kinectStream << "x: " << newSkelData[i].skelPoints[j].x << ", ";
	//			kinectStream << "y: " << newSkelData[i].skelPoints[j].y << ", ";
	//			kinectStream << "z: " << newSkelData[i].skelPoints[j].z << endl;
	//		}
	//		newSkelDataFbo.begin();
	//		ofDrawBitmapString(kinectStream.str(), foundSkelCount * 260, 20);
	//		newSkelDataFbo.end();

	//		foundSkelCount++;
	//	}
	//}


	for (int i = 0; i < skeletonDataObjects.size(); i++)
	{
		SkeletonData* skelDataObject = &skeletonDataObjects[i];
		// if the number of active skeletons hasn't changed, simply update the active skel data objects with new data
		if (skelCount == prevSkelCount)
		{
			if (skelDataObject->isActive)
				populateSkeletonData(getNewSkelFromId(newSkelData, skelDataObject->id).skelPoints, skelDataObject->id, i, false);
		}
		else
		{
			if (prevSkelCount == 0 && skelCount == 1)
			{
				ofLogVerbose("prevSkelCount == 0 && skelCount == 1 a");
				if (i == 0) 
					populateSkeletonData(newSkelData[0].skelPoints, newSkelData[0].id, 0, true);
				ofLogVerbose("prevSkelCount == 0 && skelCount == 1 b");
			}
			else if (prevSkelCount == 1 && skelCount == 0)
			{
				ofLogVerbose("prevSkelCount == 1 && skelCount == 0 a");
				if (skelDataObject->isActive) 
					resetSkeletonData(i);
				ofLogVerbose("prevSkelCount == 1 && skelCount == 0 b");
			}
			else if (prevSkelCount == 1 && skelCount == 2)
			{
				ofLogVerbose("prevSkelCount == 1 && skelCount == 2 a");
				if (skelDataObject->isActive)
				{
					SkeletonData newSkelFromId = getNewSkelFromId(newSkelData, skelDataObject->id);
					ofLogVerbose() << "skelDataObject " << i << " is active assigning id " << newSkelFromId.id;
					populateSkeletonData(newSkelFromId.skelPoints, newSkelFromId.id, i, false);
				}
				else
				{
					if (i == 0 && skeletonDataObjects[1].id == newSkelData[0].id)
						populateSkeletonData(newSkelData[1].skelPoints, newSkelData[1].id, i, true);
					else if (i == 0 && skeletonDataObjects[1].id == newSkelData[1].id)
						populateSkeletonData(newSkelData[0].skelPoints, newSkelData[0].id, i, true);
					else if (i == 1 && skeletonDataObjects[0].id == newSkelData[0].id)
						populateSkeletonData(newSkelData[1].skelPoints, newSkelData[1].id, i, true);
					else if (i == 1 && skeletonDataObjects[1].id == newSkelData[1].id)
						populateSkeletonData(newSkelData[0].skelPoints, newSkelData[0].id, i, true);
				}
				ofLogVerbose("prevSkelCount == 1 && skelCount == 2 b");
			}
			else if (prevSkelCount == 2 && skelCount == 1)
			{
				ofLogVerbose("prevSkelCount == 2 && skelCount == 1 a");
				// first, find out if this skel object's ID matches the remaining new one
				if (skelDataObject->id == newSkelData[0].id)
				{
					populateSkeletonData(newSkelData[0].skelPoints, newSkelData[0].id, i, false);
				}
				else if (skelDataObject->id != newSkelData[0].id)
				{
					resetSkeletonData(i);
				}

				ofLogVerbose("prevSkelCount == 2 && skelCount == 1 b");
			}
			else if (prevSkelCount == 0 && skelCount == 2)
			{
				ofLogVerbose("prevSkelCount == 0 && skelCount == 2 a");
				populateSkeletonData(newSkelData[i].skelPoints, newSkelData[i].id, i, true);
				ofLogVerbose("prevSkelCount == 0 && skelCount == 2 b");
			}
			else if (prevSkelCount == 2 && skelCount == 0)
			{
				ofLogVerbose("prevSkelCount == 2 && skelCount == 0 a");
				resetSkeletonData(i);
				ofLogVerbose("prevSkelCount == 2 && skelCount == 0 b");
			}
		}
	}

	prevSkelCount = skelCount;

#ifdef OSC_ENABLED
	ofxOscMessage m;
	m.setAddress("/skeleton/data");
	m.addIntArg(clientID); // client

	// add data from both SkeletonData objects, even if inactive - client, id, skelpoints, velpoints
	for (int i = 0; i < (int)skeletonDataObjects.size(); i++)
	{
		SkeletonData* skelData = &skeletonDataObjects[i];
		m.addIntArg(i);
		m.addIntArg((skelData->isActive) ? 1 : -1);
		
		for (int j = 0; j < 20; j++)
		{
			m.addFloatArg(skelData->skelPoints[j].x);
			m.addFloatArg(skelData->skelPoints[j].y);
			m.addFloatArg(skelData->skelPoints[j].z);
		}
	}
    sender.sendMessage(m);
#endif
}

//--------------------------------------------------------------
void ofApp::draw() 
{
	ofBackground(0, 0, 0);
	if(bDrawVideo)
	{
		kinect.drawVideo(0, 0, 1024, 768);	// draw video images from kinect camera
	}
	else if(bDrawDepthLabel)
	{
		ofEnableAlphaBlending();
		kinect.drawDepth(0, 0, 1024, 768);	// draw depth images from kinect depth sensor
		kinect.drawLabel(0, 0, 1024, 768);		// draw players' label images on video images
		ofDisableAlphaBlending();
	}
	else if(bDrawSkeleton)
	{
		kinect.drawSkeleton(0, 0, 1024, 768);	// draw skeleton images on video images
	}
	else
	{
		
		float sceneDrawScale = 0.95;
		float w = 480 * sceneDrawScale;
		float h = 360 * sceneDrawScale;
		ofEnableAlphaBlending();
		kinect.drawDepth(0, 0, w, h);	// draw depth images from kinect depth sensor
		kinect.drawLabel(0, 0, w, h);		// draw players' label images on video images
		ofDisableAlphaBlending();
		kinect.drawSkeleton(0, 0, w, h);	// draw skeleton images on video images
	}

	stringstream kinectReport;
	if(bPlugged && !kinect.isOpened())
	{
		ofSetColor(0, 255, 0);
		kinectReport << "Kinect is plugged..." << endl;
		ofDrawBitmapString(kinectReport.str(), 200, 400);
	}
	else if(!bPlugged)
	{
		ofSetColor(255, 0, 0);
		kinectReport << "Kinect is unplugged..." << endl;
		ofDrawBitmapString(kinectReport.str(), 200, 400);
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "fps: " << ofGetFrameRate() << endl
				<< "'c': close stream" << endl
				<< "'o': open stream" << endl
				<< "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
				<< "press 's' to show skeleton only: " << bDrawSkeleton << endl
				<< "press 'd' to show depth + users label only: " << bDrawDepthLabel << endl;

	ofDrawBitmapString(reportStream.str(), 20, 500);
	
	stringstream kinect0Stream;
	SkeletonData* skelData = &skeletonDataObjects[0];
	kinect0Stream << "skel index: " << skelData->index << ", skel id: " << skelData->id << "\n" << endl;
	for (int i = 0; i < 20; i++)
	{
		kinect0Stream << joints[i] << " ";
		kinect0Stream << "x: " << skelData->skelPoints[i].x << ", ";
		kinect0Stream << "y: " << skelData->skelPoints[i].y << ", ";
		kinect0Stream << "z: " << skelData->skelPoints[i].z << endl;
	}

	stringstream kinect1Stream;
	skelData = &skeletonDataObjects[1];
	kinect1Stream << "skel index: " << skelData->index << ", skel id: " << skelData->id << "\n" << endl;
	for (int i = 0; i < 20; i++)
	{
		kinect1Stream << joints[i] << " ";
		kinect1Stream << "x: " << skelData->skelPoints[i].x << ", ";
		kinect1Stream << "y: " << skelData->skelPoints[i].y << ", ";
		kinect1Stream << "z: " << skelData->skelPoints[i].z << endl;
	}

	float textScale = 1.8;

	displayTextFbo.begin();
	ofClear(0, 0, 0, 1);
	ofDrawBitmapString(kinect0Stream.str(), 0, 20);
	ofDrawBitmapString(kinect1Stream.str(), 260, 20);
	displayTextFbo.end();

	ofPushMatrix();
	ofTranslate(480, 10);
	ofScale(textScale, textScale);
	displayTextFbo.draw(0, 0);
	ofPopMatrix();
	
	if (isTesting)
	{
		ofPushStyle();
		ofSetColor(255, 120, 120);
		ofPushMatrix();
		ofTranslate(480, 300);
		ofScale(textScale, textScale);
		newSkelDataFbo.draw(0, 0);
		ofPopMatrix();
		ofPopStyle();
	}
}


//--------------------------------------------------------------
void ofApp::exit() 
{
	if(calibratedTexture.bAllocated())
	{
		calibratedTexture.clear();
	}
	//kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) 
{
	switch(key){
	case 'd': // draw depth + users label only
	case 'D':
		bDrawDepthLabel = !bDrawDepthLabel;
		if(bDrawDepthLabel){
			bDrawCalibratedTexture = false;
			bDrawVideo = false;
			bDrawSkeleton = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 's': // draw skeleton only
	case 'S':
		bDrawSkeleton = !bDrawSkeleton;
		if(bDrawSkeleton){
			bDrawCalibratedTexture = false;
			bDrawVideo = false;
			bDrawDepthLabel = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'o': // open stream
	case 'O':
		kinect.open();
		break;
	case 'c': // close stream
	case 'C':
		kinect.close();
		break;
	case OF_KEY_UP: // up the kinect angle
		angle++;
		if(angle > 27){
			angle = 27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_DOWN: // down the kinect angle
		angle--;
		if(angle < -27){
			angle = -27;
		}
		kinect.setAngle(angle);
		break;
	}
	
	if (isTesting)
	{
		if (key == '0')
		{
			testCount = 0;
			newSkelData.clear();
		}
		if (key == '1')
		{
			if (testCount == 0)
			{
				SkeletonData skeletonData;
				skeletonData.id = (int)ofRandom(6);
				vector<ofPoint> skelPoints;
				for(int j = 0; j < ofxKinectNui::SKELETON_POSITION_COUNT; j++) 
					skelPoints.push_back(ofPoint((int)ofRandom(100), (int)ofRandom(100), (int)ofRandom(100)));
				skeletonData.skelPoints = skelPoints;
				newSkelData.push_back(skeletonData);
				ofLogNotice() << "test - 0 - 1";
			}
			else if (testCount == 2)
			{
				vector<SkeletonData> tempNewSkelData;
				tempNewSkelData.push_back(newSkelData[(int)ofRandom(2)]);
				newSkelData.clear();
				newSkelData = tempNewSkelData;
				ofLogNotice() << "test - 2 - 1";
			}
			testCount = 1;
		}
		if (key == '2')
		{
			if (testCount == 0)
			{
				SkeletonData skeletonData;
				skeletonData.id = (int)ofRandom(6);
				vector<ofPoint> skelPoints;
				for(int j = 0; j < ofxKinectNui::SKELETON_POSITION_COUNT; j++) 
					skelPoints.push_back(ofPoint((int)ofRandom(100), (int)ofRandom(100), (int)ofRandom(100)));
				skeletonData.skelPoints = skelPoints;
				newSkelData.push_back(skeletonData);

				SkeletonData skeletonData2;
				int rand;
				do {
					rand = (int)ofRandom(6);
					skeletonData2.id = rand;
				} while (rand == skeletonData.id);

				vector<ofPoint> skelPoints2;
				for(int j = 0; j < ofxKinectNui::SKELETON_POSITION_COUNT; j++) 
					skelPoints2.push_back(ofPoint((int)ofRandom(100), (int)ofRandom(100), (int)ofRandom(100)));
				skeletonData2.skelPoints = skelPoints2;
				newSkelData.push_back(skeletonData2);

				ofLogNotice() << "test - 0 - 2";
			}
			else if (testCount == 1)
			{
				SkeletonData skeletonData2;
				int rand;
				do {
					rand = (int)ofRandom(6);
					skeletonData2.id = rand;
				} while (rand == skeletonDataObjects[0].id || rand == skeletonDataObjects[1].id);

				vector<ofPoint> skelPoints2;
				for(int j = 0; j < ofxKinectNui::SKELETON_POSITION_COUNT; j++) 
					skelPoints2.push_back(ofPoint((int)ofRandom(100), (int)ofRandom(100), (int)ofRandom(100)));
				skeletonData2.skelPoints = skelPoints2;
				newSkelData.push_back(skeletonData2);

				ofLogNotice() << "test - 1 - 2";
			}
			testCount = 2;
		}
	}
}


void ofApp::kinectPlugged()
{
	bPlugged = true;
}

void ofApp::kinectUnplugged()
{
	bPlugged = false;
}