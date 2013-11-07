/*
 *  CloudTagManager.cpp
 *  cell
 *
 *  Created by James Alliban on 09/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#include "CloudTagManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

void CloudTagManager::init(ofShader* shader, ResourceManager *resourceManager)
{
	cloudTagAmount = 1000; // 1000;

	shadeContrastMin = 0.4;
	shadeContrastMax = 0.7;
	shadeBrightnessMin = 0.0;
	shadeBrightnessMax = 0.15;
	shadeAlphaMin = 0.0;
	shadeAlphaMax = 0.15;
	shadeBlendMix = 1.0;
	shadeBlendMode = 0;// there are 10 diff. blend modes,

	
	int tagDataAmount = resourceManager->tagData.size();
	for (int i = 0; i  < cloudTagAmount; i++)
	{
	    TagData* tagData = &resourceManager->tagData[i % tagDataAmount];
		CloudTag cloudTag;
		cloudTag.init(shader, tagData, i);
		cloudTags.push_back(cloudTag);
	}

	tempUser = ofVec3f(0, 0, 0);
}



void CloudTagManager::update()
{
    if (SceneManager::isUpdateVars)
    {
        for (int i = 0; i < cloudTagAmount; i++)
        {
            cloudTags[i].rotate((float)ofGetFrameNum() * 0.01, 0, ofRandom(0, 100), 0);
            cloudTags[i].updateVars();
        }
    }

    for (int i = 0; i < cloudTagAmount; i++)
	{
	    cloudTags[i].update();
	}
}



void CloudTagManager::customDraw()
{
	for (int i = 0; i < cloudTagAmount; i++)
	{
		CloudTag* cloudTag = &cloudTags[i];
		cloudTag->customDraw();
	}
}


