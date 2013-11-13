#pragma once

class KinectSkeletonData
{
public:
    //KinectSkeletonData();
	
    int clientID;
    int skelID;
    string trackingID;
    vector<ofVec3f> skeletonPositions;
};
