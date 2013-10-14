#include "TagData.h"

#include "ofMain.h"

TagData::TagData(ofxXmlSettings XML, vector<DemographicData*> demographicData)
{
    parseXML(XML, demographicData);
    loadImage();
}

TagData::~TagData()
{
    //dtor
}


void TagData::parseXML(ofxXmlSettings XML, vector<DemographicData*> demographicData)
{
    // set word
    word = XML.getValue("word", "");
    //printf("- tag = %s\n",  word.c_str());

    int currentElement = 0;
    // loop through all the demographics in the tag XML
    int numDemoTags = XML.getNumTags("demographic");
    for (int i; i < numDemoTags; i++)
    {
        Demographic demographicStruct;

        // loop through all the available demographics to see if there is a match
        for (int j = 0; j < (int)demographicData.size(); j++)
        {
            DemographicData* dData = demographicData[j];
            int comparison = XML.getAttribute("demographic", "id", "", i).compare(dData->name);

            //printf("  %s  compared with %s = %i \n", dData->name.c_str(), XML.getAttribute("demographic", "id", "", i).c_str(), comparison);

            if (comparison == 0)
            {
                demographicStruct.demographicData = dData;
                XML.pushTag("demographic", currentElement);
                demographicStruct.strength = XML.getValue("strength", 0.0);
                printf("  - word = %s, strength = %f \n", dData->name.c_str(), demographicStruct.strength);
                demographics.push_back(demographicStruct);
                XML.popTag();
                ++currentElement;
            }
        }
    }
}



void TagData::loadImage()
{
    tagImg.loadImage("images/tags/" + word + ".png");
}
