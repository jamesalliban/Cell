#include "User.h"
#include "TestApp.h"
#include "UserManager.h"

User::User(int _userID)
{
    userID              = _userID;
    clientID            = -1;

    deactivate();

    debugColour = {255, 255, 255};

    switch(userID)
    {
        case 0: debugColour = {255, 0, 0}; break;       // red
        case 1: debugColour = {0, 255, 0}; break;       // green
        case 2: debugColour = {80, 80, 255}; break;       // blue
        case 3: debugColour = {255, 255, 0}; break;     // yellow
        case 4: debugColour = {255, 0, 255}; break;     // pink
        case 5: debugColour = {0, 255, 255}; break;     // turquoise
        case 6: debugColour = {255, 255, 255}; break;   // white
        case 7: debugColour = {100, 100, 100}; break;   // grey
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

User::~User()
{
    //dtor
}


void User::update()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    UserManager* userMan = &app->sceneManager.userManager;

    if (!isActive) return;


    secondsSinceActive = ((float)ofGetElapsedTimeMillis() - (float)millisBecameActive) / 1000;

    jointPositions[CELL_HIP_CENTRE] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
    jointPositions[CELL_SPINE] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_SPINE];
    jointPositions[CELL_SHOULDER_CENTRE] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER];
    jointPositions[CELL_HEAD] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HEAD];
    jointPositions[CELL_SHOULDER_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT];
    jointPositions[CELL_ELBOW_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT];
    jointPositions[CELL_WRIST_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
    jointPositions[CELL_HAND_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];
    jointPositions[CELL_SHOULDER_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT];
    jointPositions[CELL_ELBOW_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT];
    jointPositions[CELL_WRIST_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
    jointPositions[CELL_HAND_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
    jointPositions[CELL_HIP_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT];
    jointPositions[CELL_KNEE_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT];
    jointPositions[CELL_ANKLE_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT];
    jointPositions[CELL_FOOT_LEFT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT];
    jointPositions[CELL_HIP_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT];
    jointPositions[CELL_KNEE_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT];
    jointPositions[CELL_ANKLE_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT];
    jointPositions[CELL_FOOT_RIGHT] = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT];

    for (int i = 0; i < (int)jointPositions.size(); i++)
    {
        //jointPositions[i].z *= (clientID == 0 || clientID == 1) ? -1 : 1;

        if (clientID == 0 || clientID == 1) jointPositions[i].z *= -1;
        jointPositions[i] *= userMan->skeletonScale[clientID];
        jointPositions[i].rotate(userMan->skeletonRotDegrees[clientID], ofVec3f(0.0f, 0.0f, 0.0f), ofVec3f(userMan->skeletonRotX[clientID], userMan->skeletonRotY[clientID], userMan->skeletonRotZ[clientID]));
        jointPositions[i].x += userMan->skeletonPosOffsetX[clientID];
        if (clientID == 2 || clientID == 3) jointPositions[i].x *= -1;
        jointPositions[i].y += userMan->skeletonPosOffsetY[clientID];
        jointPositions[i].z += userMan->skeletonPosOffsetZ[clientID];
    }
}


void User::customDraw()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    if (app->isKinectAttached)
        debugDraw();
    else
        nonKinectDraw();
    //nonKinectDraw();
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
            string str = "x:" + ofToString((int)(vec.x)) + ", y:" + ofToString((int)(vec.y)) + ", z:" + ofToString((int)(vec.z));
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




void User::assignSkeleton(_ofMS_SKELETON_DATA* _skeleton)
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
    skeleton = NULL;
    clientID = -1;
    trackingID = -1;
    isActive = false;
    associatedUserBatch = NULL;
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
