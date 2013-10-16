#include "FieldColour.h"

FieldColour::FieldColour()
{
    //ctor
    //colourOffset = {1.0, 0.0, 0.0};
}

FieldColour::~FieldColour()
{
    //dtor
}


void FieldColour::update()
{
    if (!isEnabled) return;
    Field::update();
}


void FieldColour::draw()
{
    glColor4f(colourOffset.x, colourOffset.y, colourOffset.z, 1.0f);

    Field::draw();

    ofPushMatrix();
    ofTranslate(1, 0, 0);
    Field::draw();
    ofPopMatrix();
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
    if (!isEnabled) return ofVec3f(0.0, 0.0, 0.0);

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
