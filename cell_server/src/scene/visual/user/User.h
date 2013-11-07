#pragma once

#include "ofMain.h"
#include "KinectSkeletonData.h"

#define CELL_HIP_CENTRE 0
#define CELL_SPINE 1
#define CELL_SHOULDER_CENTRE 2
#define CELL_HEAD 3
#define CELL_SHOULDER_LEFT 4
#define CELL_ELBOW_LEFT 5
#define CELL_WRIST_LEFT 6
#define CELL_HAND_LEFT 7
#define CELL_SHOULDER_RIGHT 8
#define CELL_ELBOW_RIGHT 9
#define CELL_WRIST_RIGHT 10
#define CELL_HAND_RIGHT 11
#define CELL_HIP_LEFT 12
#define CELL_KNEE_LEFT 13
#define CELL_ANKLE_LEFT 14
#define CELL_FOOT_LEFT 15
#define CELL_HIP_RIGHT 16
#define CELL_KNEE_RIGHT 17
#define CELL_ANKLE_RIGHT 18
#define CELL_FOOT_RIGHT 19

struct Joint{
    string name;
    ofVec3f jointPos;
};

//class UserBatch;
class UserManager;

class User : public ofNode
{
public:

    void setup(UserManager *_parent, int _userID);
    void update();
    void customDraw();
    void nonKinectUpdate();
    void nonKinectDraw();
    void debugDraw();
    void performZScaleFix(ofVec3f* skeletonPoint);
    void drawSpheres();
    void drawData();
    void drawLines();
    void drawLine(ofVec3f startJoint, ofVec3f endJoint);
    void assignSkeleton(KinectSkeletonData* _skeleton);
    void assignIDs(int _clientID, int _skeletonID);
    void deactivate();
    void buildDebugSkeleton();
    ofVec2f getAveragePosition();


    UserManager *userMan;
    int userID;
    int clientID;

    KinectSkeletonData* skeleton;
    vector<ofVec3f> jointPositions;
    vector<ofVec3f> debugJointPositions;
    vector<User*> duplicateUsers;
    string demographic;

    ofColor debugColour;

    bool isActive;
    long trackingID;

    int millisBecameActive;
    float secondsSinceActive;

    ofVec3f hipOffset;

    //UserBatch* associatedUserBatch;


protected:
private:
};


//#include "UserBatch.h"
