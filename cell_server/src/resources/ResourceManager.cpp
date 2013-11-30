#include "ResourceManager.h"


void ResourceManager::init()
{
#ifdef CHINESE_CELL  
	unicodeFont.setup("Arial Unicode.ttf");
#else
    tagFont.loadFont("type/western/Arial Rounded Bold.ttf", 72);
	tagFont.setLineHeight(34.0f);
	tagFont.setLetterSpacing(1.037);
#endif

    
    
    
    

    if(demographicXml.loadFile("xml/demographic_data.xml") ){
		printf("Demographic XML loaded! \n");
	}else{
		printf("unable to load Demographic XML check data/ folder \n");
	}


#ifdef CHINESE_CELL
	if (chineseXML.load("xml/tag_data_Chinese.xml")) printf("Chinese tag XML loaded \n");
	else printf("Chinese tag XML not loaded \n");
#else
    if(tagXml.loadFile("xml/tag_data.xml") ){
		printf("Tag XML loaded! \n");
	}else{
		printf("unable to load Tag XML check data/ folder \n");
	}
#endif

	blackToAlphaShader.load("shaders/BlackToAlphaShader");

    numNewTagsAdded = 0;
    
	parseXML();
}


void ResourceManager::parseXML()
{
    // build demographic vector from xml data
    demographicXml.pushTag("demographics");
    demographicAmount = demographicXml.getNumTags("demographic");
    //printf("demographicAmount = %i\n", demographicAmount );
    for (int i = 0; i < demographicAmount; i++)
    {
        demographicXml.pushTag("demographic", i);
        DemographicData d;
		d.setup(&demographicXml);
        demographicData.push_back(d);
        //printf("i: %i, name =  %s\n", i, d.name.c_str());
        demographicXml.popTag();
    }
    demographicXml.popTag();
	
	
    
    // load tag data from XML
    
#ifdef CHINESE_CELL
    
    chineseXML.setTo("tags");
    numTags = chineseXML.getNumChildren();
    printf("- - - numTags = %i\n", numTags);
    chineseXML.setTo("tag[0]");
    for (int i = 0; i < numTags; i++)
    {
        //printf("i:%i, word = %s\n", i, chineseXML.getValue("word").c_str());
        TagData t;
        ofXml tagElement;
        tagElement.addXml(chineseXML, false);
		t.setup(tagElement, &unicodeFont, &demographicData, &blackToAlphaShader);
        tagData.push_back(t);
        
        chineseXML.setToSibling();
    }

#else

    // build tag vector
    tagXml.pushTag("tags");
    int tagAmount = tagXml.getNumTags("tag");
   // tagAmount = 30;
    for (int i = 0; i < tagAmount; i++)
    {
        tagXml.pushTag("tag", i);
        printf("i:%i, word = %s\n", i, tagXml.getValue("word", "").c_str());
        TagData t;
		t.setup(&tagXml, &tagFont, &demographicData, &blackToAlphaShader);
        tagData.push_back(t);
        tagXml.popTag();
    }
    tagXml.popTag();

#endif
}



string ResourceManager::getRandomDemographic()
{
    int randomDemographicIndex = (int)ofRandom(0, demographicAmount);
    return demographicData[randomDemographicIndex].name;
}



void ResourceManager::update()
{
//    if (++currentEllapsedFrames >= xmlCheckFrameFrequency)
//    {
//        remoteTagDataService.startThread(true, false);
//        xmlCheckFrameFrequency = 90;
//        currentEllapsedFrames = 0;
//    }
    
    if (currentlyChangingTag != NULL)
    {
        if (currentlyChangingTag->alphaModifier <= 1)
        {
            currentlyChangingTag->update();
            if (currentlyChangingTag->isTagUpdated)
            {
                printf("^^^^^^^^^^^^^^^ TAG UPDATED _ START THREAD HERE ££££££££££££££££££££\n");
                startThread(true, false);
            }
        }
        else
        {
            currentlyChangingTag->alphaModifier = 1;
            currentlyChangingTag->isAnimatingAlpha = false;
            currentlyChangingTag = NULL;
        }
    }
}



void ResourceManager::addNewTag(string latestTag, int question)
{
    newTag = latestTag;
    newQuestion = question;
    // use numNewTagsAdded to select one of the existing tags
    currentlyChangingTag = &tagData[numNewTagsAdded];
    // pass new tag data to the tag and tell it to fade out and reappear at the front of the play area (with red bg for debug)
    currentlyChangingTag->startChangeTagData(latestTag);
    
    // assign new demographic data to tag
    
    // add new data to tag XML and export on a separate thread
    

    // add one to numNewTagsAdded
    ++numNewTagsAdded;
    if (numNewTagsAdded >= numTags)
        numNewTagsAdded = 0;
}


void ResourceManager::threadedFunction()
{
    //TODO: kill thread on exit
    printf("threadedFunction() \n");
    
    while(isThreadRunning())
    {
        lock();
        
        ofXml newTagXml;
        newTagXml.addChild("tag");
        newTagXml.addChild("word");
        newTagXml.setValue("word", newTag);
        newTagXml.addChild("question");
        newTagXml.setValue("question", ofToString(newQuestion));
        
        vector<string> demographics;
        for (int i = 0; i < demographicData.size(); i++)
            demographics.push_back(demographicData[i].name);
        random_shuffle(demographics.begin(), demographics.end());
        
        
        for (int i = 0; i < 3; i++)
        {
            newTagXml.addChild("demographic");
            newTagXml.setTo("demographic[" + ofToString(i) + "]");
            
            newTagXml.setAttribute("id", demographics[i]);
            newTagXml.addChild("strength");
            if (i == 0) newTagXml.setValue("strength", ofToString(ofRandom(0.3, 0.5)));
            if (i == 1) newTagXml.setValue("strength", ofToString(ofRandom(0.5, 0.7)));
            if (i == 2) newTagXml.setValue("strength", ofToString(ofRandom(0.7, 0.9)));
            newTagXml.setToParent();
        }
        
        chineseXML.setTo("tag[0]");
        chineseXML.setToParent();
        chineseXML.addXml(newTagXml);
        if (chineseXML.getNumChildren() > TAG_MAX) chineseXML.remove("tag[0]");
        chineseXML.save("xml/tag_data_Chinese.xml");
        
        unlock();
        stopThread();
    }
}