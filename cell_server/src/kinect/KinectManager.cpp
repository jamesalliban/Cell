#include "KinectManager.h"
#include "testApp.h"


void KinectManager::init()
{
	for (int i = 0; i < SKELETON_MAX; i++)
    {
        KinectSkeletonData skeletonData;
		for (int i = 0; i < 20; i++)
		{
			ofVec3f v = ofVec3f(-1, -1, -1);
			skeletonData.skeletonPositions.push_back(v);
		}
		skeletonData.trackingID = "-1";
		skeletonData.clientID = (int)(i / 2);
		skeletonData.skelID = i % 2;
        trackedSkeletons.push_back(skeletonData);
    }

	for (int i = 0; i < SKELETON_MAX / 2; i++) 
	{
		clientConnected[i] = false;
		framesSinceClientConnected[i] = 0;
	}
    padding = 12;

	/*
        In order to setup the networking of skeletal data a configuration file is required this file has a simple structure
		and should contain just the following lines only

		127.0.0.1
		11999
		1
		clientname

		The first line should be the IP address of the server laptop/pc that will receive skletons data
		from the clients, in the above case it set to local host since we are testing back to back on same laptop

		The second line should be the Port that the server should use to listen on and the clients should connect to.

		The third line is a flag if set to 1 then the client will send Position-Only skeletal information, that is
		information for up to 4 non tracked selectons, in addition to sending information for the tracked skeltons.
		If the third line is set to 0 then Postion-Only skeletal information will not be sent and the server will
		not be expecting it.

		The forth line (which is optional) is the clientname ( shown above as the string "clientname" ) this is the name of the client sending the data. This
		is to allow the application on the server to identify where the data came from i.e. which laptop/pc,
		assuming that the name in the respective files on the laptop/pc clients is set differently.
		I'm assuming here that the skeletons from the clients might need to be display in a certain order on the screen of the server
		corresponding to the position of the Kinetic devices or something of this kind.
		The string can be up to 29 characters long anything more will be truncated.

		The same file (apart from the client name) should be copied to all clients and the server laptop/pc's and should contain exactly the
		same data. There is minimal checking on validity of data in this file when read in.
*/
		///set up the server giving the full path to the configuration file

    ofFile file;
	if( file.open("c:\\ipaddress.txt") )
    {
        ofBuffer buf = file.readToBuffer() ;
        file.close();
        string ipaddress_of_server = buf.getFirstLine();
        string port = buf.getNextLine();
        int iport = atoi( port.c_str() );
    }

    receiver.setup(PORT_IN);
    //sender0.setup(CLIENT0_IP, PORT_OUT);

    kinectFailedToInit = false;

    bSend = true;
    iReadCommand = 0;

	recordedFramesMax = 1000;
	recordingPixels.allocate(60*SKELETON_MAX + SKELETON_MAX, recordedFramesMax, OF_IMAGE_COLOR);
	isRecording = false;
	isPlayback = false;
	isPlaybackPaused = false;
	
	currentPlayingScene = 0;
	framesRecorded = 0;
						
}




void KinectManager::update()
{
	bool isPrint = (ofGetFrameNum() % 60 == 0) ? true : false;

	if (!isPlayback)
	{
		checkForOSCKinectData();
	}
	else
	{
		playRecordedLine();
	}
	
	for (int i = 0; i < SKELETON_MAX / 2; i++) 
	{
		//clientConnected[i] = false;
		++framesSinceClientConnected[i];
		if (framesSinceClientConnected[i] > 10) clientConnected[i] = false;
		else clientConnected[i] = true;
	}
}


void KinectManager::checkForOSCKinectData()
{
    while (receiver.hasWaitingMessages())
    {
        //trackedSkeletons.clear();

        ofxOscMessage m;
        receiver.getNextMessage(&m);
		
        if (m.getAddress() == "/skeleton/data")
        {
			//if (ofGetFrameNum() % 10 == 0) 
			//	printf("/skeleton/data message - client = %i\n", m.getArgAsInt32(0));

			// 0 = client
			// 1 = skelID
			// 2 = isActive
			// 3 - 62 = joint data
			// 63 - skelID
			// 64 - isActive
			// 65 - 124 = joint data

			for (int i = 0; i < 2; i++)
			{
				int startIndex = (i == 0) ? 1 : 63;

				int clientId = m.getArgAsInt32(0);
				int skelId = m.getArgAsInt32(startIndex);
				
				framesSinceClientConnected[clientId] = 0;

				KinectSkeletonData* skeletonDataObject = &trackedSkeletons[clientId * 2 + skelId];

				skeletonDataObject->clientID = clientId;
				skeletonDataObject->skelID = skelId;
				

				int isActive = m.getArgAsInt32(startIndex + 1);

				// if the skeleton is inactive make the trackingID '-1'. If active make it clientID_skelID e.g. 0_1, 2_0 etc
				if (m.getArgAsInt32(startIndex + 1) == -1)
					skeletonDataObject->trackingID = "-1";
				else
					skeletonDataObject->trackingID = ofToString(skeletonDataObject->clientID) + "_" + ofToString(skeletonDataObject->skelID);
				

				for (int j = 0; j < 20; j++)
				{
					skeletonDataObject->skeletonPositions[j].x = m.getArgAsFloat(startIndex + 2 +  (j * 3));
					skeletonDataObject->skeletonPositions[j].y = m.getArgAsFloat(startIndex + 2 + (j * 3) + 1);
					skeletonDataObject->skeletonPositions[j].z = m.getArgAsFloat(startIndex + 2 + (j * 3) + 2);
				}
			}

        }
    }
	if (isRecording)
	{
		for (int i = 0; i < SKELETON_MAX; i++)
		{
			KinectSkeletonData* skeletonDataObject = &trackedSkeletons[i];
		
			int isActive = (skeletonDataObject->skeletonPositions[00].x != -1) ? 1 : -1;
		
			if (framesRecorded > recordingPixels.getHeight() - 30) stopRecording();
			addSkelDataToRecording(isActive, skeletonDataObject->clientID, skeletonDataObject->skelID);

			for (int j = 0; j < 20; j++)
			{
				if (isRecording && isActive)
					addJointToRecording(skeletonDataObject->clientID, skeletonDataObject->skelID, j, skeletonDataObject->skeletonPositions[j], isActive);
			}

		}
		++framesRecorded;
	}
}


void KinectManager::playRecordedLine()
{
	for (int client = 0; client < SKELETON_MAX * 0.5; client++)
	{
		for (int skel = 0; skel < 2; skel++)
		{
			KinectSkeletonData* skeletonDataObject = &trackedSkeletons[client * 2 + skel];
			int startX = (client * 120) + (skel * 60) + (client * 2) + skel;

			// red (0 or 1) = isActive,  green = client, blue = skelId;
			int isActive = (recordingPixels.getColor(startX, currentPlaybackFrame).r == 0) ? -1 : 1;
			skeletonDataObject->clientID = recordingPixels.getColor(startX, currentPlaybackFrame).g;
			skeletonDataObject->skelID = recordingPixels.getColor(startX, currentPlaybackFrame).b;

			// if the skeleton is inactive make the trackingID '-1'. If active make it clientID_skelID e.g. 0_1, 2_0, 3_1 etc
			if (isActive == -1)
				skeletonDataObject->trackingID = "-1";
			else
				skeletonDataObject->trackingID = ofToString(skeletonDataObject->clientID) + "_" + ofToString(skeletonDataObject->skelID);
				
			if (isActive != -1)
			{
				for (int joint = 0; joint < 20; joint++)
				{
					skeletonDataObject->skeletonPositions[joint].x = getCoordFromCol(recordingPixels.getColor(startX + 1 + (joint * 3), currentPlaybackFrame));
					skeletonDataObject->skeletonPositions[joint].y = getCoordFromCol(recordingPixels.getColor(startX + 2 + (joint * 3), currentPlaybackFrame));
					skeletonDataObject->skeletonPositions[joint].z = getCoordFromCol(recordingPixels.getColor(startX + 3 + (joint * 3), currentPlaybackFrame));
				}
			}
			else
			{
				for (int joint = 0; joint < 20; joint++)
				{
					skeletonDataObject->skeletonPositions[joint].x = -1;
					skeletonDataObject->skeletonPositions[joint].y = -1;
					skeletonDataObject->skeletonPositions[joint].z = -1;
				}
			}
		}
	}

    if (!isPlaybackPaused)
        if(++currentPlaybackFrame == recordedFramesMax) currentPlaybackFrame = 0;
}



void KinectManager::draw()
{
    if (!isRecordedImageVisible) return;
    
    if (isRecording || isPlayback)
	{
		recordingImg.setFromPixels(recordingPixels);
		
        ofDisableDepthTest();
        
        ofPushStyle();
        ofSetColor(255, recordedImageAlpha);
        ofPushMatrix();
        ofTranslate(ofGetWidth() - recordingPixels.getWidth() - 10, 0);
		recordingImg.draw(0, 0);
		ofSetColor(255, 0, 0);
		ofLine(0, currentPlaybackFrame, recordingPixels.getWidth(), currentPlaybackFrame);
		ofSetColor(255);
        ofDrawBitmapString(currentPlaybackPath, 10, 10);
        ofPopMatrix();
        ofPopStyle();
        
        ofEnableDepthTest();
	}
}


void KinectManager::skipToFrame(int x, int y)
{
    ofRectangle rect = ofRectangle(ofGetWidth() - recordingPixels.getWidth() - 10, 0, recordingPixels.getWidth(), recordingPixels.getHeight());
    if (rect.inside(x, y))
    {
        currentPlaybackFrame = y;
    }
}

bool KinectManager::hasSkeleton()
{
    for (int i = 0; i < SKELETON_MAX; i++)
    {
        if (trackedSkeletons[i].trackingID != "-1") return true;
    }

    return false;
}


void KinectManager::startRecording()
{
	if (isRecording || isPlayback) return;
	printf("startRecording()\n");
	recordingPixels.clear();
	recordingPixels.allocate(60 * SKELETON_MAX + SKELETON_MAX, recordedFramesMax, OF_IMAGE_COLOR);
	isRecording = true;
	framesRecorded = 0;
}




void KinectManager::stopRecording()
{
	printf("stopRecording()\n");
	isRecording = false;
	isPlayback = false;
}




void KinectManager::saveRecording()
{
	if (!isRecording)
		ofSaveImage(recordingPixels, "images/recordedSkeletons/NEW_" + ofGetTimestampString() + ".png", OF_IMAGE_QUALITY_BEST);
}



void KinectManager::startPlayback(string recordedPath)
{
	if (ofGetFrameNum() < 10) return;

	if (!isRecording)
	{
		if (recordedPath != "")
		{
			recordingImg.loadImage(recordedPath);
            currentPlaybackPath = recordedPath;
			recordingPixels.setFromPixels(recordingImg.getPixels(), recordingImg.getWidth(), recordingImg.getHeight(), OF_IMAGE_COLOR);
		}
		currentPlaybackFrame = recordingImg.getHeight() - 2;
		isPlayback = true;
	}
}



void KinectManager::clearPlayback()
{
    if (isPlayback && !isRecording)
    {
        isPlayback = false;
        isPlaybackPaused = false;
        recordingImg.clear();
        recordingPixels.clear();
		currentPlaybackFrame = recordingImg.getHeight() - 5;
        
        for (int client = 0; client < 3; client++)
        {
            for (int skel = 0; skel < 2; skel++)
            {
                KinectSkeletonData* skeletonDataObject = &trackedSkeletons[client * 2 + skel];
                skeletonDataObject->trackingID = "-1";
            }
        }
    }
}


void KinectManager::addSkelDataToRecording(int isActive, int client, int id)
{
	ofColor skelData;
	// isActive
	skelData.r = (isActive == 1) ? 1 : 0;
	// id
	skelData.g = client;
	//client
	skelData.b = id;

	int x = (client * 120) + (id * 60) +  (client * 2) + id;
	recordingPixels.setColor(x, framesRecorded, skelData); 
}


void KinectManager::addJointToRecording(int client, int id, int jointId, ofVec3f joint, bool isActive)
{
	// convert the incoming joint vector to 3 pixels - one each for x, y, z.
	// r is the amount of times the number can be divided by 255
	// g is the remainder
	// b is whether the number is positive or negative

	int x = (client * 120) + (id * 60) + (jointId * 3) +  (client * 2) + id;

	ofColor colX;
	colX.r = (int)joint.x / 255;
	colX.g = (int)joint.x % 255;
	colX.b = (joint.x < 0) ? 0 : 255;
	recordingPixels.setColor(x + 1, framesRecorded, colX); 
	
	ofColor colY;
	colY.r = (int)joint.y / 255;
	colY.g = (int)joint.y % 255;
	colY.b = (joint.y < 0) ? 0 : 255;
	recordingPixels.setColor(x + 2, framesRecorded, colY); 
	
	ofColor colZ;
	colZ.r = (int)joint.z / 255;
	colZ.g = (int)joint.z % 255;
	colZ.b = (joint.z < 0) ? 0 : 255;
	recordingPixels.setColor(x + 3, framesRecorded, colZ);
}


float KinectManager::getCoordFromCol(ofColor col)
{
	float coord = ((col.r * 255) + col.g) * ((col.b == 0) ? -1 : 1);
	//printf("r:%i, g:%i, b:%i, coord:%f, (col.r * 255):%i, mod mult:%i \n", col.r, col.g, col.b, coord, (col.r * 255), ((col.b == 0) ? -1 : 1));
	return coord;
}



//here we show examples of commands that can be sent from server to clients .. in this case we are driving things
//from the keyboard on the server using the keypressed callback function to call into this function
void  KinectManager::ServerSendCommandstoClients(int key )
{
    /*


	if( nwserver == NULL ) // no server so get out now
		return ;

	if(key == OF_KEY_UP)
	{
		//cameraTiltVal++;
		//nwserver->SetCameraElevation( cameraTiltVal ) ;
		return ;
	}

	if(key == OF_KEY_DOWN)
	{
		//cameraTiltVal--;
		//nwserver->SetCameraElevation( cameraTiltVal ) ;
		return ;
	}

	if(key == 'P' || key == 'p')
	{
		nwserver->Pause(); // send a pause command allows client to set a flag and stop sending data
		return ;
	}

	if(key == 'R' || key == 'r')
	{
		//nwserver->Resume() ;// send a resume command allows client to reset its send flag
		return ;
	}

	if(key == 'X' || key == 'x')
	{//send command to clients to close down and exit
		//ccnwserver->CloseClients() ;
		return ;
	}

	// here we send smoothing params to all clients... in this case we just get current values
	// and send them as is.. rather that set them to anything specific
	if(key == 'S' || key == 's')
	{
		if( firstDevice != NULL )
			{
				NUI_TRANSFORM_SMOOTH_PARAMETERS inptr;    // get and set skeleton smoothing params
				firstDevice->GetTransformSmoothParamaters( &inptr );
				nwserver->SetClientsSmoothingParams( (char *) &inptr, sizeof(NUI_TRANSFORM_SMOOTH_PARAMETERS) ) ;
			}
		return ;
	}

	if(key == 'Z' || key == 'z')
	{//tell the clients the server is closing.. the clients should stop sending data but will attempt to reconnect
	 //When the server is restarted it should send a nwserver->Resume() ;  message so the client can set its sendata flag
	 // and resume sending data.
		//nwserver->NotifyClientsServerClosing();
		return;
	}

	*/
}




