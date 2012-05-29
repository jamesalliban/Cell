#ifndef KINECTMANAGER_H
#define KINECTMANAGER_H


#include "ofMain.h"

#include <stdlib.h>
#include <Windows.h>
#include "ofxMSKinect.h"
#include "ofxMSSkeletonDraw.h"
//#include "KineticNetworkClient.h"
#include "ofFileUtils.h"
#include "ofxOsc.h"

#define SKELETON_MAX 8

#define CLIENT0_IP "192.168.0.100"
#define CLIENT1_IP "192.168.0.101"
#define CLIENT2_IP "192.168.0.102"
#define CLIENT3_IP "192.168.0.103"
#define PORT_OUT 12345
#define PORT_IN 12346


class KinectManager
{
public:
    KinectManager();
    virtual ~KinectManager();

    void init();
    void update();
    void draw();
    void keyPressed(int key);
    bool hasSkeleton();
    void ServerSendCommandstoClients(int key);

    ofxMSKinect* kinect;
    ofxMSKinectDevice* firstDevice;
    //KineticNetWorkServer* nwserver;
    ofxSkeletonRenderer* skeletonDrawer; // simple class to draw skeletons
    vector<ofxMSKinectSkeleton> trackedSkeletons;
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

#endif // KINECTMANAGER_H
