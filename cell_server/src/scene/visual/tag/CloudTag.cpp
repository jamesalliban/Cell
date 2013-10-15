/*
 *  CloudTag.cpp
 *  cell
 *
 *  Created by James Alliban on 09/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */


#include "CloudTag.h"
#include "TestApp.h"
#include "CloudTagManager.h"
TestApp* app;
CloudTagManager* cloudTagMan;



void CloudTag::init(ofShader* shad, TagData* tData, int _id)
{
    app = (TestApp*)ofGetAppPtr();
    cloudTagMan = &app->sceneManager.cloudTagMan;

	shader = shad;
	tagData = tData;
	id = _id;

	for (int i = 0; i < SKELETON_MAX; i++)
	{
        TrackedUserData userData;
        userData.jointIndex = (int)ofRandom(0, 20);
        userData.joint2Index = getSecondJointIndex(userData.jointIndex);
        userData.jointOffset = ofRandomuf();
        userData.lowerBodyAdd = 0;
        if (userData.jointIndex >= 13 || userData.jointIndex >= 17) userData.lowerBodyAdd = 500; // knee
        if (userData.jointIndex >= 14 || userData.jointIndex >= 18) userData.lowerBodyAdd = 1000; // ankle
        if (userData.jointIndex >= 15 || userData.jointIndex >= 19) userData.lowerBodyAdd = 1000; // foot
        userData.user = &app->sceneManager.userManager.users[i];
        userData.userPointOffset = ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());
        userData.attraction = ofVec3f(0.0, 0.0, 0.0);
        userData.averageAttractionVec = ofVec3f(0.0, 0.0, 0.0);
        userData.attractionVecMax = 3;
        userData.isActive = false;
        userDataObjects.push_back(userData);
	}

	isTagAttractedToUser = false;

	rotationYMax = 60;

	lineStartVertexIndex = (ofRandomuf() < 0.5) ? 2 : 3;
	scaleOffset = 1.0;

	xScaleSquish = 1.0;

	position    = ofVec3f(ofRandom(-1.0f, 1.0f) * 10, (ofRandom(0.2f) + 0.2f) * 10 + 20, ofRandom(-1.0f, 1.0f) * 10);
	rotations   = ofVec3f(0, ofRandom(-rotationYMax, rotationYMax), 0);
	attractionVelocity = ofVec3f(0.0, 0.0, 0.0);
	//ofEnableNormalizedTexCoords();

	noiseStartAdd = ofRandom(0, 10000);

	float tagSize =  ofRandom(0.2, 0.5);
	float tagWidth = ((float)tagData->tagImg.width / 40.0f) * tagSize;
	float tagHeight = ((float)tagData->tagImg.height / 40.0f) * tagSize;

	tagW = tagWidth;

	tagPlaneMesh.addVertex(ofVec3f(-tagWidth*0.5, tagHeight, 0));    //ofVec3f(position->x, position->y + tagHeight, position->z));
	tagPlaneMesh.addVertex(ofVec3f(tagWidth*0.5, tagHeight, 0));    //ofVec3f(position->x + tagWidth, position->y + tagHeight, position->z));
	tagPlaneMesh.addVertex(ofVec3f(tagWidth*0.5, 0, 0));    //ofVec3f(position->x + tagWidth, position->y, position->z));
	tagPlaneMesh.addVertex(ofVec3f(-tagWidth*0.5, 0, 0));    //ofVec3f(position->x, position->y, position->z));


	tagPlaneMesh.addTexCoord(ofVec2f(0.0f, 0.0f));
	tagPlaneMesh.addTexCoord(ofVec2f(tagData->tagImg.width, 0.0f));
	tagPlaneMesh.addTexCoord(ofVec2f(tagData->tagImg.width, tagData->tagImg.height));
	tagPlaneMesh.addTexCoord(ofVec2f(0.0f, tagData->tagImg.height));

//	tagPlaneMesh.addTexCoord(ofVec2f(0.0f, 0.0f));
//	tagPlaneMesh.addTexCoord(ofVec2f(160.0f / 256.0f, 0.0f));
//	tagPlaneMesh.addTexCoord(ofVec2f(160.0f / 256.0f, 40.0f / 64.0f));
//	tagPlaneMesh.addTexCoord(ofVec2f(0.0f, 40.0f / 64.0f));

//	tagPlaneMesh.addColor(ofFloatColor(1.0, 0.0, 0.0, 1.0));
//	tagPlaneMesh.addColor(ofFloatColor(1.0, 0.0, 0.0, 1.0));
//	tagPlaneMesh.addColor(ofFloatColor(1.0, 0.0, 0.0, 1.0));
//	tagPlaneMesh.addColor(ofFloatColor(1.0, 0.0, 0.0, 1.0));


	tagPlaneMesh.addNormal(ofVec3f(0.0f, 0.0f, -1.0f));
	tagPlaneMesh.addNormal(ofVec3f(0.0f, 0.0f, -1.0f));
	tagPlaneMesh.addNormal(ofVec3f(0.0f, 0.0f, -1.0f));
	tagPlaneMesh.addNormal(ofVec3f(0.0f, 0.0f, -1.0f));

	tagPlaneVbo.setMesh(tagPlaneMesh, GL_STATIC_DRAW);

//    outOfBoundsPosXAdd = 0.0;
//    outOfBoundsPosYAdd = 0.0;
//    outOfBoundsPosZAdd = 0.0;
    outOfBoundsPosVelocity = ofVec3f(0, 0, 0);

	isGrabFbo = true;
}




void CloudTag::update()
{
    scaleOffset = 1.0;

    mappedBourdaryW = ofMap(position.z, cloudTagMan->boundaryD, -cloudTagMan->boundaryD, cloudTagMan->boundaryWFront, cloudTagMan->boundaryW);

    //ofVec3f lastPosition = ofVec3f(position.x, position.y, position.z);

    performAmbientMotion();
    performUserAttraction();

    scaleOffset += app->sceneManager.fieldMan.addFieldScale(&position);

    scaleOffset *= ofMap(position.y, cloudTagMan->yMin, cloudTagMan->yMax, cloudTagMan->scaleMin, cloudTagMan->scaleMax);

    position += ambientVelocity;

    app->sceneManager.fieldMan.addFieldForce(&position);


    for (int i = 0; i < SKELETON_MAX; i++)
	{
        TrackedUserData* userData = &userDataObjects[i] ;
        if (!userData->isActive)
        {


            position -= userData->averageAttractionVec;
            userData->averageAttractionVec *= 0.95;
        }
	}

	/*
    if (!isTagAttractedToUser)
    {
        position -= attractionVelocity;
        attractionVelocity.x *= 0.95;
        attractionVelocity.y *= 0.95;
        attractionVelocity.z *= 0.95;
       // if (attractionVelocity.y > 0.1 || attractionVelocity.y < -0.1)
        //printf("attVel.x = %f, attVel.y = %f, attVel.z = %f \n", attractionVelocity.x, attractionVelocity.y, attractionVelocity.z);

    }
    */
}


void CloudTag::customDraw()
{
    if (cloudTagMan->areTagsEnabled) drawTags();
    if (cloudTagMan->areLinesEnabled) drawLines();
}



void CloudTag::performAmbientMotion()
{
    float nX = position.x + (normSize - 0.5) * cloudTagMan->perlinInputOffsetMultiplier;
    float nY = position.y + (normSize - 0.5) * cloudTagMan->perlinInputOffsetMultiplier;
    float t = (ofGetFrameNum() * cloudTagMan->frameMultiplier) + (nX + nY);
    angleX += ofSignedNoise(t, 0, 0) * cloudTagMan->noiseMultiplier;
    angleY += ofSignedNoise(0, t, 0) * cloudTagMan->noiseMultiplier;
    angleZ += ofSignedNoise(0, 0, t) * cloudTagMan->noiseMultiplier;

    ambientVelocity = ofVec3f(0, 0, 0);

    float posXDiffPos = position.x - mappedBourdaryW * 0.5;
    float posXDiffNeg = -mappedBourdaryW * 0.5 - position.x;
    float posYDiffPos = position.y - (cloudTagMan->boundaryH * 0.5 + cloudTagMan->boundaryCentreH);
    float posYDiffNeg = (-cloudTagMan->boundaryH * 0.5) + cloudTagMan->boundaryCentreH - position.y;
    float posZDiffPos = position.z - cloudTagMan->boundaryD * 0.5;
    float posZDiffNeg = -cloudTagMan->boundaryD * 0.5 - position.z;

    ambientVelocity.x = cos(angleX) * cloudTagMan->speed;
    ambientVelocity.y = cos(angleY) * cloudTagMan->speed;
    ambientVelocity.z = sin(angleZ) * cloudTagMan->speed;

    checkBounds();

    ambientVelocity += outOfBoundsPosVelocity;
}


void CloudTag::performUserAttraction()
{
    // loop through all users to see if they are close enough to attach
	for (int i = 0; i < SKELETON_MAX; i++)
	{
        TrackedUserData* userData = &userDataObjects[i];

        if (userData->user->isActive)
        {
            // calculate user point - between jointIndex + jointIndex2
            userData->userPoint = userData->user->jointPositions[userData->jointIndex].getInterpolated(userData->user->jointPositions[userData->joint2Index], userData->jointOffset);
            userData->userPoint += userData->userPointOffset;


            ofVec3f length = position - userData->userPoint;
            userData->lengthSquared = length.lengthSquared();
            float lengthSquaredMin = cloudTagMan->lineLengthSquaredMin + userData->lowerBodyAdd;

            float demographicStrength = getDemographicStrength(userData);
            if (userData->lengthSquared < lengthSquaredMin && demographicStrength != 0)
            {
                if (userData->isActive == false)
                {
                    userData->millisBecameActive = ofGetElapsedTimeMillis();
                    userData->secondsSinceActive = 0.0;
                }
                userData->isActive = true;
                userData->secondsSinceActive = ((float)ofGetElapsedTimeMillis() - (float)userData->millisBecameActive) / 1000;

//                if (id % 50 == 0 && ofGetFrameNum() % 30 == 0)
//                    printf("ofGetElapsedTimeMillis = %i, millisBecameActive = %i, secondsSinceActive = %f \n",ofGetElapsedTimeMillis(), userData->millisBecameActive, userData->secondsSinceActive);

                float normalisedDistance = (userData->lengthSquared / lengthSquaredMin);

                // reduduse the ambient ambientVelocity as the tag gets closer to the joint.
                ambientVelocity *= normalisedDistance;

                float attractionMultiplier = ofMap(normalisedDistance, 0, 1, cloudTagMan->attractionSpeedMax, cloudTagMan->attractionSpeedMin, true);

                if (!cloudTagMan->isTagAttractionPaused)
                {
                    userData->attraction = (position - userData->userPoint) * pow(attractionMultiplier, cloudTagMan->attractionSpeedPow);
                    position -= userData->attraction;

                    //userData->attractionVecs.push_back(ofVec3f(userData->attraction.x, userData->attraction.y, userData->attraction.z));
                    vector<ofVec3f>::iterator it = userData->attractionVecs.begin();
                    userData->attractionVecs.insert(it, ofVec3f(userData->attraction.x, userData->attraction.y, userData->attraction.z));

                    if (userData->attractionVecs.size() > userData->attractionVecMax)
                        userData->attractionVecs.pop_back();

                    ofVec3f averageAttraction = ofVec3f(0.0, 0.0, 0.0);
                    for (int i = 0; i < (int)userData->attractionVecs.size(); i++)
                    {
                        averageAttraction += userData->attractionVecs[i];
                    }
                    userData->averageAttractionVec = averageAttraction / 3;
                }
            }
            else
            {
                userData->isActive = false;
                userData->millisBecameActive = ofGetElapsedTimeMillis();
                userData->secondsSinceActive = 0.0;
            }
        }
        else
        {
            userData->isActive = false;
            userData->millisBecameActive = ofGetElapsedTimeMillis();
            userData->secondsSinceActive = 0.0;
        }
	}
}


void CloudTag::checkBounds()
{
    bool isXOutOfBounds = false;
    bool isYOutOfBounds = false;
    bool isZOutOfBounds = false;

    if (position.x >= mappedBourdaryW * 0.5 && ambientVelocity.x > -0.3)
    {
        isXOutOfBounds = true;

        outOfBoundsPosVelocity.x = MAX(outOfBoundsPosVelocity.x - cloudTagMan->outOfBoundsPosAddMin, -cloudTagMan->outOfBoundsPosAddMax);
        angleX += cloudTagMan->angleAdd;
    }
    if (position.x <= mappedBourdaryW * -0.5 && ambientVelocity.x < 0.3)
    {
        isXOutOfBounds = true;
        outOfBoundsPosVelocity.x = MIN(outOfBoundsPosVelocity.x + cloudTagMan->outOfBoundsPosAddMin, cloudTagMan->outOfBoundsPosAddMax);
        angleX -= cloudTagMan->angleAdd;
    }
    if (position.y >= (cloudTagMan->boundaryH * 0.5) + cloudTagMan->boundaryCentreH)
    {
        isYOutOfBounds = true;
        outOfBoundsPosVelocity.y = MAX(outOfBoundsPosVelocity.y - cloudTagMan->outOfBoundsPosAddMin, -cloudTagMan->outOfBoundsPosAddMax);
        if (ambientVelocity.y > -0.3) angleY += cloudTagMan->angleAdd;
    }
    if (position.y <= cloudTagMan->boundaryH * -0.5 + cloudTagMan->boundaryCentreH)
    {
        isYOutOfBounds = true;
        outOfBoundsPosVelocity.y = MIN(outOfBoundsPosVelocity.y + cloudTagMan->outOfBoundsPosAddMin, cloudTagMan->outOfBoundsPosAddMax);
        if (ambientVelocity.y < 0.3) angleY -= cloudTagMan->angleAdd;
    }
    if (position.z >= cloudTagMan->boundaryD * 0.5 && ambientVelocity.z > -0.3)// && ambientVelocity.z > 0)
    {
        isZOutOfBounds = true;
        outOfBoundsPosVelocity.z = MAX(outOfBoundsPosVelocity.z - cloudTagMan->outOfBoundsPosAddMin, -cloudTagMan->outOfBoundsPosAddMax);
        angleZ += cloudTagMan->angleAdd;
    }
    if (position.z <= cloudTagMan->boundaryD * -0.5 && ambientVelocity.z < 0.3)// && ambientVelocity.x < 0)
    {
        isZOutOfBounds = true;
        outOfBoundsPosVelocity.z = MIN(outOfBoundsPosVelocity.z + cloudTagMan->outOfBoundsPosAddMin, cloudTagMan->outOfBoundsPosAddMax);
        angleZ -= cloudTagMan->angleAdd;
    }

    if (!isXOutOfBounds && outOfBoundsPosVelocity.x > 0)
        outOfBoundsPosVelocity.x = MAX(outOfBoundsPosVelocity.x - (cloudTagMan->outOfBoundsPosAddMin * cloudTagMan->outOfBoundsPosAddDecay), 0);
    if (!isYOutOfBounds && outOfBoundsPosVelocity.y > 0)
        outOfBoundsPosVelocity.y = MAX(outOfBoundsPosVelocity.y - (cloudTagMan->outOfBoundsPosAddMin * cloudTagMan->outOfBoundsPosAddDecay), 0);
    if (!isZOutOfBounds && outOfBoundsPosVelocity.z > 0)
        outOfBoundsPosVelocity.z = MAX(outOfBoundsPosVelocity.z - (cloudTagMan->outOfBoundsPosAddMin * cloudTagMan->outOfBoundsPosAddDecay), 0);


    int tempIsTagAttractedToUser = (isTagAttractedToUser) ? 1 : 0;

    isTagAttractedToUser = false;
    for (int i = 0; i < SKELETON_MAX; i++)
	{
        TrackedUserData* userData = &userDataObjects[i];
        if (userData->isActive)
        {
            //if (id % 20 == 0 && ofGetFrameNum() % 30 == 0) printf("userdata is active \n");
            isTagAttractedToUser = true;
            continue;
        }
	}

	if (tempIsTagAttractedToUser == 1 && !isTagAttractedToUser)
	{
	    outOfBoundsPosVelocity = ofVec3f(0.0, 0.0, 0.0);
	}
}



void CloudTag::drawTags()
{
	glDepthFunc(GL_LESS);

	shader->begin(); // shader begin: set values.


	if (isGrabFbo || app->sceneManager.isUpdateVars)
	{
		isGrabFbo = false;
		//shader->setUniform1f("blendmix", app->sceneManager.cloudTagMan.shadeBlendMix);
		//shader->setUniform1i("blendmode", app->sceneManager.cloudTagMan.shadeBlendMode);
	}

	float mappedContrast = ofMap(position.z, -50, 50, cloudTagMan->shadeContrastMin, cloudTagMan->shadeContrastMax, true);
	float mappedBrightness = ofMap(position.z, -50, 50, cloudTagMan->shadeBrightnessMin, cloudTagMan->shadeBrightnessMax, true);
	float mappedAlpha = ofMap(position.z, -50, 50, cloudTagMan->shadeAlphaMin, cloudTagMan->shadeAlphaMax, true);
	shader->setUniform1f("contrast", mappedContrast);
	shader->setUniform1f("brightness",	mappedBrightness);
	ofVec3f colourOffset = app->sceneManager.fieldMan.getColourOffset(&position);
	shader->setUniform1f("red", colourOffset.x);
	shader->setUniform1f("green", colourOffset.y);
	shader->setUniform1f("blue", colourOffset.z);
	shader->setUniform1f("alpha", mappedAlpha);
	shader->setUniformTexture("baseMap", tagData->fbo.getTextureReference(), 0);


    int div = 500;
   // float noise = ofNoise(((float)ofGetFrameNum() + noiseStartAdd) / div, ((float)ofGetFrameNum() + noiseStartAdd) / div);
  // float angle = ofMap(noise, 0, 1, -rotationYMax, rotationYMax);
	//rotations.y = angle;
	//rotations->y = MAX(-rotationYMax, rotations->y);
	//rotations->y = MIN(rotations->y, rotationYMax);

	ofVec3f cornerVertexVec = tagPlaneMesh.getVertex(lineStartVertexIndex);

    ofPushMatrix();
    //ofTranslate(position.x, position.y, position.z);
    ofTranslate(position.x + cornerVertexVec.x, position.y + cornerVertexVec.y, position.z + cornerVertexVec.z);//
    ofScale(scaleOffset * xScaleSquish, scaleOffset, scaleOffset);
//    ofRotateX(rotations->x);
//    ofRotateY(rotations->y);
//    ofRotateZ(rotations->z);
    setOrientation(ofVec3f(ofRandom(0, 20), ofRandom(0, 20), ofRandom(0, 20)));
	tagPlaneVbo.draw(GL_QUADS, 0, 4);
	ofPopMatrix();

	shader->end();
}



void CloudTag::drawLines()
{

	ofVec3f cornerVertexVec = tagPlaneMesh.getVertex(lineStartVertexIndex);
	float tagW = tagPlaneMesh.getVertex(3).x * 2;
	float tagH = tagPlaneMesh.getVertex(3).y * 2;

	float scaleCorrectionX = (tagW - (tagW * (scaleOffset * xScaleSquish))) * 0.5;
	//scaleCorrectionX *= 0.6;
	float scaleCorrectionY = (tagH - (tagH * scaleOffset)) * 0.5;

	if (lineStartVertexIndex == 2)
	{
	    scaleCorrectionX *= -1;
        scaleCorrectionY *= -1;
	}

	for (int i = 0; i < SKELETON_MAX; i++)
	{
        TrackedUserData* userData = &userDataObjects[i];
        if (getDemographicStrength(userData) == 0.0) continue;

        if (userData->isActive)
        {
 //           shader->begin(); // shader begin: set values.

            if (isGrabFbo || app->sceneManager.isUpdateVars)
            {
                isGrabFbo = false;
                //shader->setUniform1f("blendmix", app->sceneManager.cloudTagMan.shadeBlendMix);
                //shader->setUniform1i("blendmode", app->sceneManager.cloudTagMan.shadeBlendMode);
            }

            float mappedContrast = ofMap(position.z, cloudTagMan->lineZAreaMin, cloudTagMan->lineZAreaMax, cloudTagMan->lineContrastMin, cloudTagMan->lineContrastMax, true);
            float mappedBrightness = ofMap(position.z, cloudTagMan->lineZAreaMin, cloudTagMan->lineZAreaMax, cloudTagMan->lineBrightnessMin, cloudTagMan->lineBrightnessMax, true);
            float mappedAlpha = ofMap(position.z, cloudTagMan->lineZAreaMin, cloudTagMan->lineZAreaMax, cloudTagMan->lineAlphaMin, cloudTagMan->lineAlphaMax, true);
            float mappedDistance = ofMap(userData->lengthSquared, cloudTagMan->mapLineDistanceMax, cloudTagMan->mapLineDistanceMin, 0.0, 1.0, true);
            float mappedStartTime = ofMap(userData->secondsSinceActive, 0.0, cloudTagMan->lineFadeTime, 0.0, 1.0);

/*
            shader->setUniform1f("contrast", mappedContrast * mappedDistance);
            shader->setUniform1f("brightness",	mappedBrightness * mappedDistance);
            shader->setUniform1f("alpha", 0.3);//(mappedAlpha * mappedDistance) * ofClamp(mappedStartTime, 0, 1));
            shader->setUniform1f("red", 1.0);
            shader->setUniform1f("green", 1.0);
            shader->setUniform1f("blue", 1.0);
*/
            //ofEnableAlphaBlending();
            //ofSetColor(255, 255, 255, 255);
            float lineSize = 10;


//            ofLine(position.x + scaleCorrectionX, position.y + scaleCorrectionY, position.z, userData->userPoint.x, userData->userPoint.y, userData->userPoint.z);

            glColor4f(1.0f, 1.0f, 1.0f, (mappedAlpha * mappedDistance) * ofClamp(mappedStartTime, 0, 1));// mappedDistance);
            ofSetLineWidth(cloudTagMan->lineThickness);
            glBegin(GL_LINES);
                glVertex3f(position.x + scaleCorrectionX, position.y + scaleCorrectionY, position.z);
                glVertex3f(userData->userPoint.x, userData->userPoint.y, userData->userPoint.z);
            glEnd();

 //           shader->end();



            //glColor4f(1.0f, 1.0f, 1.0f, mappedAlpha);
            ofSetColor(255, 255, 255, (mappedAlpha * 200) * ofClamp(mappedStartTime, 0, 1));
            ofPushMatrix();
            ofTranslate(userData->userPoint.x, userData->userPoint.y, userData->userPoint.z);//
            ofCircle(0, 0, 0.1);
            ofPopMatrix();


        }
	}
}



void CloudTag::updateVars()
{
    normSize = ofRandomuf();
    boundaryOffset = ofRandom(0, cloudTagMan->boundaryOffsetMax);

    float posY = ofRandom(-cloudTagMan->boundaryH * 0.5 + cloudTagMan->boundaryCentreH, cloudTagMan->boundaryH * 0.5 + cloudTagMan->boundaryCentreH);
    float posZ = ofRandom(-cloudTagMan->boundaryD * 0.5, cloudTagMan->boundaryD * 0.5);

    mappedBourdaryW = ofMap(posZ, cloudTagMan->boundaryD, -cloudTagMan->boundaryD, cloudTagMan->boundaryWFront, cloudTagMan->boundaryW);

    float posX = ofRandom(-mappedBourdaryW * 0.5, mappedBourdaryW * 0.5);

    position = ofVec3f(posX, posY, posZ);

    outOfBoundsPosVelocity = ofVec3f(0, 0, 0);
}


// get tag-to-demographic strength
float CloudTag::getDemographicStrength(TrackedUserData* userData)
{
    // loop through all the demographics that are associated with this tag to get the strength
    for (int i = 0; i < (int)tagData->demographics.size(); i++)
    {
        DemographicData* demographicData = tagData->demographics[i].demographicData;
        string demographicName = demographicData->name;
        int comparison = demographicName.compare(userData->user->demographic);

        if (comparison == 0)
        {
            return tagData->demographics[i].strength;
        }
    }

    return 0.0; //TODO: make variable and add this to GUI
}


int CloudTag::getSecondJointIndex(int jointIndex)
{
    switch(jointIndex)
    {
        case CELL_HIP_CENTRE: 
			return (ofRandomuf() < 0.5) ? CELL_HIP_LEFT : CELL_HIP_RIGHT;
			break;
        case CELL_SPINE: 
			return CELL_HIP_CENTRE;
			break;
        case CELL_SHOULDER_CENTRE: 
			return CELL_SPINE;
			break;
        case CELL_HEAD: 
			return CELL_SHOULDER_CENTRE;
			break;
        case CELL_SHOULDER_LEFT: 
			return CELL_SHOULDER_CENTRE;
			break;
        case CELL_ELBOW_LEFT: 
			return CELL_SHOULDER_LEFT;
			break;
        case CELL_WRIST_LEFT: 
			return CELL_ELBOW_LEFT;
			break;
        case CELL_HAND_LEFT: 
			return CELL_WRIST_LEFT;
			break;
        case CELL_SHOULDER_RIGHT: 
			return CELL_SHOULDER_CENTRE;
			break;
        case CELL_ELBOW_RIGHT: 
			return CELL_SHOULDER_RIGHT;
			break;
        case CELL_WRIST_RIGHT: 
			return CELL_ELBOW_RIGHT;
			break;
        case CELL_HAND_RIGHT: 
			return CELL_WRIST_RIGHT;
			break;
        case CELL_HIP_LEFT: 
			return CELL_KNEE_LEFT;
			break;
        case CELL_KNEE_LEFT: 
			return CELL_ANKLE_LEFT;
			break;
        case CELL_ANKLE_LEFT: 
			return CELL_FOOT_LEFT;
			break;
        case CELL_FOOT_LEFT: 
			return CELL_ANKLE_LEFT;
			break;
        case CELL_HIP_RIGHT: 
			return CELL_KNEE_RIGHT;
			break;
        case CELL_KNEE_RIGHT: 
			return CELL_ANKLE_RIGHT;
			break;
        case CELL_ANKLE_RIGHT: 
			return CELL_FOOT_RIGHT;
			break;
        case CELL_FOOT_RIGHT: 
			return CELL_ANKLE_RIGHT;
			break;
    }
}
