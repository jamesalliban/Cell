/*
 *  CloudTagManager.cpp
 *  cell
 *
 *  Created by James Alliban on 09/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#include "CloudTagManager.h"
#include "TestApp.h"


CloudTagManager::CloudTagManager()
{
}


CloudTagManager::~CloudTagManager()
{

}


void CloudTagManager::init(ofShader* shader)
{
    TestApp* app = (TestApp*)ofGetAppPtr();
	cloudTagAmount = 1000; // 1000;

	shadeContrastMin = 0.4;
	shadeContrastMax = 0.7;
	shadeBrightnessMin = 0.0;
	shadeBrightnessMax = 0.15;
	shadeAlphaMin = 0.0;
	shadeAlphaMax = 0.15;
	shadeBlendMix = 1.0;
	shadeBlendMode = 0;// there are 10 diff. blend modes,

	for (int i = 0; i  < cloudTagAmount; i++)
	{
	    int tagDataAmount = app->resourceManager.tagData.size();
	    TagData* tagData = app->resourceManager.tagData[i % tagDataAmount];
		CloudTag *cloudTag = new CloudTag();
		cloudTag->init(shader, tagData, i);
		cloudTags.push_back(cloudTag);
	}


	tempUser = ofVec3f(0, 0, 0);
}



void CloudTagManager::update()
{
    TestApp* app = (TestApp*)ofGetAppPtr();

    if (app->sceneManager.isUpdateVars)
    {
        for (int i = 0; i < cloudTagAmount; i++)
        {
            cloudTags[i]->rotate((float)ofGetFrameNum() * 0.01, 0, ofRandom(0, 100), 0);
            cloudTags[i]->updateVars();
        }
    }

    for (int i = 0; i < cloudTagAmount; i++)
	{
	    cloudTags[i]->update();
	}
}



void CloudTagManager::customDraw()
{
    TestApp* app = (TestApp*)ofGetAppPtr();

	for (int i = 0; i < cloudTagAmount; i++)
	{
		CloudTag* cloudTag = cloudTags[i];
		cloudTag->customDraw();
	}
}
