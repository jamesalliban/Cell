#ifndef FIELDATTRACT_H
#define FIELDATTRACT_H

#include "ofMain.h"
#include "Field.h"

class FieldAttract : public Field
{
public:

    virtual void init();
    virtual void update();
    virtual void draw();
    virtual void addFieldForce(ofVec3f* position);
    virtual float addFieldScale(ofVec3f* position);

protected:
private:
};

#endif // FIELDATTRACT_H
