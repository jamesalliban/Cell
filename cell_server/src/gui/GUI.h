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
    void addTagBoundariesGUI();
    void addMotionFields1GUI();
    void addMotionFields2GUI();
    void addColourFieldsGUI();
    void addKinectGlobalGUI();
    void addSkeletonCalibrationAllGUI();
    void addKinectCalibration0and1GlobalGUI();
    void addKinectCalibration2and3GlobalGUI();
    void addDuplicatesGUI();
    void addNonKinectUserDegugGUI();
    void addVariousGUI();
    
    
    void variousGUIEvent(ofxUIEventArgs &e);
    void addTagShaderGUIEvent(ofxUIEventArgs &e);
    void addTagAnimationGUIEvent(ofxUIEventArgs &e);
    
    //void cameraGUIEvent(ofxUIEventArgs &e);
    
    virtual void update();
    virtual void draw();
    
    
    testApp* app;
};
