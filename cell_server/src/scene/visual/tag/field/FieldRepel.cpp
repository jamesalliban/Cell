#include "FieldRepel.h"


void FieldRepel::init()
{
    Field::init();
}


void FieldRepel::update()
{
    if (!isEnabled) return;
    Field::update();
}


void FieldRepel::draw()
{
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    Field::draw();
}



void FieldRepel::addFieldForce(ofVec3f* position)
{
    if (!isEnabled) return;
    Field::addFieldForce(position);

    if (lengthSquared < lengthSquaredMin)
    {
        float strength = -(lengthSquared / lengthSquaredMin) + 1;

        ofVec3f angle = (pos - fieldPos) * repelStrength;

        position->x += angle.x;
        position->z += angle.y;
    }
}




float FieldRepel::addFieldScale(ofVec3f* position)
{
    return 0.0;
}
