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
    void draw();
    void keyPressed(int key);
    bool hasSkeleton();
    void ServerSendCommandstoClients(int key);

    vector<KinectSkeletonData> trackedSkeletons;
    int padding;
    long val;


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

