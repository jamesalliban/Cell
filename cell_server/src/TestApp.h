#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "MyGui.h"
#include "KinectManager.h"


class TestApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
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
	KinectManager* kinectManager;
	//KinectManager kinectManager;

	MyGui* myGui;

	int topBlockHeight;
	int topBlockBot;
	int leftBlockW;
	int rightBlockW;
	float framesSinceMouseMove;

	bool isFirstFrame;

	bool isKinectAttached;
};

#endif
