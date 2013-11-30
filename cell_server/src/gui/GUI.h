//
//  GUI.h
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#pragma once

#include "ofMain.h"
#include "GUIManager.h"

class testApp;

class GUI : GUIManager
{
public:
    virtual void setup();
    virtual bool getVisible();
    
    void addKeyboardShortcutsGUI();
    void addCameraGUI();
    void addTagShaderGUI();
    void addLineAppearanceGUI();
    void addTagAnimationGUI();
    void addTagAdditionGUI();
    void addTagBoundariesGUI();
    void addMotionFields1GUI();
    void addMotionFields2GUI();
    void addColourFieldsGUI();
    void addKinectGlobalGUI();
    void addSkeletonCalibrationAllGUI();
    void addKinectCalibration0GUI();
    void addKinectCalibration1GUI();
    void addKinectCalibration2GUI();
    void addKinectCalibration3GUI();
    void addDuplicatesGUI();
    void addKinectUserDegugGUI();
    void addVariousGUI();
    
    
    void variousGUIEvent(ofxUIEventArgs &e);
    void addTagShaderGUIEvent(ofxUIEventArgs &e);
    void addTagAnimationGUIEvent(ofxUIEventArgs &e);
	void addKinectUserDegugGUIEvent(ofxUIEventArgs &e);

    
    //void cameraGUIEvent(ofxUIEventArgs &e);
    
    virtual void update();
    virtual void draw();
    
    float skelOffsetRange;

    testApp* app;
};
