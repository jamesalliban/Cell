#ifndef FIELDREPEL_H
#define FIELDREPEL_H

#include "ofMain.h"
#include "Field.h"

class FieldRepel : public Field
{
public:
    
    virtual void init();
    virtual void update();
    virtual void draw();
    virtual void addFieldForce(ofVec3f* position);
    virtual float addFieldScale(ofVec3f* position);

    float repelStrength;

    bool isEnabled;

protected:
private:
};

#endif // FIELDREPEL_H
