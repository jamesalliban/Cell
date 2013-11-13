
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// WARNING /////////////////////////////////////////
////////// THIS APP WILL RUN VERY SLOWLY IF THE NETWORK IS NOT SET UP. TO TEST /////////
///////////////// COMMENT OUT THE OSC SENDER MESSAGE CODE IN UPDATE() //////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#include "testApp.h"

void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

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

	kinect.init(false, false, false, true, true, false, false, true); // enable all captures
	kinect.open(false);
//	kinect.open(true); // when you want to use near mode

	kinect.addKinectListener(this, &testApp::kinectPlugged, &testApp::kinectUnplugged);
	
	ofSetVerticalSync(true);

	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	nearClipping = 0;
	farClipping = kinect.getFarClippingDistance();
	farClipping = 10000;

	bDrawVideo = false;
	bDrawDepthLabel = false;
	bDrawSkeleton = false;
	bDrawCalibratedTexture = false;

	ofSetFrameRate(40);
	
	calibratedTexture.allocate(320, 240, GL_RGB);

	sender.setup(serverIPAddress, PORT_OUT);
    //receiver.setup(PORT_IN);
	
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
}


void testApp::buildSkeletonDataObjects()
{
	for (int i = 0; i < 2; i++)
	{
        SkeletonData skelData;
		skelData.id = -1;
		skelData.arrayIndex = -1;
		skelData.resetCount = 0;
		skelData.isActive = false;
		for (int i = 0; i < 20; i++)
			skelData.skelPoints.push_back(ofVec3f(-1, -1, -1));
		
		skeletonDataObjects.push_back(skelData);
	}
}


void testApp::resetSkeletonData(int index)
{
    SkeletonData* skelData = &skeletonDataObjects[index];
	if (++skelData->resetCount == 6)
	{
		skelData->id = -1;
		skelData->arrayIndex = -1;
		skelData->resetCount = 0;
		skelData->isActive = false;
		for (int i = 0; i < 20; i++)
			skelData->skelPoints[i] = ofVec3f(-1, -1, -1);
	}
}


void testApp::populateSkeletonData(int skeletonPointsIndex, int skeletonDataObjectIndex, bool isSkelNew)
{
	ofPoint* points = kinect.getSkeletonPoints()[skeletonPointsIndex];
	SkeletonData* skelData = &skeletonDataObjects[skeletonDataObjectIndex];

	skelData->resetCount = 0;

	if (isSkelNew) 
	{
		skelData->id = skeletonDataObjectIndex + (clientID * 2);
		skelData->arrayIndex = skeletonPointsIndex;
		skelData->isActive = true;
	}
	for (int i = 0; i < 20; i++)
		skelData->skelPoints[i] = points[i];
}


void testApp::update() 
{
	if (ofGetElapsedTimeMillis() < 2000) return;

	kinectSource->update();
	
	//Loop through all skeleton containers from ofxKinectNui
	for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; i++)
	{
		ofPoint skelPoint = kinect.getSkeletonPoints()[i][0];
		//is this ofxKinectNui skeleton is empty
		if (skelPoint.x == -1 && skelPoint.y == -1 && skelPoint.z == -1) 
		{
			//Check SkeletonData objects to see if there is a skel with this array index and if it was 
			//previously active. If so, reset the SkeletonData object
			for (int j = 0; j < (int)skeletonDataObjects.size(); j++)
				if (skeletonDataObjects[j].arrayIndex == i && skeletonDataObjects[j].isActive)
					resetSkeletonData(j);
		}
		
		
		//if ofxKinectNui skeleton is active 
		if (skelPoint.x != -1 && skelPoint.y != -1 && skelPoint.z != -1) 
		{
			bool hasNewSkeletonBeenAsigned = false;
			//loop through SkeletonData objects to see if one has this arrayIndex
			for (int j = 0; j < (int)skeletonDataObjects.size(); j++)
			{
				SkeletonData* skelData = &skeletonDataObjects[j];
				//if there is a match, populate it with the data from this skeleton
				if (skelData->arrayIndex == i)
				{
					populateSkeletonData(i, j, false);
					hasNewSkeletonBeenAsigned = true;
				}
				if (hasNewSkeletonBeenAsigned) break;
			}

			//the skeleton has not been assigned so it is new. 
			if (!hasNewSkeletonBeenAsigned)
			{
				//Loop through the SkeletonData objects to find the first empty one, assign it to this 
				//array index and populate it with data
				for (int j = 0; j < (int)skeletonDataObjects.size(); j++)
				{
					SkeletonData* skelData = &skeletonDataObjects[j];
					if (!skelData->isActive)
					{
						populateSkeletonData(i, j, true);
						break;
					}
				}
			}
		}
	}
	

	for (int i = 0; i < (int)skeletonDataObjects.size(); i++)
	{
		SkeletonData* skelData = &skeletonDataObjects[i];
		//for (int j = 0; j < 20; j++)
			//printf("i = %i, j = %i, x = %f, y = %f, z = %f \n",i, j, skelData->skelPoints[j].x, skelData->skelPoints[j].y, skelData->skelPoints[j].z);
	}

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
}

//--------------------------------------------------------------
void testApp::draw() 
{
	ofBackground(100, 100, 100);
	if(bDrawVideo)
	{
		kinect.draw(0, 0, 1024, 768);	// draw video images from kinect camera
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
	else if(bDrawCalibratedTexture)
	{
		ofPushMatrix();
		ofPopMatrix();
	}
	else
	{
		float sceneDrawScale = 0.75;
		float w = 480 * sceneDrawScale;
		float h = 360 * sceneDrawScale;
		calibratedTexture.loadData(kinect.getCalibratedVideoPixels());
		calibratedTexture.draw(0, 0, w, h);
		//kinect.draw(0, 0, w, h);			// draw video images from kinect camera
		ofEnableAlphaBlending();
		kinect.drawDepth(0, h, w, h);	// draw depth images from kinect depth sensor
		kinect.drawLabel(0, h, w, h);		// draw players' label images on video images
		ofDisableAlphaBlending();
		kinect.drawSkeleton(0, 0, w, h);	// draw skeleton images on video images
		
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 80);
		kinect.drawLabel(0, 0, w, h);		// draw players' label images on video images	
		ofDisableAlphaBlending();
		//kinect.drawSkeleton(w, 0, 640, 480);	// draw skeleton images on video images

	}

	ofPushMatrix();
	ofTranslate(35, 35);
	ofFill();
	ofPopMatrix();

	stringstream kinectReport;
	if(bPlugged && !kinect.isOpened())
	{
		ofSetColor(0, 255, 0);
		kinectReport << "Kinect is plugged..." << endl;
		ofDrawBitmapString(kinectReport.str(), 200, 300);
	}
	else if(!bPlugged)
	{
		ofSetColor(255, 0, 0);
		kinectReport << "Kinect is unplugged..." << endl;
		ofDrawBitmapString(kinectReport.str(), 200, 300);
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	//reportStream << "fps: " << ofGetFrameRate() << "  Kinect Nearmode: " << kinect.isNearmode() << endl
	//			 << "press 'c' to close the stream and 'o' to open it again, stream is: " << kinect.isOpened() << endl
	//			 << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
	//			 << "press LEFT and RIGHT to change the far clipping distance: " << farClipping << " mm" << endl
	//			 << "press '+' and '-' to change the near clipping distance: " << nearClipping << " mm" << endl
	//			 << "press 'v' to show video only: " << bDrawVideo << ",      press 'd' to show depth + users label only: " << bDrawDepthLabel << endl
	//			 << "press 's' to show skeleton only: " << bDrawSkeleton << ",   press 'q' to show point cloud sample: " << bDrawCalibratedTexture;
	
	
	reportStream << "fps: " << ofGetFrameRate() << endl
				<< "'c': close stream" << endl
				<< "'o': open stream" << endl;

	ofDrawBitmapString(reportStream.str(), 20, 600);
	
	stringstream kinect0Stream;
	SkeletonData* skelData = &skeletonDataObjects[0];
	kinect0Stream << "skel id: " << skelData->id << endl;
	for (int i = 0; i < 20; i++)
	{
		kinect0Stream << joints[i] << ", ";
		kinect0Stream << "x: " << skelData->skelPoints[i].x << ", ";
		kinect0Stream << "y: " << skelData->skelPoints[i].y << ", ";
		kinect0Stream << "z: " << skelData->skelPoints[i].z << endl;
	}

	float textScale = 1.8;

	ofPushMatrix();
	ofTranslate(400, 30);
	ofScale(textScale, textScale);
	ofDrawBitmapString(kinect0Stream.str(), 0, 0);
	ofPopMatrix();
	
	stringstream kinect1Stream;
	skelData = &skeletonDataObjects[1];
	kinect1Stream << "skel id: " << skelData->id << endl;
	for (int i = 0; i < 20; i++)
	{
		kinect1Stream << joints[i] << " ";
		kinect1Stream << "x: " << skelData->skelPoints[i].x << ", ";
		kinect1Stream << "y: " << skelData->skelPoints[i].y << ", ";
		kinect1Stream << "z: " << skelData->skelPoints[i].z << endl;
	}
	ofPushMatrix();
	ofTranslate(850, 30);
	ofScale(textScale, textScale);
	ofDrawBitmapString(kinect1Stream.str(), 0, 0);
	ofPopMatrix();
	
}


//--------------------------------------------------------------
void testApp::exit() 
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
void testApp::keyPressed (int key) 
{
	switch(key){
	case 'v': // draw video only
	case 'V':
		bDrawVideo = !bDrawVideo;
		if(bDrawVideo){
			bDrawCalibratedTexture = false;
			bDrawSkeleton = false;
			bDrawDepthLabel = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
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
	case 'q': // draw point cloud example
	case 'Q':
		bDrawCalibratedTexture = !bDrawCalibratedTexture;
		if(bDrawCalibratedTexture){
			bDrawVideo = false;
			bDrawDepthLabel = false;
			bDrawSkeleton = false;
			glEnable(GL_DEPTH_TEST);
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
	case OF_KEY_LEFT: // increase the far clipping distance
		if(farClipping > nearClipping + 10){
			farClipping -= 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case OF_KEY_RIGHT: // decrease the far clipping distance
		if(farClipping < 4000){
			farClipping += 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case '+':
		if(nearClipping < farClipping - 10){
			nearClipping += 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	case '-':
		if(nearClipping >= 10){
			nearClipping -= 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	}
}


void testApp::kinectPlugged()
{
	bPlugged = true;
}

void testApp::kinectUnplugged()
{
	bPlugged = false;
}