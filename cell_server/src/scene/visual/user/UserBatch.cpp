#include "UserBatch.h"

UserBatch::UserBatch(int id)
{
    userBatchID = id;
    isActive = false;
    debugSphereY = ofRandom(-2, -8);
}

UserBatch::~UserBatch()
{
    //dtor
}



void UserBatch::update()
{
    //setDominantUser();
}



void UserBatch::draw()
{
    if (isActive)
    {
        ofVec2f averageBatchVec = getAverageBatchVec();
        ofSetColor(255, 0, 0, 255);
        ofDrawSphere(averageBatchVec.x, 0, averageBatchVec.y, 2);
    }
}



void UserBatch::addUser(User* user)
{
    users.push_back(user);

    if (ofGetFrameNum() % 50 == 0)
        printf("addUser - - - (int)users.size() = %i", (int)users.size());
}




//void UserBatch::removeUser(User* user)
//{
//    // vector.erase(2);
//}


//void UserBatch::setDominantUser()
//{
//    // The dominant user is the one that is clostest to a kinect camera
//    //
//
//
//    //        loop through all users to see which is in
//    for (int i = 0; i < (int)users.size(); i++)
//    {
//
//    }
//}



ofVec2f UserBatch::getAverageBatchVec()
{
    ofVec2f total = ofVec2f(0.0, 0.0);


    for (int i = 0; i < (int)users.size(); i++)
    {
        total += users[i]->getAveragePosition();
    }
    return total / (int)users.size();
}


