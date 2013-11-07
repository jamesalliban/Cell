#pragma once


#include "ofMain.h"
#include "User.h"
//#include "UserBatchManager.h"
#include "CellGlobals.h"

class KinectManager;
class ResourceManager;

#define CLIENT_AMOUNT 0

class UserManager
{
public:
    void init(KinectManager *_kinectManager, ResourceManager *_resourceManager);
    void update();
    void draw();
    bool haveSkeletonsBeenRemoved();
    bool checkIfSkeletonIsNew();
    void reassignSkeletonsIfNew();
    void deactivateAllUsers();
    bool isUserADuplicate(User* newUser);

    vector<User> users;
    //UserBatchManager userBatchManager;
    
    KinectManager *kinectManager;
    ResourceManager *resourceManager;


    float skeletonScale[4];
    float skeletonRotDegrees[4];
    float skeletonRotX[4];
    float skeletonRotY[4];
    float skeletonRotZ[4];
    float skeletonPosOffsetX[4];
    float skeletonPosOffsetY[4];
    float skeletonPosOffsetZ[4];


    float xCorrectionOffsetRangeMin;
    float xCorrectionOffsetRangeMax;
    float xCorrectionOffsetMin;
    float xCorrectionOffsetMax;
    float xCorrectionOffsetModifier;

    float duplicateLengthSquaredMin;

    bool isJointSpheres;
    bool isJointLines;
    bool isPositionDataDisplayed;
    bool isUserDataDisplayed;
    bool isAverageLineDisplayed;
    bool isNonKinectUserPaused;
    void keyPressed(int key);

protected:
private:


};
