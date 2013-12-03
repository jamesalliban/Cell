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

void CloudTagManager::init(ofShader* _shader, ResourceManager *_resourceManager)
{
    shader = _shader;
    resourceManager = _resourceManager;

//	shadeContrastMin = 0.4;
//	shadeContrastMax = 0.7;
//	shadeBrightnessMin = 0.0;
//	shadeBrightnessMax = 0.15;
//	shadeAlphaMin = 0.0;
//	shadeAlphaMax = 0.15;
//	shadeBlendMix = 1.0;
//	shadeBlendMode = 0.0;// there are 10 diff. blend modes,

	buildCloudTags();


	tempUser = ofVec3f(0, 0, 0);
}

void CloudTagManager::buildCloudTags()
{
    cloudTags.clear();
    int tagDataAmount = resourceManager->tagData.size();
	for (int i = 0; i  < (int)cloudTagAmount; i++)
	{
	    TagData* tagData = &resourceManager->tagData[i % tagDataAmount];
		CloudTag cloudTag;
		cloudTag.init(shader, tagData, i);
		cloudTags.push_back(cloudTag);
	}
    updateTags();
}


void CloudTagManager::update()
{
    for (int i = 0; i < cloudTags.size(); i++)
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


void CloudTagManager::updateTags()
{
    for (int i = 0; i < cloudTags.size(); i++)
    {
        cloudTags[i].rotate((float)ofGetFrameNum() * 0.01, 0, ofRandom(0, 100), 0);
        cloudTags[i].updateVars();
    }
}


void CloudTagManager::customDraw()
{
	for (int i = 0; i < tagsOrderedByZ.size(); i++)
		tagsOrderedByZ[i]->drawTags();
    
	for (int i = 0; i < tagsOrderedByZ.size(); i++)
		if ((float)i / (float)tagsOrderedByZ.size() < lineAmountDrawn)
            tagsOrderedByZ[i]->drawLines();

	ofPushStyle();
	ofSetColor(0, 255, 0);
	ofPushMatrix();
	ofTranslate(0, newTagStartY, newTagStartZ);
	ofDrawBox(abs(newTagStartXMin) + newTagStartXMax, 1, 1);
	ofPopMatrix();
	ofPopStyle();

	//gui->addRangeSlider("New tag starting X", -100, 100, &app->sceneManager.cloudTagMan.newTagStartXMin, &app->sceneManager.cloudTagMan.newTagStartXMax, length, dim);
 //   gui->addSlider("New tag starting Y", -30, 30, &app->sceneManager.cloudTagMan.newTagStartY, length, dim);
 //   gui->addSlider("New tag starting Z", 0, 50, &app->sceneManager.cloudTagMan.newTagStartZ, length, dim);
   
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


