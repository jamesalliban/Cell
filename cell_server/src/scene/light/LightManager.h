/*
 *  LightManager.h
 *  cell
 *
 *  Created by James Alliban on 16/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */


#ifndef _LIGHT_MANAGER
#define _LIGHT_MANAGER

#include "ofMain.h"

class LightManager
{

public:
	LightManager();
	~LightManager();

	void update();
	void enable();
	void disable();

	ofLight light;

    float ambR, ambG, ambB, ambA;
    float diffR, diffG, diffB;
    float specR, specG, specB;

	int xPos, yPos, zPos;


};

#endif
