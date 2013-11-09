#include "FieldHeightDown.h"


void FieldHeightDown::init()
{
    Field::init();
}


void FieldHeightDown::update()
{
    if (!isEnabled) return;
    Field::update();
}


void FieldHeightDown::draw()
{
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    Field::draw();
}



void FieldHeightDown::addFieldForce(ofVec3f* position)
{
    if (!isEnabled || !isAllEnabled) return;
    Field::addFieldForce(position);

    if (lengthSquared < lengthSquaredMin)
    {
        position->y -= (-(lengthSquared / lengthSquaredMin) + 1) * heightAdjust;
    }
}




float FieldHeightDown::addFieldScale(ofVec3f* position)
{
    return 0.0;
}
