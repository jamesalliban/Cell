#include "testApp.h"
#include "ofxVectorMath.h"
#include "ofxSimpleGuiToo.h"



bool	myBool1;
bool	myBool2;
bool	myBool3;
bool	myBool4;
bool	myBool5;
bool	myBool6;
bool	myBool7;
bool	myBool8;
bool	myBool9;

int		myInt1;
int		myInt2;
int		myInt3;
int		myInt4;
int		myInt5;
int		myInt6;
int		myInt7;
int		myInt8;
int		myInt9;

float	myFloat1;
float	myFloat2;
float	myFloat3;
float	myFloat4;
float	myFloat5;
float	myFloat6;
float	myFloat7;
float	myFloat8;
float	myFloat9;

float	myColors[4];


// for demonstrating adding any drawable object (that extends ofBaseDraw);
ofVideoGrabber		vidGrabber;
unsigned char * 	videoInverted;
ofTexture			videoTexture;

bool	randomizeButton = true;

ofPoint *points;
ofPoint	v[300];

//--------------------------------------------------------------
void testApp::setup(){	 
	ofBackground(0, 0, 0);
	
	// for demonstrating adding any drawable object (that extends ofBaseDraw);
	vidGrabber.initGrabber(320, 240);	
	videoInverted 	= new unsigned char[int(vidGrabber.getWidth() * vidGrabber.getHeight() * 3)];
	videoTexture.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), GL_RGB);
	
	
	// 'gui' is a global variable declared in ofxSimpleGuiToo.h
	gui.addTitle("A group");
	gui.addSlider("myFloat1", myFloat1, 0.0, 1); 
	gui.addSlider("myInt1", myInt1, 5, 10); 
	gui.addToggle("myBool1", myBool1);
	gui.addButton("Randomize Background", randomizeButton);
	gui.addColorPicker("BG Color", myColors);
	
	
	// start a new group
	gui.addTitle("Another group");
	gui.addSlider("myFloat2", myFloat2, 0.0, 1);
	gui.addSlider("myInt2", myInt2, 3, 8);
	gui.addToggle("myBool2", myBool2);	
	gui.addFPSCounter();

	// new group, this time separate into it's own column
	gui.addTitle("Yes one more group").setNewColumn(true);
	gui.addToggle("myBool4", myBool4);	
	gui.addToggle("myBool3", myBool3);	
	gui.addSlider("myFloat3", myFloat3, 0.0, 1);
	gui.addSlider("myFloat4", myFloat4, 0.0, 20);
	gui.addSlider("myInt6", myInt6, 0, 10);
	gui.addSlider("myInt4", myInt4, 10, 20);
	gui.addContent("Camera feed", vidGrabber);
	gui.addContent("Inverted", videoTexture);
	
	
	gui.addPage("A new page");		// use '[' ']' to cycle through pages, or keys 1-9
	gui.addSlider("myInt5", myInt5, 2, 5);
	gui.addSlider("myInt7", myInt7, 0, 100);
	gui.addSlider("myInt8", myInt8, 10, 50);
	gui.addSlider("myInt3", myInt3, 0, 100);
	gui.addSlider("myFloat7", myFloat7, 0.0, 1);
	gui.addSlider("myFloat8", myFloat8, 0.0, 0.1);
	gui.addSlider("myInt9", myInt9, 0, 10); 

	gui.addTitle("Final group?");
	gui.addToggle("myBool5", myBool5);	
	gui.addToggle("myBool6", myBool6);	
	gui.addToggle("myBool7", myBool7);	
	gui.addToggle("myBool8", myBool8);	
	
	
	// by default each page is saved in an xml with the same name as display name
	// you can override this with ofxSimpleGuiPage::setXMLName(string s);
	// ofxSimpleGuiToo::addPage() returns reference to the page, so you can do it directly on one line
	// of save it in a variable for use later
	gui.addPage("My 3rd page").setXMLName("foo.xml");	
	gui.addSlider("myFloat5", myFloat5, 0.0, 5);
	gui.addSlider("myFloat6", myFloat6, 0.0, 1);
	gui.addSlider("myFloat9", myFloat9, 0.0, 0.01 ); 
	gui.addToggle("myBool9", myBool9);	
	
	
	gui.loadFromXML();
	
	gui.show();
}

//--------------------------------------------------------------
void testApp::update(){
	if(randomizeButton) {
		randomizeButton = false;
		ofBackground(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
	}
	
	
	// from ofVideoGrabber example (
	vidGrabber.update();
	if(vidGrabber.isFrameNew()){
		int totalPixels = vidGrabber.getWidth() * vidGrabber.getHeight() * 3;
		unsigned char * pixels = vidGrabber.getPixels();
		for(int i = 0; i < totalPixels; i++) videoInverted[i] = 255 - pixels[i];
		videoTexture.loadData(videoInverted, vidGrabber.getWidth(), vidGrabber.getHeight(), GL_RGB);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(myColors[0] * 255.0f, myColors[1] * 255.0f, myColors[2] * 255.0f);
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key){ 
	if(key>='0' && key<='9') {
		gui.setPage(key - '0');
		gui.show();
	} else {
		switch(key) {
			case ' ': gui.toggleDraw(); break;
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
			case 'p': gui.nextPageWithBlank(); break;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}
