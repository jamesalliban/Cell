#include "UserManager.h"

#include "TestApp.h"
#include "KinectManager.h"
#include "ResourceManager.h"


///NEW IDEA
/*
If user is new
If it occupies the same space as another user from a different client
pass the user data to the new user and mute the old user


*/


float UserManager::xSpreadRangeNormalMin[SKELETON_MAX / 2];
float UserManager::xSpreadRangeNormalMax[SKELETON_MAX / 2];
float UserManager::xFrontSkewedMin[SKELETON_MAX / 2];
float UserManager::xFrontSkewedMax[SKELETON_MAX / 2];
float UserManager::xBackSkewedMin[SKELETON_MAX / 2];
float UserManager::xBackSkewedMax[SKELETON_MAX / 2];

void UserManager::init(KinectManager *_kinectManager, ResourceManager *_resourceManager)
{
    kinectManager = _kinectManager;
    resourceManager = _resourceManager;
    
    
//    skeletonScale[0] = skeletonScale[1] = skeletonScale[2] = skeletonScale[3] = 0.0;
//    skeletonRotDegrees[0] = skeletonRotDegrees[1] = skeletonRotDegrees[2] = skeletonRotDegrees[3] = 0.0;
//    skeletonRotX[0] = skeletonRotX[1] = skeletonRotX[2] = skeletonRotX[3] = 0.0;
//    skeletonRotY[0] = skeletonRotY[1] = skeletonRotY[2] = skeletonRotY[3] = 0.0;
//    skeletonRotZ[0] = skeletonRotZ[1] = skeletonRotZ[2] = skeletonRotZ[3] = 0.0;
//    skeletonPosOffsetX[0] = skeletonPosOffsetX[1] = skeletonPosOffsetX[2] = skeletonPosOffsetX[3] = 0;
//    skeletonPosOffsetY[0] = skeletonPosOffsetY[1] = skeletonPosOffsetY[2] = skeletonPosOffsetY[3] = 0;
//    skeletonPosOffsetZ[0] = skeletonPosOffsetZ[1] = skeletonPosOffsetZ[2] = skeletonPosOffsetZ[3] = 0;
    
    for (int i = 0; i < SKELETON_MAX; i++)
    {
        User user;
		user.setup(this, i);
		KinectSkeletonData* skeleton = &kinectManager->trackedSkeletons[i];  //  &app->kinectManager->trackedSkeletons[i];
		user.skeleton = skeleton;
        users.push_back(user);
    }

    //userBatchManager.init();

    if (!testApp::isKinectAttached)
    {
        users[0].isActive = true;
        users[0].demographic = resourceManager->getRandomDemographic();
        users[1].isActive = true;
        users[1].demographic = resourceManager->getRandomDemographic();
    }
}



void UserManager::update()
{
    if (testApp::isKinectAttached)
    {
        if (kinectManager->hasSkeleton())
        {
            bool hasSkeletonBeenAdded = checkIfSkeletonIsNew();


            for (int i = 0; i < SKELETON_MAX; i++)
            {
                User* user = &users[i];
                KinectSkeletonData* skeleton = &kinectManager->trackedSkeletons[i];
                
                if (skeleton->trackingID == "-1") user->isActive = false;

                if (user->isActive)
                {
                    user->update();
                }
            }
        }
        else
        {
            for (int i = 0; i < SKELETON_MAX; i++)
            {
                User* user = &users[i];
                if (user->isActive) user->deactivate();
            }
        }

        //userBatchManager.update(&users);
    }
    else
    {
        for (int i = 0; i < SKELETON_MAX; i++)
        {
            User* user = &users[i];
            user->nonKinectUpdate();
        }
    }

}


void UserManager::draw()
{
    if (testApp::isKinectAttached)
    {
        if (kinectManager->hasSkeleton())
        {
            for (int i = 0; i < SKELETON_MAX; i++)
            {
                User* user = &users[i];
                if (user->isActive) user->customDraw();
            }
        }
        //userBatchManager.draw(&users);
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            User* user = &users[i];

            user->nonKinectDraw();
        }
    }

	drawUserBounds();
}


void UserManager::drawUserBounds()
{
	int clientId = 0;
	int skel0FromClient = (int)(clientId / 2);

	ofPushMatrix();
	ofSetLineWidth(5);
	ofSetColor(100, 0, 0);
    // draw normal range
	ofPushMatrix();
	ofTranslate(skeletonPosOffsetX[clientId] * skeletonScale[clientId], 0, skeletonPosOffsetY[clientId] * skeletonScale[clientId]);
	ofLine(xSpreadRangeNormalMin[clientId], 0, User::zSpreadOutputMin, xSpreadRangeNormalMax[clientId], 0, User::zSpreadOutputMin); // back
	ofSetColor(255, 0, 0);
	ofLine(xSpreadRangeNormalMin[clientId], 0, User::zSpreadOutputMax, xSpreadRangeNormalMax[clientId], 0, User::zSpreadOutputMax); // front
	ofLine(xSpreadRangeNormalMin[clientId], 0, User::zSpreadOutputMin, xSpreadRangeNormalMin[clientId], 0, User::zSpreadOutputMax); // left
	ofLine(xSpreadRangeNormalMax[clientId], 0, User::zSpreadOutputMin, xSpreadRangeNormalMax[clientId], 0, User::zSpreadOutputMax); // right
	ofPopMatrix();

	ofSetLineWidth(3);
	ofSetColor(0, 100, 0);
	// draw skewed range
	ofLine(xFrontSkewedMin[clientId], 0, User::zSpreadOutputMin, xFrontSkewedMax[clientId], 0, User::zSpreadOutputMin); // back
	ofSetColor(0, 255, 0);
	ofLine(xBackSkewedMin[clientId], 0, User::zSpreadOutputMax, xBackSkewedMax[clientId], 0, User::zSpreadOutputMax); // front
	ofLine(xFrontSkewedMin[clientId], 0, User::zSpreadOutputMin, xBackSkewedMin[clientId], 0, User::zSpreadOutputMax); // left
	ofLine(xFrontSkewedMax[clientId], 0, User::zSpreadOutputMin, xBackSkewedMax[clientId], 0, User::zSpreadOutputMax); // right

	
	ofPopMatrix();
}



bool UserManager::checkIfSkeletonIsNew()
{
    bool isThereANewSkel = false;
    for (int i = 0; i < SKELETON_MAX; i++) // loop though all skeletons
    {
		// if the user connected to skelton is inActive and the skeleton is active, assign skeleton
		KinectSkeletonData* skeleton = &kinectManager->trackedSkeletons[i];  //  &app->kinectManager->trackedSkeletons[i];
		User* user = &users[i];
		if (skeleton->skeletonPositions[0].x != -1 &&  !user->isActive)
		{
			isThereANewSkel = true;
			user->assignSkeleton(skeleton);
			user->demographic = resourceManager->getRandomDemographic();
		}
	}

	return isThereANewSkel; 
}




void UserManager::deactivateAllUsers()
{
    for (int i = 0; i < SKELETON_MAX; i++)
    {
        User* user = &users[i];
        user->deactivate();
    }
}



bool UserManager::isUserADuplicate(User* newUser)
{
    ofVec2f newUserAverage = newUser->getAveragePosition();
    for (int i = 0; i < (int)users.size(); i++)
    {
        User* existingUser = &users[i];
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

