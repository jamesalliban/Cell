#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define SERVER_IP "192.168.0.199" // "localhost"//"192.168.0.199"
#define PORT_OUT 12346
#define PORT_IN 12345

class testApp : public ofBaseApp
{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		void drawDebug();
		bool kinectFailedToInit;

		string client_name;

		ofxOscSender sender;
		ofxOscReceiver receiver;
};
