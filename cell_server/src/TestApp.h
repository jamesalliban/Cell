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
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
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
};