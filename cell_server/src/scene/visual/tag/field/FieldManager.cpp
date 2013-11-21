#include "FieldManager.h"


void FieldManager::init()
{
    fieldEnergy.init();
    fields.push_back(&fieldEnergy);

//    FieldAttract* fieldAttract = new FieldAttract();
//    fields.push_back(fieldAttract);
//
    fieldRepel.init();
    fieldRepel.type = "Repel";
    fields.push_back(&fieldRepel);

    fieldScaleUp.init();
    fieldScaleUp.type = "Scale up";
    fields.push_back(&fieldScaleUp);

    fieldHeightDown.init();
    fieldHeightDown.type = "Height down";
    fields.push_back(&fieldHeightDown);

    fieldHeightUp.init();
    fieldHeightUp.type = "Height up";
    fields.push_back(&fieldHeightUp);

    fieldColour1.init();
    fieldColour1.type = "Colour 1";
    fieldColour1.colourOffset[0] = 1.0;
    fieldColour1.colourOffset[1] = 0.0;
    fieldColour1.colourOffset[2] = 0.0;
    fields.push_back(&fieldColour1);

    fieldColour2.init();
    fieldColour2.type = "Colour 2";
    fieldColour2.colourOffset[0] = 0.0;
    fieldColour2.colourOffset[1] = 1.0;
    fieldColour2.colourOffset[2] = 0.0;
    fields.push_back(&fieldColour2);

    fieldColour3.init();
    fieldColour3.type = "Colour 3";
    fieldColour3.colourOffset[0] = 0.0;
    fieldColour3.colourOffset[1] = 0.0;
    fieldColour3.colourOffset[2] = 1.0;
    fields.push_back(&fieldColour3);
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
    return (fieldColour1.getColourOffset(position) + fieldColour2.getColourOffset(position) + fieldColour3.getColourOffset(position)) / 3;
}
