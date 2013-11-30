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
	cloudTagAmount = 1800;//  800;

	shadeContrastMin = 0.4;
	shadeContrastMax = 0.7;
	shadeBrightnessMin = 0.0;
	shadeBrightnessMax = 0.15;
	shadeAlphaMin = 0.0;
	shadeAlphaMax = 0.15;
	shadeBlendMix = 1.0;
	shadeBlendMode = 0.0;// there are 10 diff. blend modes,

	
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
    
    
	tagsOrderedByZ.clear();
    
	for (int i = 0; i < (int)cloudTags.size(); i++)
	{
		CloudTag* tag = &cloudTags[i];
        // populate a vector with active indices. This will be reordered based on their z value
        tagsOrderedByZ.push_back(tag);
	}
	std::sort(tagsOrderedByZ.begin(), tagsOrderedByZ.end(), compareTagByZ());

}



void CloudTagManager::customDraw()
{
	for (int i = 0; i < tagsOrderedByZ.size(); i++)
		tagsOrderedByZ[i]->drawTags();
    
	for (int i = 0; i < tagsOrderedByZ.size(); i++)
		if ((float)i / (float)tagsOrderedByZ.size() < lineAmountDrawn)
            tagsOrderedByZ[i]->drawLines();
}



void CloudTagManager::displayNewTag(string word)
{
    for (int i = 0; i < cloudTags.size(); i++)
    {
        if (cloudTags[i].tagData->word == word)
        {
            cloudTags[i].position = ofVec3f(ofRandom(newTagStartXMin, newTagStartXMax), newTagStartY, newTagStartZ);
            return;
        }
    }
}


