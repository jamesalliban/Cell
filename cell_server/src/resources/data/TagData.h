#pragma once

#include "ofMain.h"
#include "DemographicData.h"
#include "ofxXmlSettings.h"

#include "CellGlobals.h"


#ifdef CHINESE_CELL
#include "ofxFontStash.h"
#endif

struct Demographic{
    DemographicData* demographicData;
    float strength;
};

class TagData
{
    
public:

#ifdef CHINESE_CELL
	void setup(ofXml XML, ofxFontStash &tagFont, vector<DemographicData> *demographicData, ofShader *blackToAlphaShader);
	void parseXML(ofXml XML, vector<DemographicData> *demographicData);
#else
	void setup(ofxXmlSettings *XML, ofTrueTypeFont *tagFont, vector<DemographicData> *demographicData, ofShader *blackToAlphaShader);
    void parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData);
#endif
	

	void	begin();
	void	end();
	void	checkCharHeight(char letter);



    string word;
    vector<Demographic> demographics;

	float width, height;
	bool doesTextContainAscender, doesTextContainDescender;
	
	ofFbo fbo;
	ofFbo alphaFbo;
	ofBlendMode blendMode;

	bool isGrabFbo;

protected:
private:
};

