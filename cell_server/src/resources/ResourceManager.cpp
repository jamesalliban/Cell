#include "ResourceManager.h"


void ResourceManager::init()
{
    tagFont.loadFont("type/Arial Rounded Bold.ttf", 34);
	tagFont.setLineHeight(34.0f);
	tagFont.setLetterSpacing(1.037);

    //if( XML.loadFile("xml/demographic_data.xml") ){
    if( demographicXml.loadFile("xml/demographic_data.xml") ){
		printf("Demographic XML loaded! \n");
	}else{
		printf("unable to load Demographic XML check data/ folder \n");
	}

    if( tagXml.loadFile("xml/tag_data.xml") ){
		printf("Tag XML loaded! \n");
	}else{
		printf("unable to load Tag XML check data/ folder \n");
	}


	parseXML();
}


void ResourceManager::parseXML()
{
    // build demographic vector
    demographicXml.pushTag("demographics");
    demographicAmount = demographicXml.getNumTags("demographic");
    printf("demographicAmount = %i\n", demographicAmount );
    for (int i = 0; i < demographicAmount; i++)
    {
        demographicXml.pushTag("demographic", i);
        DemographicData d;
		d.setup(&demographicXml);
        demographicData.push_back(d);
        printf("i: %i, name =  %s\n", i, d.name.c_str());
        demographicXml.popTag();
    }
    demographicXml.popTag();
	
     // build tag vector
    tagXml.pushTag("tags");
    int tagAmount = tagXml.getNumTags("tag");
   // tagAmount = 30;
    for (int i = 0; i < tagAmount; i++)
    {
        tagXml.pushTag("tag", i);
        TagData t;
		t.setup(&tagXml, &demographicData);
        tagData.push_back(t);
        printf("i:%i, word = %s\n", i, t.word.c_str());
        tagXml.popTag();
    }
    tagXml.popTag();
}



string ResourceManager::getRandomDemographic()
{
    int randomDemographicIndex = (int)ofRandom(0, demographicAmount);
    return demographicData[randomDemographicIndex].name;
}
