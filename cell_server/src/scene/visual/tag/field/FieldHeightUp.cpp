#include "FieldHeightUp.h"


void FieldHeightUp::init()
{
    Field::init();
}


void FieldHeightUp::update()
{
    if (!isEnabled) return;
    Field::update();
}


void FieldHeightUp::draw()
{
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    Field::draw();
}



void FieldHeightUp::addFieldForce(ofVec3f* position)
{
    if (!isEnabled) return;
    Field::addFieldForce(position);

    if (lengthSquared < lengthSquaredMin)
    {
        position->y += (-(lengthSquared / lengthSquaredMin) + 1) * heightAdjust;
    }
}




float FieldHeightUp::addFieldScale(ofVec3f* position)
{
    return 0.0;
}
