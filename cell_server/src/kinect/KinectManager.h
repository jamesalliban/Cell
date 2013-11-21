#pragma once

#include "ofMain.h"

#include "ofFileUtils.h"
#include "ofxOsc.h"

#include "KinectSkeletonData.h"

#include "CellGlobals.h"

// server IP should be 192.168.0.199
#define CLIENT0_IP "192.168.0.200"
#define CLIENT1_IP "192.168.0.201"
#define CLIENT2_IP "192.168.0.202"
#define CLIENT3_IP "192.168.0.203"
#define PORT_OUT 12345
#define PORT_IN 12346


class KinectManager
{
public:
    void init();
    void update();
	void checkForOSCKinectData();
	void playRecordedLine();
    void draw();
    void skipToFrame(int x, int y);
    bool hasSkeleton();
    void ServerSendCommandstoClients(int key);

	void startRecording();
	void stopRecording();
	void saveRecording();
	void startPlayback(string recordedPath = "");
	void clearPlayback();
    
	void addSkelDataToRecording(int isActive, int client, int id);
	void addJointToRecording(int client, int id, int jointId, ofVec3f joint);
	float getCoordFromCol(ofColor col);

    vector<KinectSkeletonData> trackedSkeletons;
    int padding;
    long val;


	ofPixels recordingPixels;
	ofImage recordingImg;
	bool isRecording;
	bool isPlayback;
    bool isPlaybackPaused;
	int currentPlayingScene;
	int framesRecorded;
	int currentPlaybackFrame;
	int recordedFramesMax;
	
	float recordedImageAlpha;

protected:
private:

    ofxOscReceiver receiver;
//    ofxOscSender sender0;
//    ofxOscSender sender1;
//    ofxOscSender sender2;
//    ofxOscSender sender3;

    bool kinectFailedToInit;
    bool bSend;
    int  iReadCommand;
    int  cameraTiltVal;
};

