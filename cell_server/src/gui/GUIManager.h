//
//  GUIManager.h
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxUI.h"

// gui titles
#define TOP_TITLE "TOP"

#define TOP_AUTO_SAVE "AUTO SAVE"
#define TOP_SAVE "SAVE"
#define TOP_NEXT "NEXT"
#define TOP_PREVIOUS "PREV"


class GUIManager
{
public:
    //GUIManager();
    virtual void setup();
    virtual bool getVisible();
    
    ofColor color;
    
protected:
    
    virtual void buildTopBarGUI();
    virtual ofxUICanvas* getNewGUI(string title);
    virtual void keyPressed(ofKeyEventArgs& eventArgs);
    virtual void exit(ofEventArgs& eventArgs);
    virtual void finaliseCanvas(ofxUICanvas* gui, bool isAddedToArray);
    virtual void topBarGUIEvent(ofxUIEventArgs &e);
    void guiEvent(ofxUIEventArgs &e);
    
    
    
    void nextPage();
    void previousPage();
    void showGUI();
    void toggleVisible();
    
    void saveSettings();
    
    void setGUIColour();
    
    ofxUICanvas* topBarGui;
    
    vector<ofxUICanvas *> guiPages;
    int currentUIID;
    
    float dim;
    float toggleDim;
    float xInit;
    float length;
    float canvasW;
    float canvasH;
    float toggleSize;
    float spacerW, spacerH;
    bool isVisible;
    bool isAutoSave;
};
