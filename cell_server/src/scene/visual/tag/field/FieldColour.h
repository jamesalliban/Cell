#ifndef FIELDCOLOUR_H
#define FIELDCOLOUR_H

#include "ofMain.h"
#include "Field.h"


class FieldColour : public Field
{
    public:
        FieldColour();
        virtual ~FieldColour();

        virtual void update();
        virtual void draw();
        virtual void addFieldForce(ofVec3f* position);

        virtual float addFieldScale(ofVec3f* position);

        float	colours[4];

        ofVec3f getColourOffset(ofVec3f* position);

        float scaleUpStrength;

        bool isEnabled;

        ofVec3f colourOffset;

    protected:
    private:
};

#endif // FIELDCOLOUR_H
