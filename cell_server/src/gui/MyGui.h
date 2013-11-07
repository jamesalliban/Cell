/*
 *  MyGui.h
 *  cell
 *
 *  Created by James Alliban on 10/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

class testApp;

class MyGui
{

public:

	MyGui();
	void	draw();
	void	keyPressed(int key);

	testApp*	app;

protected:

};


#include "testApp.h"
