﻿#include "TagData.h"
#include "ofApp.h"

float TagData::textAlphaAdd;

#ifdef CHINESE_CELL
void TagData::setup(ofXml XML, ofxFontStash *_tagFont, vector<DemographicData> *demographicData, ofShader *_blackToAlphaShader)
#else
void TagData::setup(ofxXmlSettings *XML, ofTrueTypeFont *_tagFont, vector<DemographicData> *demographicData, ofShader *_blackToAlphaShader)
#endif
{
	doesTextContainAscender = doesTextContainDescender = false;

    parseXML(XML, demographicData);
    
    alphaModifier = 1.0;
    alphaReductionSpeed = 0.1;
    alphaAdditionSpeed = 0.15;
    isAnimatingAlpha = false;
    isFadingOut = true;
    
#ifdef CHINESE_CELL
    tagFont = _tagFont;
#else    
    tagFont = _tagFont;
#endif
    
    blackToAlphaShader = _blackToAlphaShader;
    
    isTagUpdated = false;
    
    createTagFbo();
}


#ifdef CHINESE_CELL
void TagData::parseXML(ofXml XML, vector<DemographicData> *demographicData)
#else
void TagData::parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData)
#endif
{
    
#ifdef CHINESE_CELL
    word = XML.getValue("word");
    //printf("word = %s\n", word.c_str());
#else
    word = XML->getValue("word", "");
	for (int i = 0; i < word.size(); i++)
		checkCharHeight(word[i]);
#endif
    
    int currentElement = 0;
    
#ifdef CHINESE_CELL
    int numDemoTags = XML.getNumChildren("demographic");
#else
    int numDemoTags = XML->getNumTags("demographic");
#endif
    
    //printf("numDemoTags1:%i \n", numDemoTags);
    // loop through all the demographics in the tag XML
    for (int i = 0; i < numDemoTags; i++)
    {
        Demographic demographicStruct;

        // loop through all the available demographics to see if there is a match
        for (int j = 0; j < (int)demographicData->size(); j++)
        {
            DemographicData* dData = &demographicData->at(j);
            
#ifdef CHINESE_CELL
            string demographicElement = "demographic[" + ofToString(i) + "]";
			if (XML.getValue(demographicElement + "[@id]").c_str() == dData->name)
            {
                demographicStruct.demographicData = dData;
                demographicStruct.strength = ofToFloat(XML.getValue(demographicElement + "/strength"));
				//printf("  - demographic:%s strength = %f \n", dData->name.c_str(), demographicStruct.strength);
                demographics.push_back(demographicStruct);
                ++currentElement;
            }
#else
			if (XML->getAttribute("demographic", "id", "", i).c_str() == dData->name)
            {
                demographicStruct.demographicData = dData;
                XML->pushTag("demographic", currentElement);
                demographicStruct.strength = XML->getValue("strength", 0.0);
				//printf("  - word = %s, demographic:%s strength = %f \n", dData->name.c_str(), dData->name.c_str(), demographicStruct.strength);
                demographics.push_back(demographicStruct);
                XML->popTag();
                ++currentElement;
            }
#endif
        }
    }
}


void TagData::update()
{
    isTagUpdated = false;
    alphaModifier += (isFadingOut) ? -alphaReductionSpeed : alphaReductionSpeed;
    if (isFadingOut)
    {
        if (alphaModifier <= 0)
        {
            // change the tag to display the new tag
            word = newWord;
            fbo.getTextureReference().clear();
            alphaFbo.getTextureReference().clear();
            createTagFbo();
            
            // now make one of the CloudTag objects appear at the front
            ofApp* app = (ofApp*)ofGetAppPtr();
            app->sceneManager.cloudTagMan.displayNewTag(word);
            
            isTagUpdated = true;
            
//            printf("***** should be changing tag colour ****** \n");
//            alphaFbo.begin();
//            ofPushStyle();
//            ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
//            ofRect(0, 0, fbo.getWidth() * 0.9, fbo.getHeight() * 0.9);
//            ofPopStyle();
//            alphaFbo.end();
            
            // move the tag that appears first in the vector to the front of the play area (below the camera - it will float up)
            
            
            
            // animate alphaModifier back to 1.0
            alphaModifier = alphaReductionSpeed;
            isFadingOut = false;
        }
    }
    else
    {
    }
}



void TagData::createTagFbo()
{
    
    float ascDescAdd = 0.25;
	float bordertoHeightRatio = 0.35;
    
    // get the bounding box of the tag
    
	ofRectangle textBoundingBox;
#ifdef CHINESE_CELL
    int fontSize = 110;
	textBoundingBox = tagFont->getBBox(word, fontSize, 0, 0);
#else
	textBoundingBox = tagFont->getStringBoundingBox(word, 0, 0);
#endif
    
    // adjust bounding box for border
    
	height = textBoundingBox.getHeight();
	float heightAscDescAdd = height * ascDescAdd;
	float border = height * bordertoHeightRatio;
	height += border * 2;
	width = textBoundingBox.getWidth() + border * 2;
    
    blendMode = OF_BLENDMODE_ALPHA;
    
    // configure fbo settings
    
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.internalformat = GL_RGBA;
    fbo.allocate(settings);
	alphaFbo.allocate(settings);
    
	glDisable(GL_DEPTH_TEST);
    
    // draw tag to Fbo
    
	fbo.begin(); //////////////////////////////////////////////
    ofClear(0, 0, 0, 1);
    ofSetColor(255);
    ofRect(0, 0, width, height);
    ofSetColor(0);
    
#ifdef CHINESE_CELL
    float x = border;
    float y = textBoundingBox.getHeight() + border - (textBoundingBox.getHeight() * 0.1);
    ofPushMatrix();
    ofTranslate(x, y);
    
    tagFont->drawMultiLine(word, fontSize, 0, 0);
    ofPopMatrix();
#else
    float x = border;
    float y = height - ((doesTextContainDescender) ? heightAscDescAdd : 0) - border;
    ofPushMatrix();
    ofTranslate(x, y);
    tagFont->drawString(word, 0, 0);
    ofPopMatrix();
#endif
	fbo.end(); /////////////////////////////////////////////////
    
    // draw fbo to alphaFbo and apply shader to make the black text transparant
    
	alphaFbo.begin(); ///////////////////////////////////////////////////
	ofClear(0, 0, 0, 1);
	blackToAlphaShader->begin();
    blackToAlphaShader->setUniform1f("textAlphaAdd", textAlphaAdd);
	fbo.draw(0, 0);
	blackToAlphaShader->end();
	alphaFbo.end(); /////////////////////////////////////////////////////
	
	fbo.getTextureReference().clear();
}



void TagData::checkCharHeight(char letter)
{
	if (letter == 'b' ||
		letter == 'd' ||
		letter == 'f' ||
		letter == 'h' ||
		letter == 'i' ||
		letter == 'j' ||
		letter == 'k' ||
		letter == 'l' ||
		letter == 't' ||
		((int)letter > 32 && (int)letter < 91))
	{
		doesTextContainAscender = true;
	}
	
	if (letter == 'g' ||
		letter == 'j' ||
		letter == 'p' ||
		letter == 'q' ||
		letter == 'y')
	{
		doesTextContainDescender = true;
	}
}



//void TagData::testDraw()
//{
//    TestApp* app = (TestApp*)ofGetAppPtr();
//    ofTrueTypeFont* font = &app->resourceManager.tagFont;
//
//	ofSetColor(255, 0, 0, 255);
//	font->drawString(typeStr, 650, 650);
//	ofSetColor(255, 255, 255, 255);
//	fbo.draw(500, 650);
//}
//


// TODO: add question here.
void TagData::startChangeTagData(string latestTag)
{
    isAnimatingAlpha = true;
    isFadingOut = true;
    newWord = latestTag;
}


