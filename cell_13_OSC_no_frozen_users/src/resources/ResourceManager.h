#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "DemographicData.h"
#include "TagData.h"

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    void init();
    void parseXML();
    string getRandomDemographic();

	ofTrueTypeFont  tagFont;
	ofxXmlSettings XML;

	int demographicAmount;
	int tagAmount;

	vector<DemographicData*> demographicData;
	vector<TagData*> tagData;

protected:
private:

};

#endif // RESOURCEMANAGER_H
