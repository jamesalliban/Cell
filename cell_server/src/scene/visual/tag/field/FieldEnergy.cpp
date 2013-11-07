#include "FieldEnergy.h"


void FieldEnergy::init()
{
    Field::init();
}

void FieldEnergy::update()
{
    if (!isEnabled) return;
    Field::update();
}



void FieldEnergy::draw()
{
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    Field::draw();
}



void FieldEnergy::addFieldForce(ofVec3f* position)
{
    if (!isEnabled) return;
    Field::addFieldForce(position);

    if (lengthSquared < lengthSquaredMin)
    {
        float noiseLevel = noiseMultiplier * (-(lengthSquared / lengthSquaredMin) + 1);

        float t = (ofGetFrameNum() * frameMultiplier) + (position->x + position->z);
        position->x += ofSignedNoise(t, 0, 0) * noiseLevel;
        position->y += ofSignedNoise(0, t, 0) * noiseLevel;
        position->z += ofSignedNoise(0, 0, t) * noiseLevel;
    }
}


float FieldEnergy::addFieldScale(ofVec3f* position)
{
    return 0.0;
}
