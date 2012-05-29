#ifndef TAGDATA_H
#define TAGDATA_H

#include "ofMain.h"
#include "DemographicData.h"
#include "ofxXmlSettings.h"

struct Demographic{
    DemographicData* demographicData;
    float strength;
};

class TagData
{
    public:
        TagData(ofxXmlSettings XML, vector<DemographicData*> demographicData);
        virtual ~TagData();
        //void test(ofxXmlSettings XML);
        void parseXML(ofxXmlSettings XML, vector<DemographicData*> demographicData);
        void loadImage();

        string word;
        vector<Demographic> demographics;
        ofImage tagImg;

    protected:
    private:
};

#endif // TAGDATA_H
