
#pragma once

#include "ofxKinectNui.h"
#include "ofMain.h"
#include "ofxKinectNuiPlayer.h"
//#include "ofxKinectNuiRecorder.h"

#include "ofxOsc.h"
#include "ofFileUtils.h"

#define SERVER_IP "10.0.42.82" // "192.168.0.7" // "localhost"//"192.168.0.199"
#define PORT_OUT 12346
#define PORT_IN 12345

struct SkeletonData
{
	int id;
	int arrayIndex;
	int resetCount;
	vector<ofVec3f> skelPoints;
	vector<ofVec3f> pointVelocities;
    bool isActive;

    //int jointIndex;
    //int joint2Index;
    //int lowerBodyAdd;
    //float jointOffset;
    //ofVec3f userPoint;
    //ofVec3f userPointOffset;
    //ofVec3f attraction;
    //ofVec3f averageAttractionVec;
    //vector<ofVec3f> attractionVecs;
    //int attractionVecMax;
    //float lengthSquared;
    //float dist;
    //int millisBecameActive;
    //float secondsSinceActive;
};

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		
		void buildSkeletonDataObjects();
		void resetSkeletonData(int index);
		void populateSkeletonData(int skeletonPointsIndex, int skeletonDataObjectIndex, bool isSkelNew);
		/**
		 * @brief	example for adjusting video images to depth images
		 * @note	inspired by akira's video http://vimeo.com/17146552
		 */
		void exit();
		

		void keyPressed  (int key);
		void kinectPlugged();
		void kinectUnplugged();
		
		ofxKinectNui kinect;

		ofxBase3DVideo* kinectSource;

		ofTexture calibratedTexture;

		int clientID;

		bool bDrawVideo;
		bool bDrawDepthLabel;
		bool bDrawSkeleton;
		bool bDrawCalibratedTexture;
		bool bPlugged;
		bool bUnplugged;
		
		unsigned short nearClipping;
		unsigned short farClipping;
		int angle;
		
		vector<SkeletonData> skeletonDataObjects;

		ofxOscSender sender;
		ofxOscReceiver receiver;
};
