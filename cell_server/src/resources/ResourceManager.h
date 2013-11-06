#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "DemographicData.h"
#include "TagData.h"

#include "CellGlobals.h"


#ifdef CHINESE_CELL
#include "ofxFontStash.h"
#endif

class ResourceManager
{
public:
    void init();
    void parseXML();
    string getRandomDemographic();
	
#ifdef CHINESE_CELL
	ofxFontStash unicodeFont;
	ofXml chineseXML;
#else
	ofTrueTypeFont  tagFont;
	ofxXmlSettings tagXml;
#endif

	ofxXmlSettings demographicXml;

	int demographicAmount;
	int tagAmount;

	vector<DemographicData> demographicData;
	vector<TagData> tagData;

	ofShader blackToAlphaShader;

protected:
private:

};
