//
//  GUIManager.cpp
//  emptyExample
//
//  Created by James Alliban on 25/06/2013.
//
//

#include "GUIManager.h"



void GUIManager::setup()
{
    ofAddListener(ofEvents().keyPressed, this, &GUIManager::keyPressed);
    ofAddListener(ofEvents().exit, this, &GUIManager::exit);
    
    currentUIID = 0;
    dim = 16;
    toggleDim = 16;
	canvasW = 320;
    canvasH = ofGetScreenHeight();
    xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    length = canvasW - xInit* 2;
    toggleSize = 32;
    spacerW = canvasW - xInit* 2;
    spacerH = 1;
    color = ofColor(100, 100, 210, 230);
    isVisible = false;
	isAutoSave = true;
    
    buildTopBarGUI();
}


bool GUIManager::getVisible()
{
    return isVisible;
}


void GUIManager::buildTopBarGUI()
{
	topBarGui = new ofxUICanvas(canvasW, 0, 600, 146);
    topBarGui->setColorBack(color);
	topBarGui->setName(TOP_TITLE);
    
	topBarGui->addWidgetRight(new ofxUIFPS(OFX_UI_FONT_MEDIUM));
    topBarGui->addWidgetRight(new ofxUILabelToggle(true, TOP_AUTO_SAVE));
    topBarGui->addWidgetRight(new ofxUILabelButton(false, TOP_SAVE));
    topBarGui->addWidgetRight(new ofxUILabelButton(false, TOP_PREVIOUS));
    topBarGui->addWidgetRight(new ofxUILabelButton(false, TOP_NEXT));
    
    topBarGui->autoSizeToFitWidgets();
    
	ofAddListener(topBarGui->newGUIEvent, this, &GUIManager::topBarGUIEvent);
	finaliseCanvas(topBarGui, false);
}



ofxUICanvas* GUIManager::getNewGUI(string title)
{
    ofxUICanvas* gui = new ofxUICanvas(0, 0, canvasW, canvasH);
    gui->setColorBack(color);
	gui->setName(title);
	gui->addWidgetDown(new ofxUILabel(gui->getName(), OFX_UI_FONT_MEDIUM));
    gui->addSpacer(spacerW, spacerH);
    return gui;
}



void GUIManager::finaliseCanvas(ofxUICanvas* gui, bool isAddedToArray)
{
    string name = gui->getName();
    string xmlPath = "GUI/" + name + "GuiPageSettings.xml";
    gui->loadSettings(xmlPath);
    gui->setVisible(false);
    gui->autoSizeToFitWidgets();
    if (isAddedToArray) guiPages.push_back(gui);
}



void GUIManager::topBarGUIEvent(ofxUIEventArgs &e) //---------------- Top bar
{
	string name = e.widget->getName();
    
	if (name == TOP_AUTO_SAVE)
	{
		ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
		isAutoSave = toggle->getValue();
	}
	else if (name == TOP_SAVE)
	{
		saveSettings();
	}
	else if (name == TOP_NEXT)
	{
		ofxUIButton *button = (ofxUIButton *)e.widget;
		if (button->getValue()) nextPage();
	}
	else if (name == TOP_PREVIOUS)
	{
		ofxUIButton *button = (ofxUIButton *)e.widget;
		if (button->getValue()) previousPage();
	}
}


void GUIManager::keyPressed(ofKeyEventArgs& eventArgs) //int key)
{
    printf("keyPressed - key = %c \n", eventArgs.key);
    switch (eventArgs.key)
    {
		case '[':
            previousPage();
            break;
            
        case ']':
            nextPage();
            break;
            
        case ' ':
            toggleVisible();
    }
}


void GUIManager::nextPage()
{
    printf("next\n");
	++currentUIID;
    showGUI();
}


void GUIManager::previousPage()
{
    printf("prev\n");
	--currentUIID;
	if (currentUIID < 0) currentUIID = guiPages.size() - 1;
    showGUI();
}

void GUIManager::showGUI()
{
    for (int i = 0; i < (int)guiPages.size(); i++)
    {
        guiPages[i]->setVisible(false);
    }
	isVisible = true;
    if (guiPages.size() > 0)
    {
        int newVisibleGUIID = currentUIID % (int)guiPages.size();
        guiPages[newVisibleGUIID]->setVisible(true);
	}
    topBarGui->setVisible(true);
}


void GUIManager::toggleVisible()
{
    isVisible = !isVisible;
    if (isVisible)
    {
        showGUI();
    }
    else
    {
        for (int i = 0; i < (int)guiPages.size(); i++)
        {
            guiPages[i]->setVisible(false);
        }
		topBarGui->setVisible(false);
    }
}


void GUIManager::saveSettings()
{
	for (int i = 0; i < (int)guiPages.size(); i++)
    {
        string xmlPath = "GUI/" + guiPages[i]->getName() + "guiPagesettings.xml";
        guiPages[i]->saveSettings(xmlPath);
    }
    //topBarGui->saveSettings("GUI/" + topBarGui->getName() + "guiPagesettings.xml");
}


void GUIManager::setGUIColour()
{
    for (int i = 0; i < (int)guiPages.size(); i++)
    {
        guiPages[i]->setColorBack(color);
    }
    topBarGui->setColorBack(color);
}


void GUIManager::exit(ofEventArgs& eventArgs)
{
    if (isAutoSave) saveSettings();
	delete topBarGui;
    for (int i = 0; i < guiPages.size(); i++)
        delete guiPages[i];
}

