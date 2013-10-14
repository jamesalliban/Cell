#pragma once

class KinectSkeletonData
{
public:
    //KinectSkeletonData();

    int clientID;
    long dwTrackingID;
    vector<ofVec3f> skeletonPositions;
};
