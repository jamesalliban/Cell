#pragma once


#include "ofMain.h"
#include "ofxXmlSettings.h"

class DemographicData
{
    public:
        void setup(ofxXmlSettings *XML);
        //virtual ~DemographicData();
        void parseXML(ofxXmlSettings *XML);

        string name;
        ofColor debugColour;

    protected:
    private:
};

