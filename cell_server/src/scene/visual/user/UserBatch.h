#pragma once


#include "ofMain.h"

class User;

class UserBatch : public ofNode
{
    public:
        UserBatch(int id);
        virtual ~UserBatch();

        void update();
        void draw();
        void addUser(User* user);
        ofVec2f getAverageBatchVec();

        int userBatchID;

        vector<User*> users;

        bool isActive;

        float debugSphereY;


    protected:
    private:
};

#include "User.h"
