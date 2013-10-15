#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "DemographicData.h"
#include "TagData.h"

class ResourceManager
{
public:
    void init();
    void parseXML();
    string getRandomDemographic();

	ofTrueTypeFont  tagFont;
	ofxXmlSettings demographicXml;
	ofxXmlSettings tagXml;

	int demographicAmount;
	int tagAmount;

	vector<DemographicData> demographicData;
	vector<TagData> tagData;

protected:
private:

};

#endif // RESOURCEMANAGER_H
