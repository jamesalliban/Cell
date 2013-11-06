/*
 *  Grid.h
 *  cell
 *
 *  Created by James Alliban on 10/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

// Grid is a custom 3D
// object that we've
// created in this
// example.
//
// We inherit from ofNode

#pragma once

#include "ofMain.h"

class Grid : public ofNode
{
public:
	void	customDraw();
	bool    isActive;
	int     alpha;
};