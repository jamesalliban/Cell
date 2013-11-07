#include "Field.h"

void Field::init()
{
    randomity = ofRandom(10000);
    playArea = 50;
    movementSpeed = .05;
    lengthSquaredMin = 300;
}


void Field::update()
{
    float t = (ofGetElapsedTimef() + randomity) * movementSpeed;

    fieldPos.x = ofSignedNoise(t, 0) * playArea;
    fieldPos.y = ofSignedNoise(0, t) * playArea;
}


void Field::draw()
{
    ofSetLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(fieldPos.x, 0, fieldPos.y);
        glVertex3f(fieldPos.x, 50, fieldPos.y);
    glEnd();
}


void Field::addFieldForce(ofVec3f* position)
{
    calculateLengthSquared(position);
}


float Field::addFieldScale(ofVec3f* position)
{
    calculateLengthSquared(position);
	return 0.0;
}


void Field::calculateLengthSquared(ofVec3f* position)
{
    pos = ofVec2f(position->x, position->z);
    length = fieldPos - pos;
    lengthSquared = length.lengthSquared();
}


float Field::calculateBellCurve()
{
    float angle = ofMap(lengthSquared, 0, lengthSquaredMin, HALF_PI, -HALF_PI);
    float normCurve = (sin(angle) + 1) * 0.5;
    float normCurveExt = pow(normCurve, ofMap(normCurve, 0, 1, 1, 2));
    return normCurveExt;
}
