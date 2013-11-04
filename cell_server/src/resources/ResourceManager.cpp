#include "ResourceManager.h"


void ResourceManager::init()
{
#ifdef CHINESE_CELL
	//ofxTrueTypeFontUC  tagFont;
	//tagFont.loadFont("type/chinese/MSYH.TTC", 72, true, true);  
	unicodeFont.setup("Arial Unicode.ttf");
#else
    tagFont.loadFont("type/western/Arial Rounded Bold.ttf", 72);
	tagFont.setLineHeight(34.0f);
	tagFont.setLetterSpacing(1.037);
#endif


    if(demographicXml.loadFile("xml/demographic_data.xml") ){
		printf("Demographic XML loaded! \n");
	}else{
		printf("unable to load Demographic XML check data/ folder \n");
	}


#ifdef CHINESE_CELL
	//if(tagXml.loadFile("xml/tag_data_chinese.xml") ){
	//	printf("Chinese Tag XML loaded! \n");
	//}else{
	//	printf("unable to load Chinese Tag XML check data/ folder \n");
	//}
	if (chineseXML.load("tag_data_Chinese.xml")) printf("Chinese tag XML loaded \n");
	else printf("Chinese tag XML not loaded \n");
#else
    if(tagXml.loadFile("xml/tag_data.xml") ){
		printf("Tag XML loaded! \n");
	}else{
		printf("unable to load Tag XML check data/ folder \n");
	}
#endif

	blackToAlphaShader.load("shaders/BlackToAlphaShader");


	parseXML();
}


void ResourceManager::parseXML()
{
    // build demographic vector
    demographicXml.pushTag("demographics");
    demographicAmount = demographicXml.getNumTags("demographic");
    printf("demographicAmount = %i\n", demographicAmount );
    for (int i = 0; i < demographicAmount; i++)
    {
        demographicXml.pushTag("demographic", i);
        DemographicData d;
		d.setup(&demographicXml);
        demographicData.push_back(d);
        printf("i: %i, name =  %s\n", i, d.name.c_str());
        demographicXml.popTag();
    }
    demographicXml.popTag();
	
	
#ifdef CHINESE_CELL

#else

    // build tag vector
    tagXml.pushTag("tags");
    int tagAmount = tagXml.getNumTags("tag");
   // tagAmount = 30;
    for (int i = 0; i < tagAmount; i++)
    {
        tagXml.pushTag("tag", i);
        printf("i:%i, word = %s\n", i, tagXml.getValue("word", "").c_str());
        TagData t;
		t.setup(&tagXml, &tagFont, &demographicData, &blackToAlphaShader);
        tagData.push_back(t);
        tagXml.popTag();
    }
    tagXml.popTag();

#endif
}



string ResourceManager::getRandomDemographic()
{
    int randomDemographicIndex = (int)ofRandom(0, demographicAmount);
    return demographicData[randomDemographicIndex].name;
}