#ifndef FIELDENERGY_H
#define FIELDENERGY_H

#include "ofMain.h"
#include "Field.h"

class FieldEnergy : public Field
{
public:
    
    virtual void init();
    virtual void update();
    virtual void draw();
    virtual void addFieldForce(ofVec3f* position);
    virtual float addFieldScale(ofVec3f* position);

    //float energyLevel;
    float frameMultiplier;
    float noiseMultiplier;

    bool isEnabled;

protected:
private:
};

#endif // FIELDENERGY_H
