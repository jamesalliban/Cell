#pragma once


#include "ofMain.h"
#include "UserBatch.h"
#include "User.h"

class UserBatchManager
{
    public:
        UserBatchManager();
        virtual ~UserBatchManager();
        void init();
        void update(vector<User*>* users);
        void draw(vector<User*>* users);
        bool areThereAnyActiveBatches();
        int isUserInBatch(User* user);


        vector<UserBatch*> userBatches;

        bool isAverageLineDisplayed;
        float lengthSquaredMin;

    protected:
    private:
};
