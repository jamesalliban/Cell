#ifndef FIELDHEIGHTUP_H
#define FIELDHEIGHTUP_H

#include "ofMain.h"
#include "Field.h"


class FieldHeightUp : public Field
{
public:
    
    virtual void init();
    virtual void update();
    virtual void draw();
    virtual void addFieldForce(ofVec3f* position);
    virtual float addFieldScale(ofVec3f* position);

    float heightAdjust;

    bool isEnabled;

protected:
private:
};

#endif // FieldHeightUp_H
