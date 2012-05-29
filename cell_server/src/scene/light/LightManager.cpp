/*
 *  LightManager.cpp
 *  cell
 *
 *  Created by James Alliban on 16/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#include "LightManager.h"

LightManager::LightManager()
{
    //light.setPointLight();
    //light.setDirectional();

    //light.lookAt(
}




LightManager::~LightManager()
{

}


void LightManager::update()
{
//	light.setPosition(ofGetWidth()*.5, ofGetHeight()*.5, 300 );

	ofFloatColor ambientColour(ambR, ambG, ambB, ambA);
	ofFloatColor diffuseColour(diffR, diffG, diffB);
	ofFloatColor specularColour(specR, specG, specB);

	light.setAmbientColor(ambientColour);
	light.setDiffuseColor(diffuseColour);
    light.setSpecularColor(specularColour);

    light.setPosition(xPos, yPos, zPos);
}





void LightManager::enable()
{
    light.enable();
}


void LightManager::disable()
{
    light.disable();
}
