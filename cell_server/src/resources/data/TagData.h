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
	//TagData(ofxXmlSettings XML, vector<DemographicData*> demographicData);
    //virtual ~TagData();
    //void test(ofxXmlSettings XML);
	void setup(ofxXmlSettings *XML, vector<DemographicData> *demographicData);
    void parseXML(ofxXmlSettings *XML, vector<DemographicData> *demographicData);
    void loadImage();


	void	begin();
	void	end();
	void	bind();
	void	unbind();
	void	testDraw();




    string word;
    vector<Demographic> demographics;
	ofImage tagImg;



	
	ofFbo fbo;
	ofBlendMode blendMode;

	string typeStr;

	bool isGrabFbo;

protected:
private:
};

#endif // TAGDATA_H
