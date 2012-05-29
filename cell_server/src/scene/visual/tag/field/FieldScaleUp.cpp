#include "FieldScaleUp.h"

FieldScaleUp::FieldScaleUp()
{
    //ctor
}

FieldScaleUp::~FieldScaleUp()
{
    //dtor
}


void FieldScaleUp::update()
{
    if (!isEnabled) return;
    Field::update();
}


void FieldScaleUp::draw()
{
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    Field::draw();
}



void FieldScaleUp::addFieldForce(ofVec3f* position)
{
}


float FieldScaleUp::addFieldScale(ofVec3f* position)
{
    if (!isEnabled) return 0.0;
    Field::addFieldScale(position);

    if (lengthSquared < lengthSquaredMin)
    {

        return calculateBellCurve() * scaleUpStrength;
        //return (-(lengthSquared / lengthSquaredMin) + 1) * scaleUpStrength;
    }
    else
    {
        return 0.0;
    }

}
