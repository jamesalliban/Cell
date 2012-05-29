#include "UserManager.h"

#include "TestApp.h"
#include "KinectManager.h"


///NEW IDEA
/*
If user is new
If it occupies the same space as another user from a different client
pass the user data to the new user and mute the old user


*/





UserManager::UserManager()
{
}

UserManager::~UserManager()
{
}



void UserManager::init()
{
    TestApp* app = (TestApp*)ofGetAppPtr();

    skeletonScale = {0.0, 0.0, 0.0, 0.0};
    skeletonRotDegrees = {0.0, 0.0, 0.0, 0.0};
    skeletonRotX = {0.0, 0.0, 0.0, 0.0};
    skeletonRotY = {0.0, 0.0, 0.0, 0.0};
    skeletonRotZ = {0.0, 0.0, 0.0, 0.0};
    skeletonPosOffsetX = {0, 0, 0, 0};
    skeletonPosOffsetY = {0, 0, 0, 0};
    skeletonPosOffsetZ = {0, 0, 0, 0};

    for (int i = 0; i < SKELETON_MAX; i++)
    {
        User* user = new User(i);
        users.push_back(user);
    }

    //userBatchManager.init();

    if (!app->isKinectAttached)
    {
        users[0]->isActive = true;
        users[0]->demographic = app->resourceManager.getRandomDemographic();
        users[1]->isActive = true;
        users[1]->demographic = app->resourceManager.getRandomDemographic();
    }
}



void UserManager::update()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    if (app->isKinectAttached)
    {
        if (app->kinectManager->hasSkeleton())
        {
            bool hasSkeletonBeenRemoved = haveSkeletonsBeenRemoved();
            bool hasSkeletonBeenAdded = checkIfSkeletonIsNew();
            if (hasSkeletonBeenRemoved || hasSkeletonBeenAdded) reassignSkeletonsIfNew();


            for (int i = 0; i < SKELETON_MAX; i++)
            {
                User* user = users[i];
                if (user->isActive)
                {
                    //if (ofGetFrameNum() % 60 == 0) user->demographic = app->resourceManager.getRandomDemographic();
                    user->update();
                }



//                if (ofGetFrameNum() % 90 == 0)
//                {
////                    printf("users[%i].trackingID                = %ld, hip.x = %f \n", i, users[i]->trackingID, users[i]->jointPositions[0].x);
////                    printf("trackedSkeletons[%i].dwTrackingID   = %ld, hip.x = %f \n", i, app->kinectManager->trackedSkeletons[i].dwTrackingID, app->kinectManager->trackedSkeletons[i].SkeletonPositions[0].x);
//                    //printf("users[%i].trackingID                = %ld, hip.x = %f \n", i, users[i]->trackingID, users[i]->jointPositions[0].x);
//                    //printf("trackedSkeletons[%i].dwTrackingID   = %ld, hip.x = %f \n", i, app->kinectManager->trackedSkeletons[i].dwTrackingID, app->kinectManager->trackedSkeletons[i].SkeletonPositions[0].x);
//                }
            }
//            if (ofGetFrameNum() % 90 == 0) printf("\n");
        }
        else
        {
            for (int i = 0; i < SKELETON_MAX; i++)
            {
                User* user = users[i];
                if (user->isActive) user->deactivate();
            }
        }

        //userBatchManager.update(&users);
    }
    else
    {
        for (int i = 0; i < SKELETON_MAX; i++)
        {
            User* user = users[i];
            user->nonKinectUpdate();
        }
    }

}


void UserManager::draw()
{
    TestApp* app = (TestApp*)ofGetAppPtr();
    if (app->isKinectAttached)
    {
        if (app->kinectManager->hasSkeleton())
        {
            for (int i = 0; i < SKELETON_MAX; i++)
            {
                User* user = users[i];
                if (user->isActive) user->customDraw();
            }
        }
        //userBatchManager.draw(&users);
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            User* user = users[i];

            user->nonKinectDraw();
        }
    }
}




bool UserManager::haveSkeletonsBeenRemoved()
{
    // First we need to see if any skeletons have been removed. If so, we deactivate the user
    //        for all users
    //            for all skeletons
    //                check the User ID aginst the skeleton. If it isnt there, deactivate

    TestApp* app = (TestApp*)ofGetAppPtr();
    bool hasOneOrMoreSkeletonsBeenRemoved = false;
    for (int i = 0; i < SKELETON_MAX; i++) // loop through users
    {
        User* user = users[i];
        bool doesUserValueMatchSkeleton = false;
        for (int j = 0; j < SKELETON_MAX; j++) // loop through skeletons
        {
            ofxMSKinectSkeleton* skeleton = &app->kinectManager->trackedSkeletons[j];

            if (user->trackingID == skeleton->dwTrackingID && skeleton->dwTrackingID != -1)
            {
                // this user is matched to an active skeleton
                doesUserValueMatchSkeleton = true;
            }
        }
        // if this user is currently active but the trackingID does not match an active skeleton...
        if (user->isActive && !doesUserValueMatchSkeleton)
        {
            hasOneOrMoreSkeletonsBeenRemoved = true;
            user->deactivate();
        }
    }
    return hasOneOrMoreSkeletonsBeenRemoved;
}




bool UserManager::checkIfSkeletonIsNew()
{
    // loop through all active skeletons and try to match these to a User trackingID. If there
    // is no match, the skeleton is new. Assign the skeleton to an inactive User.

    TestApp* app = (TestApp*)ofGetAppPtr();
    bool isSkeletonCountChanged = false;
    for (int i = 0; i < SKELETON_MAX; i++) // loop though all skeletons
    {
        ofxMSKinectSkeleton* skeleton = &app->kinectManager->trackedSkeletons[i];
        bool isSkeletonAssignedToAUser = false;
        // if skeleton is active
        if (skeleton->dwTrackingID != -1)
        {
            for (int j = 0; j < SKELETON_MAX; j++) // loop through all users
            {
                User* user = users[j];
                // check if skeleton is already assigned to a user or is blank. If so, move on to the next skeleton
                if (user->trackingID == skeleton->dwTrackingID)
                {
                    isSkeletonAssignedToAUser = true;
                }
            }
        }
        else
        {
            isSkeletonAssignedToAUser = true;
        }

        if (!isSkeletonAssignedToAUser)
        {
            // the skeleton is new as it has not been assigned to a User. Find an inactive User
            // and assign the skeleton to it
            isSkeletonCountChanged = true;
            for (int j = 0; j < SKELETON_MAX; j++)
            {
                User* user = users[j];
                if (!user->isActive)
                {
                    user->assignSkeleton(skeleton);
                    user->demographic = app->resourceManager.getRandomDemographic();
                    break;
                }
            }
        }
    }
    return isSkeletonCountChanged;
}



void UserManager::reassignSkeletonsIfNew()
{
    // it seems we have to reassign the skeletons to their respective users each time a new skeleton enters.
    // this is because the new skeletons are added to the front of the vector

    //return;

    TestApp* app = (TestApp*)ofGetAppPtr();


    for (int i = 0; i < SKELETON_MAX; i++) // loop through skeletons
    {
        ofxMSKinectSkeleton* skeleton = &app->kinectManager->trackedSkeletons[i];
        for (int j = 0; j < SKELETON_MAX; j++) // loop through users
        {
            User* user = users[j];
            if (user->trackingID == skeleton->dwTrackingID && skeleton->dwTrackingID != -1)
            {
                //printfprintf("2");
                user->assignSkeleton(skeleton);
            }
        }
    }
}



void UserManager::deactivateAllUsers()
{
    for (int i = 0; i < SKELETON_MAX; i++)
    {
        users[i]->deactivate();
    }
}



bool UserManager::isUserADuplicate(User* newUser)
{
    ofVec2f newUserAverage = newUser->getAveragePosition();
    for (int i = 0; i < (int)users.size(); i++)
    {
        User* existingUser = users[i];
        if (newUser->trackingID == existingUser->trackingID) continue;

        ofVec2f existingUserAverage = existingUser->getAveragePosition();
        ofVec2f length = newUserAverage - existingUserAverage;
        float lengthSquared = length.lengthSquared();
        if (lengthSquared < duplicateLengthSquaredMin) // && newUser->skeleton->clientID != existingUser->skeleton->clientID)
        {
            //printf("\n* DUPLICATE DETECTED!! Assign the same demographic %ld \n", newUser->trackingID, existingUser->trackingID);
            newUser->demographic = existingUser->demographic;
            return true;
        }
    }
    return false;
}



void UserManager::keyPressed(int key)
{
	switch(key)
	{
		case 'o': isNonKinectUserPaused = !isNonKinectUserPaused;
			break;

	}
}
