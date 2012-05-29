#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
    //ctor
}

ResourceManager::~ResourceManager()
{
    //dtor
}


void ResourceManager::init()
{
    tagFont.loadFont("type/Arial Rounded Bold.ttf", 34);
	tagFont.setLineHeight(34.0f);
	tagFont.setLetterSpacing(1.037);

    //if( XML.loadFile("xml/demographic_data.xml") ){
    if( XML.loadFile("xml/demographic_data.xml") ){
		printf("XML loaded! \n");
	}else{
		printf("unable to load XML check data/ folder \n");
	}

	parseXML();
}


void ResourceManager::parseXML()
{
    // build demographic vector
    XML.pushTag("cell");
    XML.pushTag("demographics");
    demographicAmount = XML.getNumTags("demographic");
    for (int i = 0; i < demographicAmount; i++)
    {
        XML.pushTag("demographic", i);
        DemographicData* d = new DemographicData(XML);
        demographicData.push_back(d);
        //printf("%s\n",  d->name.c_str());
        XML.popTag();
    }
    XML.popTag();
    XML.popTag();

     // build tag vector
    XML.pushTag("cell");
    XML.pushTag("tags");
    int tagAmount = XML.getNumTags("tag");
   // tagAmount = 30;
    for (int i = 0; i < tagAmount; i++)
    {
        XML.pushTag("tag", i);
        TagData* t = new TagData(XML, demographicData);
        tagData.push_back(t);
        XML.popTag();
    }
    XML.popTag();
    XML.popTag();
}



string ResourceManager::getRandomDemographic()
{
    int randomDemographicIndex = (int)ofRandom(0, demographicAmount);
    return demographicData[randomDemographicIndex]->name;
}
