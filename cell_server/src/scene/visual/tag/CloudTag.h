/*
 *  CloudTag.h
 *  cell
 *
 *  Created by James Alliban on 09/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "TagData.h"
#include "User.h"

struct TrackedUserData{
    User* user;
    int jointIndex;
    int joint2Index;
    int lowerBodyAdd;
    float jointOffset;
    ofVec3f userPoint;
    ofVec3f userPointOffset;
    ofVec3f attraction;
    ofVec3f averageAttractionVec;
    vector<ofVec3f> attractionVecs;
    int attractionVecMax;
    float lengthSquared;
    bool isActive;
    float dist;
    int millisBecameActive;
    float secondsSinceActive;
};


class CloudTag : public ofNode
{
public:
	void	    init(ofShader* shad, TagData* tData, int _id);
	void	    update();
	void	    customDraw();
	void        updateVars();
	float       getDemographicStrength(TrackedUserData* userData);


	bool        isGrabFbo;
	vector<TrackedUserData> userDataObjects;
	bool        isTagAttractedToUser;
	// movement
    ofVec3f		position;
	ofVec3f     ambientVelocity;
	ofVec3f     attractionVelocity;
	ofVec3f		rotations;

	TagData*    tagData;



protected:

    void        performAmbientMotion();
    void        checkBounds();
    void        performUserAttraction();
	void        drawTags();
    void        drawLines();
    int         getSecondJointIndex(int jointIndex);


	int         id;
	float       boundaryOffset;

	float       mappedBourdaryW;
	ofVec3f     outOfBoundsPosVelocity;
	float       normSize;

	int         lineStartVertexIndex;
	float       scaleOffset;
	float       xScaleSquish;

	float       angleX;
    float       angleY;
    float       angleZ;


	ofColor		colour;

	//mesh
	ofMesh      tagPlaneMesh;
	ofVbo       tagPlaneVbo;

	//shader
	ofShader*   shader;


	int         rotationYMax;
	int         noiseStartAdd;

	float tagW;


	//attraction

	ofVec3f jointPositionDifference;

};
