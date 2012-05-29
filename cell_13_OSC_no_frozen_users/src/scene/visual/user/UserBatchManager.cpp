#include "UserBatchManager.h"



UserBatchManager::UserBatchManager()
{
    //ctor
}

UserBatchManager::~UserBatchManager()
{
    //dtor
}


void UserBatchManager::init()
{
    for (int i = 0; i < 4; i++)
    {
        UserBatch* userBatch = new UserBatch(i);
        userBatches.push_back(userBatch);
    }
}



void UserBatchManager::update(vector<User*>* users)
{
    // calculate distance between all user averages

    // if user is close to another, search all active batches to see if they are already in a batch. If they
    // are, continue to use that batch. If they are in separate batches, combine those batches

    // figure out which user is oldest. This is the Dominant User demographic and user

    // add an isMutedDuplicate boolean to users. When this is true it is not drawn and does not attract tags

    // alter the data of the dominant user if required

    // figure out which user is most suitable by the area in which the user stands. E.g. if batch is in front
    // left, use client 1 skel if there is one

    // add a pointer to each user that refers to the UserBatch it is associated with. If this pointer is non-NULL
    // tags will attach to userData.user.userBatch.dominantUser

    // if the dominant user disappears, the next oldest User is then selected.


    ///NEED A DOMINANT SKELETON AND A DOMINANT USER SEPARATELY
    ///The skeleton is the closest to a kinect camera. The User data is the oldest data for the batch
    ///User data. This is the oldest


    // calculate distance between all user averages
    for (int i = 0; i < (int)users->size(); i++)
    {
        User* user1 = (*users)[i];
        ofVec2f average1 = user1->getAveragePosition();

        for (int j = i + 1; j < (int)users->size(); j++)
        {
            User* user2 = (*users)[j];

            // if either of the users is inactive, continue
            if (!user1->isActive || !user2->isActive) continue;

            // check to see if user is close to another
            ofVec2f average2 = user2->getAveragePosition();
            ofVec2f length = average1 - average2;
            float lengthSquared = length.lengthSquared();
            if (lengthSquared < lengthSquaredMin)
            {
                bool isUserInBatch = false;

//                if (ofGetFrameNum() % 50 == 0)
//                    printf("user %i is close to user %i, lengthSquared = %f \n", user1->userID, user2->userID, lengthSquared);

                // if there are no active batches, add these users to userBatches[0] and activate it
                if (!areThereAnyActiveBatches())
                {
                    userBatches[0]->addUser(user1);
                    userBatches[0]->addUser(user2);
                    userBatches[0]->isActive = true;
                }
                else
                {
                    // search all active batches to see if they are already in a batch.
//                    int isUser1InABatch = isUserInBatch(user1);
//                    int isUser2InABatch = isUserInBatch(user2);

                    // if both users are in a batch
//                    if (isUser1InABatch != -1 && isUser2InABatch != -1)
//                    {
//                        // if the batch index is the same - do nothing
//                        if (isUser1InABatch == isUser2InABatch) continue;
//
//                        // if the batches are different - figure out which batch is oldest and place all users
//                        // from new batch into the older one
//                        if ()
//
//                    }

                    // if one user is in a batch and the other is not, place the non-batched user into the batched user's batch ?!?
//                    if ((isUser1InABatch == -1 && isUser2InABatch != -1) || (isUser1InABatch != -1 && isUser2InABatch == -1))
//                    {
//
//                    }
                }



            }
        }
    }


    // loop through all batches to see if users have separated
//    for (int i = 0; i < (int)users->size(); i++)
//    {
//        if (batches[i])
//    }
}



void UserBatchManager::draw(vector<User*>* users)
{
    for (int i = 0; i < (int)userBatches.size(); i++)
    {
        userBatches[i]->draw();
    }
}



bool UserBatchManager::areThereAnyActiveBatches()
{
    for (int i = 0; i < (int)userBatches.size(); i++)
        if (userBatches[i]->isActive) return true;

    return false;
}


int UserBatchManager::isUserInBatch(User* user)
{
//    for (int i = 0; i < (int)userBatches.size(); i++)
//    {
//        if (userBatches[i]->isActive)
//            if (userBatches[i]->checkIfUserIsInBatch(user)) return i;
//    }
    return -1;
}
