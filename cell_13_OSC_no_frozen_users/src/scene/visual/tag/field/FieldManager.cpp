#include "FieldManager.h"

FieldManager::FieldManager()
{
    //ctor
}

FieldManager::~FieldManager()
{
    //dtor
}


void FieldManager::init()
{
    fieldEnergy = new FieldEnergy();
    fields.push_back(fieldEnergy);

//    FieldAttract* fieldAttract = new FieldAttract();
//    fields.push_back(fieldAttract);
//
    fieldRepel = new FieldRepel();
    fields.push_back(fieldRepel);

    fieldScaleUp = new FieldScaleUp();
    fields.push_back(fieldScaleUp);

    fieldHeightDown = new FieldHeightDown();
    fields.push_back(fieldHeightDown);

    fieldHeightUp = new FieldHeightUp();
    fields.push_back(fieldHeightUp);

    fieldColour1 = new FieldColour();
    fieldColour1->colourOffset = {1.0, 0.0, 0.0};
    fields.push_back(fieldColour1);

    fieldColour2 = new FieldColour();
    fieldColour2->colourOffset = {0.0, 1.0, 0.0};
    fields.push_back(fieldColour2);

    fieldColour3 = new FieldColour();
    fieldColour3->colourOffset = {0.0, 0.0, 1.0};
    fields.push_back(fieldColour3);
}


void FieldManager::update()
{
    for (int i = 0; i < (int)fields.size(); i++)
    {
        fields[i]->update();
    }
}


void FieldManager::draw()
{
    if (!isFieldLineVisible) return;

    for (int i = 0; i < (int)fields.size(); i++)
    {
        fields[i]->draw();
    }
}


void FieldManager::addFieldForce(ofVec3f* position)
{
    for (int i = 0; i < (int)fields.size(); i++)
    {
        fields[i]->addFieldForce(position);
    }
}


float FieldManager::addFieldScale(ofVec3f* position)
{
    float fieldScale = 0.0;
    for (int i = 0; i < (int)fields.size(); i++)
    {
        fieldScale += fields[i]->addFieldScale(position);
    }
    return fieldScale;
}



ofVec3f FieldManager::getColourOffset(ofVec3f* position)
{
    return (fieldColour1->getColourOffset(position) + fieldColour2->getColourOffset(position) + fieldColour3->getColourOffset(position)) / 3;
}
