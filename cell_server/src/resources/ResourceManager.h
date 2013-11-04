#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "DemographicData.h"
#include "TagData.h"

#define CHINESE_CELL


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
#endif

	ofxXmlSettings demographicXml;
	ofxXmlSettings tagXml;

	int demographicAmount;
	int tagAmount;

	vector<DemographicData> demographicData;
	vector<TagData> tagData;

	ofShader blackToAlphaShader;

protected:
private:

};

#endif // RESOURCEMANAGER_H
