#ifndef FIELDHEIGHTDOWN_H
#define FIELDHEIGHTDOWN_H

#include "ofMain.h"
#include "Field.h"


class FieldHeightDown : public Field
{
    public:
        FieldHeightDown();
        virtual ~FieldHeightDown();

        virtual void update();
        virtual void draw();
        virtual void addFieldForce(ofVec3f* position);
        virtual float addFieldScale(ofVec3f* position);

        float heightAdjust;

        bool isEnabled;

    protected:
    private:
};

#endif // FIELDHEIGHTDOWN_H
