#include "TagData.h"
#include "TestApp.h"

void TagData::setup(ofxXmlSettings *XML, vector<DemographicData> *demographicData)
{
    parseXML(XML, demographicData);
    loadImage();






	TestApp* app = (TestApp*)ofGetAppPtr();
    ofTrueTypeFont* font = &app->resourceManager.tagFont;


    blendMode = OF_BLENDMODE_ALPHA;

    ofFbo::Settings settings;
    settings.width = tagImg.width;
    settings.height = tagImg.height;
    settings.internalformat = GL_RGBA;
    //settings.textureTarget = GL_TEXTURE_2D;
    fbo.allocate(settings);
    //fbo.allocate(160, 40, GL_RGBA);

    typeStr = (ofRandomuf() < 0.5) ? "ABCD" : "1234";


	glDisable(GL_DEPTH_TEST);

	fbo.begin();
		ofClear(0, 0, 0, 1); // we clear the fbo.

		tagImg.draw(0, 0);

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

	tagImg.clear();

}


void TagData::parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData)
{
    // set word
    word = XML->getValue("word", "");
    //printf("- tag = %s\n",  word.c_str());

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



void TagData::loadImage()
{
    tagImg.loadImage("images/tags/" + word + ".png");
}






void TagData::bind()
{
	fbo.getTextureReference(0).bind();
}



void TagData::unbind()
{
	fbo.getTextureReference(0).unbind();
}



void TagData::testDraw()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    ofTrueTypeFont* font = &app->resourceManager.tagFont;

	ofSetColor(255, 0, 0, 255);
	font->drawString(typeStr, 650, 650);
	ofSetColor(255, 255, 255, 255);
	fbo.draw(500, 650);
}

