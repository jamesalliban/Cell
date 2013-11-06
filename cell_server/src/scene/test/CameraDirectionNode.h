#pragma once

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