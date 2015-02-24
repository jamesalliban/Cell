/*
 *  TagTexture.cpp
 *  cell
 *
 *  Created by James Alliban on 14/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#include "TagTexture.h"
#include "ofApp.h"

TagTexture::TagTexture()
{

}



void TagTexture::init(ofImage* tImg)
{
    ofApp* app = (ofApp*)ofGetAppPtr();

#ifdef CHINESE_CELL
    ofxFontStash* unicodefont = &app->resourceManager.unicodeFont;
#else
    ofTrueTypeFont* font = &app->resourceManager.tagFont;
#endif
    
    tagImg = tImg;

    blendMode = OF_BLENDMODE_ALPHA;

    ofFbo::Settings settings;
    settings.width = tagImg->width;
    settings.height = tagImg->height;
    settings.internalformat = GL_RGBA;
    //settings.textureTarget = GL_TEXTURE_2D;
    fbo.allocate(settings);
    //fbo.allocate(160, 40, GL_RGBA);

    typeStr = (ofRandomuf() < 0.5) ? "ABCD" : "1234";


	glDisable(GL_DEPTH_TEST);

	fbo.begin();
		ofClear(0, 0, 0, 1); // we clear the fbo.

		tagImg->draw(0, 0);

//		ofSetColor(255, 255, 255, 255);
//		ofRect(0, 0, 160, 40);
//
//		ofSetColor(0, 0, 0, 200);
//		//ofEnableAlphaBlending();
//        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		font->drawString(typeStr, 2, 37);
//        //ofDisableBlendMode();

	fbo.end();
	glEnable(GL_DEPTH_TEST);

}



void TagTexture::begin()
{
	//fbo.begin();
}



void TagTexture::end()
{

}



void TagTexture::bind()
{
	fbo.getTextureReference(0).bind();
}



void TagTexture::unbind()
{
	fbo.getTextureReference(0).unbind();
}



void TagTexture::testDraw()
{
//    TestApp* app = (TestApp*)ofGetAppPtr();
//    ofTrueTypeFont* font = &app->resourceManager.tagFont;
//
//	ofSetColor(255, 0, 0, 255);
//	font->drawString(typeStr, 650, 650);
//	ofSetColor(255, 255, 255, 255);
//	fbo.draw(500, 650);
}

