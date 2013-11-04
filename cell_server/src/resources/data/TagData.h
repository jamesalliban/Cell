#ifndef TAGDATA_H
#define TAGDATA_H

#include "ofMain.h"
#include "DemographicData.h"
#include "ofxXmlSettings.h"

#define CHINESE_CELL

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
	void setup(ofxXmlSettings *XML, ofxFontStash *tagFont, vector<DemographicData> *demographicData, ofShader *blackToAlphaShader);
#else
	void setup(ofxXmlSettings *XML, ofTrueTypeFont *tagFont, vector<DemographicData> *demographicData, ofShader *blackToAlphaShader);
#endif
	void parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData);


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

#endif // TAGDATA_H
