#include "DemographicData.h"

void DemographicData::setup(ofxXmlSettings *XML)
{
    parseXML(XML);
}



void DemographicData::parseXML(ofxXmlSettings *XML)
{
    name = XML->getValue("name", "");
    debugColour.r = XML->getValue("debug_colour_r", 0);
    debugColour.g = XML->getValue("debug_colour_g", 0);
    debugColour.b = XML->getValue("debug_colour_b", 0);
}
