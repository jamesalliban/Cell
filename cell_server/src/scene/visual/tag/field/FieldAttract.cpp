#include "FieldAttract.h"


void FieldAttract::init()
{
    Field::init();
}



void FieldAttract::update()
{
    Field::update();
}


void FieldAttract::draw()
{
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    Field::draw();
}



void FieldAttract::addFieldForce(ofVec3f* position)
{
//    Field::addFieldForce(position);
//
//    float lengthSquaredMin = 300;
//    if (lengthSquared < lengthSquaredMin)
//    {
//        float strength = -(lengthSquared / lengthSquaredMin) + 1;
//        position->y += ofRandomf() * strength;
//    }
}




float FieldAttract::addFieldScale(ofVec3f* position)
{
    return 0.0;
}
