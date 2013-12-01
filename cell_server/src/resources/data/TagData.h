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
	void setup(ofXml XML, ofxFontStash *_tagFont, vector<DemographicData> *demographicData, ofShader *_blackToAlphaShader);
	void parseXML(ofXml XML, vector<DemographicData> *demographicData);
#else
	void setup(ofxXmlSettings *XML, ofTrueTypeFont *_tagFont, vector<DemographicData> *demographicData, ofShader *_blackToAlphaShader);
    void parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData);
#endif
	
    
    void update();
	void begin();
	void end();
    
    void createTagFbo();

	void checkCharHeight(char letter);
    void startChangeTagData(string latestTag);
    void changeTagData();
    
#ifdef CHINESE_CELL
    ofxFontStash *tagFont;
#else
    ofTrueTypeFont *tagFont;
#endif
    
    ofShader *blackToAlphaShader;
    static float textAlphaAdd;
    
    string word;
    vector<Demographic> demographics;

	float width, height;
	bool doesTextContainAscender, doesTextContainDescender;
	
	ofFbo fbo;
	ofFbo alphaFbo;
	ofBlendMode blendMode;

	bool isGrabFbo;
    
    
    string newWord;
    float alphaModifier;
    float alphaReductionSpeed;
    float alphaAdditionSpeed;
    float isAnimatingAlpha;
    float isFadingOut;
    bool isTagUpdated;

protected:
private:
};

