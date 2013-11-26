#pragma once

#include "ofMain.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "GUI.h"
#include "KinectManager.h"


class testApp : public ofBaseApp
{
    
public:
	void setup();
	void update();
	void draw();
    
	void keyPressed(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void windowResized(int w, int h);
	void gotMessage(ofMessage msg);
    
    
    void loadTagIndexData();
    void urlResponse(ofHttpResponse & response);
    
    
    void exit();
    
	ResourceManager resourceManager;
	SceneManager sceneManager;
	KinectManager kinectManager;
    
	GUI gui;
    
	float topBlockHeight;
	float topBlockBot;
	float leftBlockW;
	float rightBlockW;
	float framesSinceMouseMove;
    
	bool isFirstFrame;
    
	static bool isKinectAttached;
    static bool isAllUserDebugVisible;
    
	bool isPaused;
    
    
//    int xmlCheckFrameFrequency;
//    int currentEllapsedFrames;
//    int currentXmlIndex;
//    bool isLoadingXml;
//    int tagTotal;
#ifdef CHINESE_CELL
    // Tag XML loading stuff
    ofXml tagIndexXml;
    string tagIndexData;
    int latestTagTotal;
    bool isLoading;
    bool isNewIndexXml;
    
    int timesLoaded;
#endif
    
};