
#pragma once

#include <stdlib.h>
#include <Windows.h>
#include "testApp.h"
#include "ofxMSKinect.h"
#include "ofxMSSkeletonDraw.h"
//Kinetic demo networking stuff
//#include "KineticNetworkClient.h"
//end Kinetic demo networking stuff
#include "ofFileUtils.h";

ofxMSKinect* kinect;
ofxMSKinectDevice* firstDevice;
int padding = 12;

bool bSendDataToServer = true  ; // flag added to allow client data sending to be controled by server. see function Update() below
//NOTE IF this flag is set false at start of client so the server MUST send a Resume command before any data is sent from client
//Set the flag to true if data should be sent as soon as it is available
//end Kinetic demo networking stuff

ofxSkeletonRenderer* skeletonDrawer; // simple class to draw skeletons


void testApp::setup()
{
    ofSetFrameRate(30);  // set the frame rate this may need to be adjusted based on performance of network and laptops/pc's

    sender.setup(SERVER_IP, PORT_OUT);
    receiver.setup(PORT_IN);

	//this will initialize everything we need for basic kinect functionality
	//this will also check if the device is connected and handle gracefully
	kinect = new ofxMSKinect();
	kinect->Init(SINGLE_DEVICE); //Pass MULTIPLE_DEVICES if you intend to use more than 1 kinect devices physically attached  to the same laptop/pc.

	if(kinect->HasKinectDevice())
	{
		skeletonDrawer = new ofxSkeletonRenderer();
		ofFile file;

/*		In order to setup the networking of skeletal data a configuration file is required this file has a simple structure
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

		//set up the client
		if( file.open("c:\\ipaddress.txt") )
		{
			ofBuffer buf = file.readToBuffer() ;
			file.close();
			string ipaddress_of_server = buf.getFirstLine();
			string port = buf.getNextLine();
			int iport = atoi( port.c_str() );
			string tracked_only = buf.getNextLine();
			int itracked = atoi( tracked_only.c_str() );
			client_name = buf.getNextLine();
        }
		//end Kinetic demo networking stuff


		firstDevice = kinect->GetKinectDeviceDefault();

    //	NUI_TRANSFORM_SMOOTH_PARAMETERS inptr;    // get and set skeleton smoothing params
	//	firstDevice->GetTransformSmoothParamaters( &inptr );
	//	firstDevice->SetTransformSmoothParamaters( &inptr );
    //  or call individual functions for each of NUI_TRANSFORM_SMOOTH_PARAMETERS properties e.g
	//	firstDevice->SetSmoothing(f);
	//	float f  = firstDevice->GetSmoothing() ;
		if( firstDevice != NULL )
		{
		// image res for RGB has to be this set by the NUI api .lower res returns failure
			bool isOk = firstDevice->StartRGB(IMAGE_RESOLUTION_640x480) ;

		// default depth resolution is 320x240
			isOk = firstDevice->StartDepth(IMAGE_RESOLUTION_320x240);

		//start tracking skeleton
			isOk = firstDevice->StartSkeletonTracking();
		}
		kinectFailedToInit = false;
	}
	else
	{
		kinectFailedToInit = true;
	}
}

bool bSend = true;
int  iReadCommand = 0;
void testApp::update()
{
    if (kinectFailedToInit) return;

    /*
    Data sent in this format
    0 = client client0, client1 etc - sting

    Skel 1
    1 = dwTrackingID - int
    2-61 = skel vertex data - float

    */

	ofxOscMessage m;
	m.setAddress("/skeleton/data");

	char clientIDChar = client_name[6];
    int clientID = atoi(&clientIDChar);
	m.addIntArg(clientID);

	//printf("tracked_skeletons.size() = %i \n", (int)firstDevice->tracked_skeletons.size());

	for(int i = 0; i < 2; i++)
    {
        bool skelActive = true;

        if ((int)firstDevice->tracked_skeletons.size() <= i) skelActive = false;

        m.addIntArg((skelActive) ? (int)firstDevice->tracked_skeletons[i].dwTrackingID : -1);



        for(int j = 0; j < 20; j++)
        {
            m.addFloatArg((skelActive) ? firstDevice->tracked_skeletons.at(i).SkeletonPositions[j].x : -1000);
            m.addFloatArg((skelActive) ? firstDevice->tracked_skeletons.at(i).SkeletonPositions[j].y : -1000);
            m.addFloatArg((skelActive) ? firstDevice->tracked_skeletons.at(i).SkeletonPositions[j].z : -1000);
        }

        if (ofGetFrameNum() % 60 == 0)
        {
            printf("i = %i, size() = %i, dwTrackingID = %i, hip.x = %f \n", i, (int)firstDevice->tracked_skeletons.size(), (skelActive) ? (int)firstDevice->tracked_skeletons[i].dwTrackingID : -1, (i == 0) ? m.getArgAsFloat(2) : m.getArgAsFloat(63));
        }
    }

    if (ofGetFrameNum() % 60 == 0)
    {
        printf("\n");
    }

    //m.addFloatArg((float)ofRandom(0, 100));

    sender.sendMessage(m);

	if(kinectFailedToInit)
		return;

	if( firstDevice != NULL)
	{
		bool isOk = firstDevice->UpdateRGBVideo();
		isOk = firstDevice->UpdateDepth() ;
		isOk = firstDevice->UpdateSkeletons();
	}

	//Kinetic demo networking stuff
	//here is where we send data to the server


/*
    // we always send .. if there is no data then server will get empty skels which is fine
    if( nwclient != NULL && bSendDataToServer) // this flag can be set by server commands to enable disable sending data
    {
        nwclient->Send ( &firstDevice->tracked_skeletons, &firstDevice->positiononly_skeletons ) ;
    }
    */

	// here we show how to handle commands sent from the server to the client ...
	// if the app is running as a client
	//ClientProcessesCommandFromServer() ;
	//end Kinetic demo networking stuff
}

//Kinetic demo networking stuff
	/*
void testApp::ClientProcessesCommandFromServer()
{
	//here we show examples of handling commands on the client which are sent from the server
	//iReadCommand used to control how often we check for incoming data. The Update() function is called every 16 msec (normally)
	//so we are checking for commands sent from server every 160 msec here ... since don't want to do it every Update() call


	if( nwclient != NULL &&  iReadCommand++ > 30)
	{ // here we read commands sent from the server ...
	    iReadCommand = 0 ;
		ofMS_NETWORK_COMMAND command;
		if( nwclient->GetCommand(&command) )
		{
			switch( command.command ) //we have got a command
			{
				case 'C':  // a camera elevation command
				{
					int elevation = atoi( command.command_data ) ;
					if( firstDevice != NULL )
						firstDevice->SetCameraElevationAngle( elevation );
				}
				break ;

				case'P':
                    bSendDataToServer = false;
                    printf("Pause pressed \n");
                    break ; // a Pause command from server so disable data send flag

				case'R':
                    bSendDataToServer = true;
                    printf("Resume pressed \n");
                    break ; // a Resume command from server so enable data send flag

				case'X': // close client command.. so simulate Escape Key press to close the app. app must have keyboard focus though
					// done this way since I can't find a function to call to close the app programmatically
					keybd_event(OF_KEY_ESC, 0, 0, 0);
					keybd_event(OF_KEY_ESC, 0, KEYEVENTF_KEYUP, 0);
					break ;

				case 'S':  // A Smoothing command. get skeleton smoothing params sent from server and apply them
					if( firstDevice != NULL )
					{
						NUI_TRANSFORM_SMOOTH_PARAMETERS inptr;
						memcpy( &inptr, command.command_data, sizeof(NUI_TRANSFORM_SMOOTH_PARAMETERS));
						firstDevice->SetTransformSmoothParamaters( &inptr );
					}
				break;

				case'Z': // A Server closing down command.  as a client we continue running but set the send data flag to disabled
					     // to stop sending data ... and whenever the  nwclient->GetCommand(&command) )  is called
						 // the code will try to connect to the server on the specified socket and IP as set  in the config file
						 // once connected (i.e. when server restarts ) ... the client will not send any skel data until the server sends
					     // a Resume command to re-enable to send data flag

					bSendDataToServer = false ;  //stop sending data
					nwclient->CloseSocket()   ;  //close socket client will try to reconnect on each nwclient->GetCommand(&command)  call
					break ; // a Pause command from server so disable data send flag

			}
		}
	}
}
	*/


void ServerSendCommandstoClients(int key){}
int val=0;

//MJM   James I have left the drawing in on the client... though I assume it not really needed
// except perhaps for showing the skels.

void testApp::draw()
{
	ofSetHexColor(0xffffff);

	if(kinectFailedToInit)
	{
		ofSetHexColor(0xff0000);
		ofDrawBitmapString("No Kinect Device Found", padding, padding, 0);
		return;
	}

	int dwidth  = firstDevice->DepthSize.x;
	int dheight = firstDevice->DepthSize.y;

	//Draw depth
	firstDevice->DrawDepthTexture(padding, padding, dwidth, dheight);

	ofSetHexColor(0xffffff);

	//Draw video (scale to size of depth for debug)
	firstDevice->DrawVidImage(padding + padding + dwidth, padding, dwidth, dheight);

//	skeletonDrawer->Draw(padding, firstDevice->DepthSize, firstDevice->HasSkeleton(),  &firstDevice->tracked_skeletons);
//	skeletonDrawer->DrawJoints(padding, firstDevice->DepthSize.x, firstDevice->DepthSize.y, firstDevice->HasSkeleton(),  &firstDevice->skeletons );
//	skeletonDrawer->DrawLimbs(padding, firstDevice->DepthSize.x, firstDevice->DepthSize.y, firstDevice->HasSkeleton(),  &firstDevice->skeletons );
//end Kinetic demo networking stuff

    //if( nwclient != NULL && firstDevice != NULL)

    skeletonDrawer->Draw(padding, firstDevice->DepthSize, firstDevice->HasSkeleton(),  &firstDevice->tracked_skeletons);

	drawDebug();
	// or call firstDevice->Draw( padding ) to call convenience Draw function ;
}


void testApp::drawDebug()
{
	ofSetHexColor(0x0);
	int dwidth = firstDevice->DepthSize.x;

	int leftOffset = padding + (dwidth * 2) + padding + padding;

	string name = firstDevice->GetDeviceName();
	int yPos = padding;

	ostringstream ostr;

	ostr << "Device Name: " << name << "\r\n";

	ostr << "Connected :" << (firstDevice->IsConnected() ? "yes" : "no") << "\r\n";

	ostr << "RGB Size: " << firstDevice->VideoSize.x << " - " << firstDevice->VideoSize.y << "\r\n";

	ostr << "Depth Size: " << firstDevice->DepthSize.x << " - " << firstDevice->DepthSize.y << "\r\n";

	ostr << "Depth Has Player Info: " << (firstDevice->DepthHasPlayers() ? "yes" : "no") << "\r\n";

	bool hasSkeleton = firstDevice->HasSkeleton();
	ostr << "Has Skeletons: " << (hasSkeleton? "yes" : "no") << "\r\n";
	ostr << "Tracked      : " << firstDevice->NumberSkeletonsTracked() << "\r\n";
	ostr << "Position Only: " << firstDevice->positiononly_skeletons.size()  << "\r\n";

	ofDrawBitmapString(ostr.str(), leftOffset, yPos, 0);
}


void testApp::keyPressed(int key)
{
	if(key == OF_KEY_UP)
	{
		val++;
		firstDevice->SetCameraElevationAngle(val);
	}
	if(key == OF_KEY_DOWN)
	{
		val--;
		firstDevice->SetCameraElevationAngle(val);
	}

	printf("Key %d\r\n", key);
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button){

}

void testApp::mousePressed(int x, int y, int button){

}

void testApp::mouseReleased(int x, int y, int button){

}

void testApp::windowResized(int w, int h){

}

void testApp::gotMessage(ofMessage msg){

}

void testApp::dragEvent(ofDragInfo dragInfo){

}

void testApp::exit()
{
	if(kinect != NULL)
	{
		kinect->Close();

		delete kinect;
		kinect = NULL;
	}

	if( firstDevice != NULL )
	{
		delete firstDevice;
		firstDevice = NULL;
	}

	if( skeletonDrawer  != NULL )
	{
		delete skeletonDrawer;
		skeletonDrawer = NULL;
	}
}
