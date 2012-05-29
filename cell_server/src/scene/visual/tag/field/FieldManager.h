#ifndef FIELDMANAGER_H
#define FIELDMANAGER_H

#include "ofMain.h"
#include "Field.h"
#include "FieldEnergy.h"
#include "FieldAttract.h"
#include "FieldRepel.h"
#include "FieldScaleUp.h"
#include "FieldHeightDown.h"
#include "FieldHeightUp.h"
#include "FieldColour.h"

class FieldManager
{
    public:
        FieldManager();
        virtual ~FieldManager();

        void init();
        void update();
        void draw();
        void addFieldForce(ofVec3f* position);
        float addFieldScale(ofVec3f* position);
        ofVec3f getColourOffset(ofVec3f* position);

        vector<Field*> fields;
        FieldEnergy* fieldEnergy;
        FieldRepel* fieldRepel;
        FieldScaleUp* fieldScaleUp;
        FieldHeightDown* fieldHeightDown;
        FieldHeightUp* fieldHeightUp;

        FieldColour* fieldColour1;
        FieldColour* fieldColour2;
        FieldColour* fieldColour3;

        bool isFieldLineVisible;

    protected:
    private:
};

#endif // FIELDMANAGER_H
