/*
 *  LightManager.h
 *  cell
 *
 *  Created by James Alliban on 16/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */


#pragma once

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