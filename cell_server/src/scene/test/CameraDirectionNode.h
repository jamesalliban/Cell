#ifndef CAMERADIRECTIONNODE_H
#define CAMERADIRECTIONNODE_H

#include "ofMain.h"

class CameraDirectionNode : public ofNode
{
    public:
        CameraDirectionNode();
        virtual ~CameraDirectionNode();

        void update();

        int xPos;
        int yPos;
        int zPos;
};

#endif // CAMERADIRECTIONNODE_H
