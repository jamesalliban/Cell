/*
 *  CloudTagManager.h
 *  cell
 *
 *  Created by James Alliban on 09/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#ifndef _CLOUD_TAG_MANAGER
#define _CLOUD_TAG_MANAGER

#include "ofMain.h"

#include "CloudTag.h"
#include "TagTexture.h"


class CloudTagManager : public ofNode
{

public:
	void init(ofShader* shader);

	void update();
	void customDraw();

	vector<CloudTag> cloudTags;
	int		cloudTagAmount;

	//tag shading
	float shadeContrastMin;
	float shadeContrastMax;
	float shadeBrightnessMin;
	float shadeBrightnessMax;
	float shadeAlphaMin;
	float shadeAlphaMax;
	float shadeBlendMix;
	int shadeBlendMode;

	//line shading
	float lineContrastMin;
	float lineContrastMax;
	float lineBrightnessMin;
	float lineBrightnessMax;
	float lineAlphaMin;
	float lineAlphaMax;

	// line props
	bool areLinesEnabled;
	int lineDistanceMin;
	int lineLengthSquaredMin;
	float lineThickness;
	float lineZAreaMin;
	float lineZAreaMax;
	float lineFadeTime;

	//user attraction
	float attractionMultiplier;
	float mapLineDistanceMin;
	float mapLineDistanceMax;

	float attractionSpeedMin;
	float attractionSpeedMax;
	int attractionSpeedPow;
	bool isTagAttractionPaused;




	bool areTagsEnabled;
	// cloud tag manager update vars
	float perlinInputOffsetMultiplier;
	float speed;
	float frameMultiplier;
	float noiseMultiplier;
	float angleAdd;
	float outOfBoundsPosAddMax;
	float outOfBoundsPosAddMin;
	float outOfBoundsPosAddDecay;

	// cloud boundaries
	float boundaryW;
	float boundaryWFront;
	float boundaryD;
	float boundaryH;
	float boundaryCentreH;
	float boundaryOffsetMax;
	float boundaryOffsetAdd;
	float boundaryOffsetAddMult;

	ofVec3f tempUser;


	// scale mapped to y
	float yMin;
	float yMax;
	float scaleMin;
	float scaleMax;
	bool yScaleOnlyOnAttraction;



protected:


};


#endif
