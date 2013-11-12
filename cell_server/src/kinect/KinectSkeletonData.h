#pragma once

class KinectSkeletonData
{
public:
    //KinectSkeletonData();

    int clientID;
    string trackingID;
    vector<ofVec3f> skeletonPositions;
};
