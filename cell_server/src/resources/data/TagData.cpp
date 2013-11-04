#include "TagData.h"
#include "TestApp.h"

#ifdef CHINESE_CELL
void TagData::setup(ofxXmlSettings *XML, ofxFontStash *tagFont, vector<DemographicData> *demographicData, ofShader *blackToAlphaShader)
#else
void TagData::setup(ofxXmlSettings *XML, ofTrueTypeFont *tagFont, vector<DemographicData> *demographicData, ofShader *blackToAlphaShader)
#endif
{
	doesTextContainAscender = doesTextContainDescender = false;

    parseXML(XML, demographicData);
    
	TestApp* app = (TestApp*)ofGetAppPtr();

	ofRectangle textBoundingBox;
	
	float ascDescAdd = 0.25;
	float bordertoHeightRatio = 0.35;

	//tagFont->setLineHeight(1.0);
#ifdef CHINESE_CELL
	textBoundingBox = tagFont->getBBox(word, 0, 0, 72);
#else
	textBoundingBox = tagFont->getStringBoundingBox(word, 0, 0);
#endif
	height = textBoundingBox.height;
	float heightAscDescAdd = height * ascDescAdd;
	float border = height * bordertoHeightRatio;
	height += border * 2;
	width = textBoundingBox.width + border * 2;

    blendMode = OF_BLENDMODE_ALPHA;

    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.internalformat = GL_RGBA;
    fbo.allocate(settings);
	alphaFbo.allocate(settings);

	glDisable(GL_DEPTH_TEST);

	fbo.begin();
		ofClear(0, 0, 0, 1); // we clear the fbo.

		ofSetColor(255);
		ofRect(0, 0, width, height);
		
		ofSetColor(0);

#ifdef CHINESE_CELL
		//tagFont->drawString(word, 20, 80);  //"汉字漢字 頠餈 溿煔煃 鱙鷭黂 \n"
		tagFont->drawMultiLine(word, 32, 10, 50);
#else
		float x = border;
		float y = height - ((doesTextContainDescender) ? heightAscDescAdd : 0) - border;
		ofPushMatrix();
		ofTranslate(x, y);
		tagFont->drawString(word, 0, 0);
		ofPopMatrix();
#endif

//		ofSetColor(255, 255, 255, 255);
//		ofRect(0, 0, 160, 40);
//
//		ofSetColor(0, 0, 0, 200);
//		//ofEnableAlphaBlending();
//        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		font->drawString(typeStr, 2, 37);
//        //ofDisableBlendMode();

	fbo.end();

	alphaFbo.begin();
	ofClear(0, 0, 0, 1);
	blackToAlphaShader->begin();
	fbo.draw(0, 0);
	blackToAlphaShader->end();
	alphaFbo.end();
	
	fbo.getTextureReference().clear();
}


void TagData::parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData)
{
    // set word
    word = XML->getValue("word", "");
#ifndef CHINESE_CELL
	for (int i = 0; i < word.size(); i++)
	{
		checkCharHeight(word[i]);
	}
#endif
    int currentElement = 0;
    // loop through all the demographics in the tag XML
    int numDemoTags = XML->getNumTags("demographic");
    for (int i = 0; i < numDemoTags; i++)
    {
        Demographic demographicStruct;

        // loop through all the available demographics to see if there is a match
        for (int j = 0; j < (int)demographicData->size(); j++)
        {
            DemographicData* dData = &demographicData->at(j);

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
        }
    }
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
