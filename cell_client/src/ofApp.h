/******************************************************************/
/**
 * @file	ofApp.h
 * @brief	Example for ofxKinectNui addon
 * @note
 * @todo
 * @bug	
 *
 * @author	sadmb
 * @date	Oct. 28, 2011
 */
/******************************************************************/
#pragma once

#define OSC_ENABLED

#include "ofMain.h"
#include "ofxKinectNui.h"
#include "ofxKinectNuiPlayer.h"
#include "ofxKinectNuiRecorder.h"
#ifdef OSC_ENABLED
#include "ofxOsc.h"
#endif

class ofxKinectNuiDrawTexture;
class ofxKinectNuiDrawSkeleton;


// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS


#include "ofFileUtils.h"

#define SERVER_IP "10.0.42.82" // "192.168.0.7" // "localhost"//"192.168.0.199"
#define PORT_OUT 12346
#define PORT_IN 12345

struct SkeletonData
{
	int index;
	int id;
	int resetCount;
	vector<ofPoint> skelPoints;
	vector<ofPoint> pointVelocities;
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

class ofApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		
		SkeletonData getEmptySkelDataObject();


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
		
		// Please declare these texture pointer and initialize when you want to draw them
		ofxKinectNuiDrawTexture*	videoDraw_;
		ofxKinectNuiDrawTexture*	depthDraw_;
		ofxKinectNuiDrawTexture*	labelDraw_;
		ofxKinectNuiDrawSkeleton*	skeletonDraw_;


		ofTexture calibratedTexture;

		vector<string> joints;
		
		ofFbo displayTextFbo;
		ofFbo newSkelDataFbo;

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
		
		vector<SkeletonData> prevSkeletonDataObjects;
		vector<SkeletonData> newSkelData;

		int prevSkelCount;

#ifdef OSC_ENABLED
		ofxOscSender sender;
		ofxOscReceiver receiver;
#endif
		bool isTesting;
		int testCount;
		int framesSinceLastKinectSkelFound;
};

