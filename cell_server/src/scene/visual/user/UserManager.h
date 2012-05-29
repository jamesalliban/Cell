#ifndef USERMANAGER_H
#define USERMANAGER_H

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
        int skeletonPosOffsetX[4];
        int skeletonPosOffsetY[4];
        int skeletonPosOffsetZ[4];

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

#endif // USERMANAGER_H
