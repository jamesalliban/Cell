#ifndef FIELDSCALEUP_H
#define FIELDSCALEUP_H

#include "ofMain.h"
#include "Field.h"


class FieldScaleUp : public Field
{
    public:
        FieldScaleUp();
        virtual ~FieldScaleUp();

        virtual void update();
        virtual void draw();
        virtual void addFieldForce(ofVec3f* position);

        virtual float addFieldScale(ofVec3f* position);

        float scaleUpStrength;

        bool isEnabled;

    protected:
    private:
};

#endif // FIELDSCALEUP_H
