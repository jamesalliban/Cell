#pragma once


#include "ofMain.h"

class Field : public ofNode
{
public:
    virtual void init();
    virtual void update();
    virtual void draw();
    virtual void addFieldForce(ofVec3f* position);
    virtual float addFieldScale(ofVec3f* position);

    float randomity;

    ofVec2f fieldPos;
    ofVec2f length;
    ofVec2f pos;
    float lengthSquared;
    float lengthSquaredMin;


    int playArea;
    float movementSpeed;


protected:
    void calculateLengthSquared(ofVec3f* position);
    float calculateBellCurve();

private:
};

