/*
 *  TagTexture.h
 *  cell
 *
 *  Created by James Alliban on 14/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFontStash.h"
#include "CellGlobals.h"

class TagTexture
{
public:
	TagTexture();

	void	init(ofImage* tImg);

	void	begin();
	void	end();
	void	bind();
	void	unbind();
	void	testDraw();


	ofFbo fbo;
	ofBlendMode blendMode;
	ofImage* tagImg;

	string typeStr;

	bool isGrabFbo;
};


