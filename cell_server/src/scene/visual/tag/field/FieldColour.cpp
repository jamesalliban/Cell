#include "FieldColour.h"


void FieldColour::init()
{
    Field::init();
}


void FieldColour::update()
{
    if (!isEnabled) return;
    Field::update();
}


void FieldColour::draw()
{
    ofPushStyle();
    //glColor4f(colourOffset.x, colourOffset.y, colourOffset.z, 1.0f);
    ofSetColor(colourOffset.x * 255, colourOffset.y * 255, colourOffset.z * 255, 255);
    
    Field::draw();
    ofPopStyle();

}



void FieldColour::addFieldForce(ofVec3f* position)
{
}


float FieldColour::addFieldScale(ofVec3f* position)
{
    return 0.0;
}



ofVec3f FieldColour::getColourOffset(ofVec3f* position)
{
    if (!isEnabled || !isAllEnabled) return ofVec3f(0.0, 0.0, 0.0);

    Field::addFieldScale(position);

    if (lengthSquared < lengthSquaredMin)
    {
        float normDist = (-(lengthSquared / lengthSquaredMin) + 1);
		return ofVec3f((colours[0] - 1.0) * normDist, (colours[1] - 1.0) * normDist, (colours[2] - 1.0) * normDist);
    }
    else
    {
        return ofVec3f(0.0, 0.0, 0.0);
    }
}
