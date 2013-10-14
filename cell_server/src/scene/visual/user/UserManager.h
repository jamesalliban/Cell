#pragma once


#include "ofMain.h"
#include "User.h"
//#include "UserBatchManager.h"

#define CLIENT_AMOUNT 0

class UserManager
{
    public:
        UserManager();
        virtual ~UserManager();

        void init();
        void update();
        void draw();
        bool haveSkeletonsBeenRemoved();
        bool checkIfSkeletonIsNew();
        void reassignSkeletonsIfNew();
        void deactivateAllUsers();
        bool isUserADuplicate(User* newUser);

        vector<User*> users;
        //UserBatchManager userBatchManager;


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
