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
    void drawUserBounds();
    bool checkIfSkeletonIsNew();
    void deactivateAllUsers();
    bool isUserADuplicate(User* newUser);

    vector<User> users;
    //UserBatchManager userBatchManager;
    
    KinectManager *kinectManager;
    ResourceManager *resourceManager;
    
    
    float skeletonScale[SKELETON_MAX / 2];
    float skeletonRotDegrees[SKELETON_MAX / 2];
    float skeletonRotX[SKELETON_MAX / 2];
    float skeletonRotY[SKELETON_MAX / 2];
    float skeletonRotZ[SKELETON_MAX / 2];
    float skeletonPosOffsetX[SKELETON_MAX / 2];
    float skeletonPosOffsetY[SKELETON_MAX / 2];
    float skeletonPosOffsetZ[SKELETON_MAX / 2];
    
    static float xSpreadRangeNormalMin[SKELETON_MAX / 2];
    static float xSpreadRangeNormalMax[SKELETON_MAX / 2];
    static float xFrontSkewedMin[SKELETON_MAX / 2];
    static float xFrontSkewedMax[SKELETON_MAX / 2];
    static float xBackSkewedMin[SKELETON_MAX / 2];
    static float xBackSkewedMax[SKELETON_MAX / 2];
    
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

protected:
private:


};
