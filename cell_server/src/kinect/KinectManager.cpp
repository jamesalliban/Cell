#include "KinectManager.h"


KinectManager::KinectManager()
{
    for (int i = 0; i < SKELETON_MAX; i++)
    {
        KinectSkeletonData skeletonData;
		for (int i = 0; i < 20; i++)
		{
			ofVec3f v = ofVec3f(-1, -1, -1);
			skeletonData.skeletonPositions.push_back(v);
		}
		skeletonData.dwTrackingID = -1;
		skeletonData.clientID = -1;
        trackedSkeletons.push_back(skeletonData);
    }
}

KinectManager::~KinectManager()
{
    //dtor
}



void KinectManager::init()
{
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
		//Kinetic demo networking stuff
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

}




void KinectManager::update()
{
    //if (ofGetFrameNum() % 50 == 0) printf("\n");

	bool isPrint = (ofGetFrameNum() % 60 == 0) ? true : false;


    while (receiver.hasWaitingMessages())
    {

        //trackedSkeletons.clear();

        ofxOscMessage m;
        receiver.getNextMessage(&m);

        //printf("receiver.hasWaitingMessages() \n");

		for (int i = 0; i < m.getNumArgs(); i++)
		{

		}
		
        if (m.getAddress() == "/skeleton/data")
        {
			for (int i = 0; i < 2; i++)
			{
				KinectSkeletonData* skeletonDataObject = &trackedSkeletons[i];
				int startIndex = ((20 * 3) + 2) * i;
				skeletonDataObject->clientID = m.getArgAsInt32(0);
				skeletonDataObject->dwTrackingID = m.getArgAsInt32(startIndex + 1);
				//printf("skeletonDataObject->clientID = %i, skeletonDataObject->id = %i \n", skeletonDataObject->clientID, skeletonDataObject->id);
				for (int j = 0; j < 20; j++)
				{
					skeletonDataObject->skeletonPositions[j].x = m.getArgAsFloat(startIndex + 2 + (j * 3));
					skeletonDataObject->skeletonPositions[j].y = m.getArgAsFloat(startIndex + 2 + (j * 3) + 1);
					skeletonDataObject->skeletonPositions[j].z = m.getArgAsFloat(startIndex + 2 + (j * 3) + 2);
				}
			}


			/*
            for (int i = 0; i < 20; i++)
            {
                int clientID = m.getArgAsInt32(0);

                KinectSkeletonData* skeletonData1 = &trackedSkeletons[(clientID * 2)];
                KinectSkeletonData* skeletonData2 = &trackedSkeletons[(clientID * 2) + 1];
                skeletonData1->clientID = clientID;
                skeletonData1->dwTrackingID = (long)m.getArgAsInt32(1);
                skeletonData2->clientID = clientID;
                skeletonData2->dwTrackingID = (long)m.getArgAsInt32(62);
                for (int j = 0; j < 2; j++)
                {
                    skeletonData1->skeletonPositions[j].x = m.getArgAsFloat(2 + (j * 3));
                    skeletonData1->skeletonPositions[j].y = m.getArgAsFloat(2 + (j * 3) + 1);
                    skeletonData1->skeletonPositions[j].z = m.getArgAsFloat(2 + (j * 3) + 2);
					
                    skeletonData2->skeletonPositions[j].x = m.getArgAsFloat(63 + (j * 3));
                    skeletonData2->skeletonPositions[j].y = m.getArgAsFloat(63 + (j * 3) + 1);
                    skeletonData2->skeletonPositions[j].z = m.getArgAsFloat(63 + (j * 3) + 2);

					if (isPrint)
					{
						printf("i:%i, skel0.x = %f, skel0.y = %f, skel0.z = %f\n", i, skeletonData1->skeletonPositions[j].x, skeletonData1->skeletonPositions[j].y, skeletonData1->skeletonPositions[j].z);
						printf("i:%i, skel1.x = %f, skel1.y = %f, skel1.z = %f\n", i, skeletonData2->skeletonPositions[j].x, skeletonData2->skeletonPositions[j].y, skeletonData2->skeletonPositions[j].z);
					}
				}
            }
			*/
        }
    }

//    if (ofGetFrameNum() % 60 == 0)
//    {
//        for (int i = 0; i < (int)trackedSkeletons.size(); i++)
//        {
//            printf("hip0.x = %f, hip1.x = %f \n", trackedSkeletons[0].skeletonPositions[0].x, trackedSkeletons[1].skeletonPositions[0].x);
//            //printf("skel %i, trackedSkeletons.size() = %i, clientID = %i \n", i, (int)trackedSkeletons.size(), trackedSkeletons[i].clientID);
//        }
//    }
}


void KinectManager::draw()
{
    ofSetHexColor(0xffffff);

	if(kinectFailedToInit)
	{
		ofSetHexColor(0xff0000);
		ofDrawBitmapString("No Kinect Device Found", padding, padding, 0);
		return;
	}
}


void KinectManager::keyPressed(int key)
{
	//Kinetic demo networking stuff
	// we are using certain keys to map to commands to send to the clients for ease of use
	ServerSendCommandstoClients(key );
	//printf("Key %d\r\n", key);
}


bool KinectManager::hasSkeleton()
{
    for (int i = 0; i < SKELETON_MAX; i++)
    {
        if (trackedSkeletons[i].dwTrackingID > -1) return true;
    }

    return false;
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




