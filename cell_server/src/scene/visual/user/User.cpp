#include "User.h"
#include "TestApp.h"
#include "UserManager.h"

void User::setup(int _userID)
{
    userID              = _userID;
    clientID            = -1;

    deactivate();
	
    debugColour[0] = 255;
    debugColour[1] = 255;
    debugColour[2] = 255;

    switch(userID)
    {
        case 0: 
			debugColour[0] = 255;
			debugColour[1] = 0;
			debugColour[2] = 0;
			break;       // red
        case 1: 
			debugColour[0] = 0;
			debugColour[1] = 255;
			debugColour[2] = 0;
			break;       // green
        case 2: 
			debugColour[0] = 80;
			debugColour[1] = 80;
			debugColour[2] = 255;
			break;       // blue
        case 3: 
			debugColour[0] = 255;
			debugColour[1] = 255;
			debugColour[2] = 0;
			break;     // yellow
        case 4: 
			debugColour[0] = 255;
			debugColour[1] = 0;
			debugColour[2] = 255;
			break;     // pink
        case 5: 
			debugColour[0] = 0;
			debugColour[1] = 255;
			debugColour[2] = 255;
			break;     // turquoise
        case 6: 
			debugColour[0] = 255;
			debugColour[1] = 255;
			debugColour[2] = 255;
			break;   // white
        case 7: 
			debugColour[0] = 100;
			debugColour[1] = 100;
			debugColour[2] = 100;
			break;   // grey
    }

    TestApp* app = (TestApp*)ofGetAppPtr();

    if (app->isKinectAttached)
    {
        for (int i = 0; i < 20; i++)
        {
            jointPositions.push_back(ofVec3f(0.0, 0, 0));
        }
    }
    else
    {
        buildDebugSkeleton();
    }
}


void User::update()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    UserManager* userMan = &app->sceneManager.userManager;

    if (!isActive) return;


    secondsSinceActive = ((float)ofGetElapsedTimeMillis() - (float)millisBecameActive) / 1000;

    jointPositions[CELL_HIP_CENTRE] = skeleton->skeletonPositions[CELL_HIP_CENTRE];
    jointPositions[CELL_SPINE] = skeleton->skeletonPositions[CELL_SPINE];
    jointPositions[CELL_SHOULDER_CENTRE] = skeleton->skeletonPositions[CELL_SHOULDER_CENTRE];
    jointPositions[CELL_HEAD] = skeleton->skeletonPositions[CELL_HEAD];
    jointPositions[CELL_SHOULDER_LEFT] = skeleton->skeletonPositions[CELL_SHOULDER_LEFT];
    jointPositions[CELL_ELBOW_LEFT] = skeleton->skeletonPositions[CELL_ELBOW_LEFT];
    jointPositions[CELL_WRIST_LEFT] = skeleton->skeletonPositions[CELL_WRIST_LEFT];
    jointPositions[CELL_HAND_LEFT] = skeleton->skeletonPositions[CELL_HAND_LEFT];
    jointPositions[CELL_SHOULDER_RIGHT] = skeleton->skeletonPositions[CELL_SHOULDER_RIGHT];
    jointPositions[CELL_ELBOW_RIGHT] = skeleton->skeletonPositions[CELL_ELBOW_RIGHT];
    jointPositions[CELL_WRIST_RIGHT] = skeleton->skeletonPositions[CELL_WRIST_RIGHT];
    jointPositions[CELL_HAND_RIGHT] = skeleton->skeletonPositions[CELL_HAND_RIGHT];
    jointPositions[CELL_HIP_LEFT] = skeleton->skeletonPositions[CELL_HIP_LEFT];
    jointPositions[CELL_KNEE_LEFT] = skeleton->skeletonPositions[CELL_KNEE_LEFT];
    jointPositions[CELL_ANKLE_LEFT] = skeleton->skeletonPositions[CELL_ANKLE_LEFT];
    jointPositions[CELL_FOOT_LEFT] = skeleton->skeletonPositions[CELL_FOOT_LEFT];
    jointPositions[CELL_HIP_RIGHT] = skeleton->skeletonPositions[CELL_HIP_RIGHT];
    jointPositions[CELL_KNEE_RIGHT] = skeleton->skeletonPositions[CELL_KNEE_RIGHT];
    jointPositions[CELL_ANKLE_RIGHT] = skeleton->skeletonPositions[CELL_ANKLE_RIGHT];
    jointPositions[CELL_FOOT_RIGHT] = skeleton->skeletonPositions[CELL_FOOT_RIGHT];


    for (int i = 0; i < (int)jointPositions.size(); i++)
    {
        //jointPositions[i].z *= (clientID == 0 || clientID == 1) ? -1 : 1;
		/*
        //if (clientID == 0 || clientID == 1)
        jointPositions[i].z *= -1;
        jointPositions[i] *= userMan->skeletonScale[clientID];
        jointPositions[i].rotate(userMan->skeletonRotDegrees[clientID], ofVec3f(0.0f, 0.0f, 0.0f), ofVec3f(userMan->skeletonRotX[clientID], userMan->skeletonRotY[clientID], userMan->skeletonRotZ[clientID]));
        jointPositions[i].x += userMan->skeletonPosOffsetX[clientID];
        //if (clientID == 2 || clientID == 3) jointPositions[i].x *= -1;
        jointPositions[i].y += userMan->skeletonPosOffsetY[clientID];
        jointPositions[i].z += userMan->skeletonPosOffsetZ[clientID];
		*/
		
		// do some initial alterations to the skeleton data
		jointPositions[i].y *= -1.0f;
		jointPositions[i].z = (jointPositions[i].z * -1) * 0.0095;//skeletonZReductionMultiplier;

		//position and rotate the skeleton
		jointPositions[i] += ofVec3f(userMan->skeletonPosOffsetX[clientID], userMan->skeletonPosOffsetY[clientID], userMan->skeletonPosOffsetZ[clientID]);
		jointPositions[i].rotate(userMan->skeletonRotDegrees[clientID], ofVec3f(0.0, 0.0, 0.0), ofVec3f(userMan->skeletonRotX[clientID], userMan->skeletonRotY[clientID], userMan->skeletonRotZ[clientID]));

		//capture the hip vector then fix the z scaling bug
		if (i == 0) hipOffset = ofVec3f(jointPositions[CELL_HIP_CENTRE]);
		performZScaleFix(&jointPositions[i]);

		jointPositions[i] *= userMan->skeletonScale[clientID];

        /*

        200 - -200


        */
    }

    float xCorrectionOffset = ofMap(jointPositions[CELL_HIP_CENTRE].z, userMan->xCorrectionOffsetRangeMin, userMan->xCorrectionOffsetRangeMax, userMan->xCorrectionOffsetMin, userMan->xCorrectionOffsetMax);

    if (jointPositions[CELL_HIP_CENTRE].x < 0)
        xCorrectionOffset *= -1;

    xCorrectionOffset *= ofMap(abs(jointPositions[CELL_HIP_CENTRE].x), 0, userMan->xCorrectionOffsetModifier, 0, 1);

    for (int i = 0; i < (int)jointPositions.size(); i++)
    {
        jointPositions[i].x += xCorrectionOffset;
    }
}


void User::customDraw()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    if (app->isKinectAttached)
        debugDraw();
    else
        nonKinectDraw();

}



// This method fixes a bug in ofxKinectNui that results in the skeleton shrinking too much as it moves back.
void User::performZScaleFix(ofVec3f* skeletonPoint)
{
	// centre the skeleton in the scene using the centre of the hip then scale it up
	*skeletonPoint -= hipOffset;
	float scaler = ofMap(hipOffset.z, -200, 200, 0.35, 0.08);   //skeleton1ZScaleFixMax, skeleton1ZScaleFixMin);
	*skeletonPoint *= scaler;
	// reset the position of the skeleton using the 'hip offset'
	*skeletonPoint += hipOffset;
	// correct the x and z values after scaling
	skeletonPoint->x += hipOffset.x;
	skeletonPoint->z += hipOffset.z;
}





void User::nonKinectUpdate()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    UserManager* userMan = &app->sceneManager.userManager;

    if (!userMan->isNonKinectUserPaused)
    {
        for (int i = 0; i < 20; i++)
        {
            float mX = (userID % 2 == 0) ? ofGetMouseX() : -ofGetMouseX() + ofGetWidth();
            float mY = (userID % 2 == 0) ? ofGetMouseY() : -ofGetMouseY() + ofGetHeight();
//            float mX = ofGetMouseX() + (200 * userID);
//            float mY = ofGetMouseY() + (200 * userID);

            jointPositions[i].x = debugJointPositions[i].x + ((mX - (ofGetWidth() * 0.5)) * 0.1);
            jointPositions[i].z = debugJointPositions[i].z + ((mY - (ofGetHeight() * 0.5)) * 0.1);
        }
    }
}


void User::nonKinectDraw()
{
    drawLines();
}



void User::debugDraw()
{
    ofSetSphereResolution(4);

    //if (ofGetFrameNum() % 50 == 0) printf("drawing user %i \n", userID);

    TestApp* app = (TestApp*)ofGetAppPtr();
    UserManager* userMan = &app->sceneManager.userManager;

    for (int i = 0; i < (int)jointPositions.size(); i++)
    {
        ofVec3f vec = jointPositions[i];
        ofPushMatrix();
        ofTranslate(vec);
        ofPushStyle();

        if (userMan->isJointSpheres)
        {
            ofSetColor(debugColour);
            ofSphere(ofRandom(0.1, 0.4));     //0.4);
        }
        ofSetColor(255, 255, 255);
        ofScale(0.4, -0.4, 0.4);
        if (userMan->isPositionDataDisplayed)
        {
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            string str = "i:" + ofToString(i) + ", x:" + ofToString((vec.x)) + ", y:" + ofToString((vec.y)) + ", z:" + ofToString((vec.z));
            ofDrawBitmapString(str, 2, 0, 0);
        }
        if (userMan->isUserDataDisplayed)
        {
            if (i == CELL_HEAD)
            {
                ofTranslate(0, -3, 0);
                ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
                string str = "userID:" + ofToString(userID) + ", clientID:" + ofToString(clientID);
                ofDrawBitmapString(str, 2, 0, 0);

                ofTranslate(0, -3, 0);
                str = "trackingID:" + ofToString((int)skeleton->dwTrackingID) + ", " + demographic;
                ofDrawBitmapString(str, 2, 0, 0);
            }
        }
        ofPopStyle();
        ofPopMatrix();
        ofSetColor(255, 255, 255);
    }

    if (userMan->isJointLines)
    {
        drawLines();
    }
    if (userMan->isAverageLineDisplayed)
    {
        ofVec2f vec = getAveragePosition();
        drawLine(ofVec3f(vec.x, 0, vec.y), ofVec3f(vec.x, 50, vec.y));
    }

}



void User::drawSpheres()
{

}

void User::drawData()
{

}

void User::drawLines()
{
    drawLine(jointPositions[CELL_HEAD], jointPositions[CELL_SHOULDER_CENTRE]);
    drawLine(jointPositions[CELL_SHOULDER_CENTRE], jointPositions[CELL_SPINE]);
    drawLine(jointPositions[CELL_SPINE], jointPositions[CELL_HIP_CENTRE]);
    drawLine(jointPositions[CELL_SHOULDER_CENTRE], jointPositions[CELL_SHOULDER_LEFT]);
    drawLine(jointPositions[CELL_SHOULDER_LEFT], jointPositions[CELL_ELBOW_LEFT]);
    drawLine(jointPositions[CELL_ELBOW_LEFT], jointPositions[CELL_WRIST_LEFT]);
    drawLine(jointPositions[CELL_WRIST_LEFT], jointPositions[CELL_HAND_LEFT]);
    drawLine(jointPositions[CELL_SHOULDER_CENTRE], jointPositions[CELL_SHOULDER_RIGHT]);
    drawLine(jointPositions[CELL_SHOULDER_RIGHT], jointPositions[CELL_ELBOW_RIGHT]);
    drawLine(jointPositions[CELL_ELBOW_RIGHT], jointPositions[CELL_WRIST_RIGHT]);
    drawLine(jointPositions[CELL_WRIST_RIGHT], jointPositions[CELL_HAND_RIGHT]);
    drawLine(jointPositions[CELL_HIP_CENTRE], jointPositions[CELL_HIP_LEFT]);
    drawLine(jointPositions[CELL_HIP_LEFT], jointPositions[CELL_KNEE_LEFT]);
    drawLine(jointPositions[CELL_KNEE_LEFT], jointPositions[CELL_ANKLE_LEFT]);
    drawLine(jointPositions[CELL_ANKLE_LEFT], jointPositions[CELL_FOOT_LEFT]);
    drawLine(jointPositions[CELL_HIP_CENTRE], jointPositions[CELL_HIP_RIGHT]);
    drawLine(jointPositions[CELL_HIP_RIGHT], jointPositions[CELL_KNEE_RIGHT]);
    drawLine(jointPositions[CELL_KNEE_RIGHT], jointPositions[CELL_ANKLE_RIGHT]);
    drawLine(jointPositions[CELL_ANKLE_RIGHT], jointPositions[CELL_FOOT_RIGHT]);

}



void User::drawLine(ofVec3f startJoint, ofVec3f endJoint)
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    ofSetLineWidth(1);
    glBegin(GL_LINES);
        glVertex3f(startJoint.x, startJoint.y, startJoint.z);
        glVertex3f(endJoint.x, endJoint.y, endJoint.z);
    glEnd();
}




void User::assignSkeleton(KinectSkeletonData* _skeleton)
{
    skeleton = _skeleton;
    trackingID = skeleton->dwTrackingID;
    clientID = skeleton->clientID;

    //if (ofGetFrameNum() % 30 == 0)
     //   printf("User::assignSkeleton - clientID = %i, dwTrackingID = %f \n", clientID, trackingID);

    if (!isActive)
    {
        millisBecameActive = ofGetElapsedTimeMillis();
        secondsSinceActive = 0.0;
    }
    isActive = true;
}


void User::assignIDs(int _clientID, int _skeletonID)
{
    clientID = _clientID;

}



void User::deactivate()
{
	printf("User::deactivate() frame number = %i \n", ofGetFrameNum());
    skeleton = NULL;
    clientID = -1;
    trackingID = -1;
    isActive = false;
    //associatedUserBatch = NULL;
}


void User::buildDebugSkeleton()
{
    jointPositions.push_back(ofVec3f(2.944353, 7.110139, 26.315323));
    jointPositions.push_back(ofVec3f(3.169317, 7.971251, 25.564495));
    jointPositions.push_back(ofVec3f(3.201722, 12.975382, 25.583363));
    jointPositions.push_back(ofVec3f(2.727229, 15.908387, 26.246433));
    jointPositions.push_back(ofVec3f(0.365671, 11.499765, 24.789856));
    jointPositions.push_back(ofVec3f(-2.505701, 9.473697, 23.591682));
    jointPositions.push_back(ofVec3f(-6.268208, 10.110686, 24.198410));
    jointPositions.push_back(ofVec3f(-7.306726, 10.188894, 24.636562));
    jointPositions.push_back(ofVec3f(5.703905, 11.299618, 26.282230));
    jointPositions.push_back(ofVec3f(9.569173, 9.938474, 26.989307));
    jointPositions.push_back(ofVec3f(12.569213, 11.004990, 28.318794));
    jointPositions.push_back(ofVec3f(13.527909, 11.856798, 29.158825));
    jointPositions.push_back(ofVec3f(1.722497, 5.905774, 26.209633));
    jointPositions.push_back(ofVec3f(1.547234, -1.642440, 27.377213));
    jointPositions.push_back(ofVec3f(2.168538, -6.736201, 27.929012));
    jointPositions.push_back(ofVec3f(1.388633, -7.670440, 28.421452));
    jointPositions.push_back(ofVec3f(4.059348, 5.885036, 26.800846));
    jointPositions.push_back(ofVec3f(4.109383, -1.808313, 27.124569));
    jointPositions.push_back(ofVec3f(4.130900, -7.125211, 27.070328));
    jointPositions.push_back(ofVec3f(4.754252, -7.926762, 27.090336));

    for (int i = 0; i < 20; i++)
    {
        jointPositions[i].y += 7;
        jointPositions[i].z -= 25;
        debugJointPositions.push_back(ofVec3f(jointPositions[i].x, jointPositions[i].y, jointPositions[i].z));
    }
}

ofVec2f User::getAveragePosition()
{
    ofVec3f averageVec3 = (jointPositions[CELL_SHOULDER_CENTRE] + jointPositions[CELL_SPINE] + jointPositions[CELL_HIP_CENTRE]) / 3;
    return ofVec2f(averageVec3.x, averageVec3.z);
}
