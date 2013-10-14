/*
 *  MyGui.cpp
 *  cell
 *
 *  Created by James Alliban on 10/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#include "MyGui.h"



MyGui::MyGui()
{

	app = (TestApp*)ofGetAppPtr();

	int guiWidth = 250;

	//Camera
	gui.addTitle("Camera").width = guiWidth;
	gui.addTitle("Position/Rotation", 20).width = guiWidth;
	gui.addSlider("Camera X", app->sceneManager.camX, -150, 150).width = guiWidth;
	gui.addSlider("Camera Y", app->sceneManager.camY, -150, 150).width = guiWidth;
	gui.addSlider("Camera Z", app->sceneManager.camZ, -150, 150).width = guiWidth;
	gui.addSlider("Camera Direction X", app->sceneManager.camDirectionX, -50, 50).width = guiWidth;
	gui.addSlider("Camera Direction Y", app->sceneManager.camDirectionY, -50, 50).width = guiWidth;
	gui.addSlider("Camera Direction Z", app->sceneManager.camDirectionZ, -50, 50).width = guiWidth;
	gui.addSlider("FOV", app->sceneManager.fov, 0, 50).width = guiWidth;
	gui.addTitle("Mouse", 20).width = guiWidth;
	gui.addToggle("Mouse input", app->sceneManager.isCamMouseInput);
	gui.addSlider("Mouse input smooth", app->sceneManager.mouseSmoothAmount, 0.01, 0.3).width = guiWidth;

	// lighting
	gui.addPage("Lighting").width = guiWidth;
	gui.addTitle("Position").width = guiWidth;
	gui.addSlider("X position", app->sceneManager.light.xPos, -150, 150).width = guiWidth;
	gui.addSlider("Y position", app->sceneManager.light.yPos, -150, 150).width = guiWidth;
	gui.addSlider("Z position", app->sceneManager.light.zPos, -150, 150).width = guiWidth;
	gui.addSlider("Ambient Col R", app->sceneManager.light.ambR, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Ambient Col G", app->sceneManager.light.ambG, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Ambient Col B", app->sceneManager.light.ambB, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Ambient Col A", app->sceneManager.light.ambA, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Diffuse Col R", app->sceneManager.light.diffR, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Diffuse Col G", app->sceneManager.light.diffG, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Diffuse Col B", app->sceneManager.light.diffB, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Specular Col R", app->sceneManager.light.specR, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Specular Col G", app->sceneManager.light.specG, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Specular Col B", app->sceneManager.light.specB, 0.0, 1.0).width = guiWidth;

	// Cloud Tag Shader
	gui.addPage("Cloud Tag Shader").width = guiWidth;
	gui.addTitle("Cloud Tag Shader").width = guiWidth;
	gui.addSlider("Contrast min", app->sceneManager.cloudTagMan.shadeContrastMin, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Contrast max", app->sceneManager.cloudTagMan.shadeContrastMax, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Brightness min", app->sceneManager.cloudTagMan.shadeBrightnessMin, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Brightness max", app->sceneManager.cloudTagMan.shadeBrightnessMax, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Alpha min", app->sceneManager.cloudTagMan.shadeAlphaMin, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Alpha max", app->sceneManager.cloudTagMan.shadeAlphaMax, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Blend mix", app->sceneManager.cloudTagMan.shadeBlendMix, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Blend mode", app->sceneManager.cloudTagMan.shadeBlendMode, 0, 10).width = guiWidth;
	gui.addToggle("Apply", app->sceneManager.isUpdateVars);

	// line visual
	gui.addPage("Line Appearance").width = guiWidth;
	gui.addToggle("Lines enabled", app->sceneManager.cloudTagMan.areLinesEnabled);
	gui.addTitle("Line Shader").width = guiWidth;
	gui.addSlider("Line Contrast min", app->sceneManager.cloudTagMan.lineContrastMin, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Line Contrast max", app->sceneManager.cloudTagMan.lineContrastMax, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Line Brightness min", app->sceneManager.cloudTagMan.lineBrightnessMin, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Line Brightness max", app->sceneManager.cloudTagMan.lineBrightnessMax, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Line Alpha min", app->sceneManager.cloudTagMan.lineAlphaMin, -1.0, 1.0).width = guiWidth;
	gui.addSlider("Line Alpha max", app->sceneManager.cloudTagMan.lineAlphaMax, -1.0, 1.0).width = guiWidth;
	gui.addTitle("Line props").width = guiWidth;
	gui.addSlider("Line Length Squared Min", app->sceneManager.cloudTagMan.lineLengthSquaredMin, 500.0, 10000.0).width = guiWidth;
	gui.addSlider("Map Line Distance Min", app->sceneManager.cloudTagMan.mapLineDistanceMin, 500, 10000.0).width = guiWidth;
	gui.addSlider("Map Line Distance Max", app->sceneManager.cloudTagMan.mapLineDistanceMax, 500, 10000.0).width = guiWidth;
	gui.addSlider("Map Z Area Min", app->sceneManager.cloudTagMan.lineZAreaMin, -100, 10.0).width = guiWidth;
	gui.addSlider("Map Z Area Max", app->sceneManager.cloudTagMan.lineZAreaMax, 10, 100.0).width = guiWidth;
	gui.addSlider("Line Thickness", app->sceneManager.cloudTagMan.lineThickness, 0.0, 10.0).width = guiWidth;
	gui.addTitle("Line Animation").width = guiWidth;
	gui.addSlider("Init line fade", app->sceneManager.cloudTagMan.lineFadeTime, 0.0, 5.0).width = guiWidth;

	// Cloud tag static animation
	gui.addPage("Cloud tag static animation").width = guiWidth;
	gui.addToggle("Tags enabled", app->sceneManager.cloudTagMan.areTagsEnabled);
	gui.addTitle("Animation").width = guiWidth;
//	gui.addSlider("Decay Min", app->sceneManager.cloudTagMan.decayMin, 0.3, 0.999).width = guiWidth;
//	gui.addSlider("Decay Max", app->sceneManager.cloudTagMan.decayMax, 0.3, 0.999).width = guiWidth;
	gui.addSlider("Perlin Input offset multiplier", app->sceneManager.cloudTagMan.perlinInputOffsetMultiplier, 0.0000, 10).width = guiWidth;
	gui.addSlider("Speed", app->sceneManager.cloudTagMan.speed, 0.0001, 0.05).width = guiWidth;
	gui.addSlider("Frame multiplier", app->sceneManager.cloudTagMan.frameMultiplier, 0.00001, 0.01).width = guiWidth;
	gui.addSlider("Noise multiplier", app->sceneManager.cloudTagMan.noiseMultiplier, 0.001, 0.5).width = guiWidth;
	gui.addTitle("Boundaries").width = guiWidth;
	gui.addSlider("Out of Bounds Angle Add", app->sceneManager.cloudTagMan.angleAdd, 0.001, 1).width = guiWidth;
	gui.addSlider("Out of Bounds Pos Add Max", app->sceneManager.cloudTagMan.outOfBoundsPosAddMax, 0.001, 1).width = guiWidth;
	gui.addSlider("Out of Bounds Pos Add Min", app->sceneManager.cloudTagMan.outOfBoundsPosAddMin, 0.0001, 0.01).width = guiWidth;
	gui.addSlider("Out of Bounds Pos Add Decay", app->sceneManager.cloudTagMan.outOfBoundsPosAddDecay, 1, 5).width = guiWidth;

	gui.addPage("Cloud tag boundaries").width = guiWidth;
	gui.addTitle("Boundaries").width = guiWidth;
	gui.addSlider("Boundary Width", app->sceneManager.cloudTagMan.boundaryW, 0.50, 500.0).width = guiWidth;
	gui.addSlider("Boundary Width Front", app->sceneManager.cloudTagMan.boundaryWFront, 0.50, 500.0).width = guiWidth;
	gui.addSlider("Boundary Depth", app->sceneManager.cloudTagMan.boundaryD, 0.50, 500.0).width = guiWidth;
	gui.addSlider("Boundary Height", app->sceneManager.cloudTagMan.boundaryH, 0.50, 100.0).width = guiWidth;
	gui.addTitle("Height offset").width = guiWidth;
	gui.addSlider("Boundary Centre Height", app->sceneManager.cloudTagMan.boundaryCentreH, 0.0, 100.0).width = guiWidth;

    // Cloud tag attractor animation
	gui.addPage("Cloud tag attractor animation").width = guiWidth;
	gui.addTitle("Cloud Tag attraction").width = guiWidth;
	gui.addSlider("Attraction speed Min", app->sceneManager.cloudTagMan.attractionSpeedMin, 0.00001, 0.1).width = guiWidth;
	gui.addSlider("Attraction speed Max", app->sceneManager.cloudTagMan.attractionSpeedMax, 0.5, 0.99).width = guiWidth;
	gui.addSlider("Attraction speed Pow", app->sceneManager.cloudTagMan.attractionSpeedPow, 1, 30).width = guiWidth;
	gui.addToggle("Apply", app->sceneManager.isUpdateVars);
	gui.addSlider("Attraction speed Pow", app->sceneManager.cloudTagMan.attractionSpeedPow, 1, 30).width = guiWidth;


	gui.addPage("Cloud tag Y scaling").width = guiWidth;
	gui.addToggle("y Scale Only On Attraction", app->sceneManager.cloudTagMan.yScaleOnlyOnAttraction);
	gui.addSlider("Y Min", app->sceneManager.cloudTagMan.yMin, 0, 20).width = guiWidth;
	gui.addSlider("Y Max", app->sceneManager.cloudTagMan.yMax, 10, 60).width = guiWidth;
	gui.addSlider("Scale Min", app->sceneManager.cloudTagMan.scaleMin, 0.01, 1).width = guiWidth;
	gui.addSlider("Scale Max", app->sceneManager.cloudTagMan.scaleMax, 0.01, 2).width = guiWidth;

    gui.addPage("Fields 1").width = guiWidth;
	gui.addTitle("Debug").width = guiWidth;
	gui.addToggle("Field Lines Visible", app->sceneManager.fieldMan.isFieldLineVisible);
	gui.addTitle("Enegery field - Red").width = guiWidth;
	gui.addToggle("Enabled", app->sceneManager.fieldMan.fieldEnergy->isEnabled);
	gui.addSlider("Play Area", app->sceneManager.fieldMan.fieldEnergy->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed", app->sceneManager.fieldMan.fieldEnergy->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size", app->sceneManager.fieldMan.fieldEnergy->lengthSquaredMin, 100, 500.0).width = guiWidth;
	gui.addSlider("Frame Multiplier", app->sceneManager.fieldMan.fieldEnergy->frameMultiplier, 0.0, 1.0).width = guiWidth;
	gui.addSlider("Noise Multiplier", app->sceneManager.fieldMan.fieldEnergy->noiseMultiplier, 0.0, 1.0).width = guiWidth;
	gui.addTitle("Repel field - Green").width = guiWidth;
	gui.addToggle("Enabled", app->sceneManager.fieldMan.fieldRepel->isEnabled);
	gui.addSlider("Play Area", app->sceneManager.fieldMan.fieldRepel->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed", app->sceneManager.fieldMan.fieldRepel->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size", app->sceneManager.fieldMan.fieldRepel->lengthSquaredMin, 100, 500.0).width = guiWidth;
	gui.addSlider("Repel Strength", app->sceneManager.fieldMan.fieldRepel->repelStrength, 0.001, 0.03).width = guiWidth;

    gui.addPage("Fields 2").width = guiWidth;
	gui.addTitle("Scale Up field - Yellow").width = guiWidth;
	gui.addToggle("Enabled", app->sceneManager.fieldMan.fieldScaleUp->isEnabled);
	gui.addSlider("Play Area", app->sceneManager.fieldMan.fieldScaleUp->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed", app->sceneManager.fieldMan.fieldScaleUp->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size", app->sceneManager.fieldMan.fieldScaleUp->lengthSquaredMin, 100, 500.0).width = guiWidth;
	gui.addSlider("Scale Strength", app->sceneManager.fieldMan.fieldScaleUp->scaleUpStrength, 0.5, 3.0).width = guiWidth;
	gui.addTitle("Height Down field - Blue").width = guiWidth;
	gui.addToggle("Enabled", app->sceneManager.fieldMan.fieldHeightDown->isEnabled);
	gui.addSlider("Play Area", app->sceneManager.fieldMan.fieldHeightDown->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed", app->sceneManager.fieldMan.fieldHeightDown->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size", app->sceneManager.fieldMan.fieldHeightDown->lengthSquaredMin, 100, 500.0).width = guiWidth;
	gui.addSlider("Height Adjust", app->sceneManager.fieldMan.fieldHeightDown->heightAdjust, 0.001, 0.5).width = guiWidth;

    gui.addPage("Fields 3").width = guiWidth;
	gui.addTitle("Height Up field - Pink").width = guiWidth;
	gui.addToggle("Enabled", app->sceneManager.fieldMan.fieldHeightUp->isEnabled);
	gui.addSlider("Play Area", app->sceneManager.fieldMan.fieldHeightUp->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed", app->sceneManager.fieldMan.fieldHeightUp->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size", app->sceneManager.fieldMan.fieldHeightUp->lengthSquaredMin, 100, 500.0).width = guiWidth;
	gui.addSlider("Height Adjust", app->sceneManager.fieldMan.fieldHeightUp->heightAdjust, 0.001, 0.5).width = guiWidth;

    gui.addPage("Fields 4").width = guiWidth;
	gui.addTitle("Colour 1 field - Double Red").width = guiWidth;
	gui.addToggle("Enabled 1", app->sceneManager.fieldMan.fieldColour1->isEnabled).width = guiWidth;
	gui.addSlider("Play Area 1", app->sceneManager.fieldMan.fieldColour1->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed 1", app->sceneManager.fieldMan.fieldColour1->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size 1", app->sceneManager.fieldMan.fieldColour1->lengthSquaredMin, 100, 1000.0).width = guiWidth;
	gui.addColorPicker("Colours 1", app->sceneManager.fieldMan.fieldColour1->colours).width = guiWidth;
	gui.addTitle("Colour 2 field - Double Green").width = guiWidth;
	gui.addToggle("Enabled 2", app->sceneManager.fieldMan.fieldColour2->isEnabled).width = guiWidth;
	gui.addSlider("Play Area 2", app->sceneManager.fieldMan.fieldColour2->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed 2", app->sceneManager.fieldMan.fieldColour2->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size 2", app->sceneManager.fieldMan.fieldColour2->lengthSquaredMin, 100, 1000.0).width = guiWidth;
	gui.addColorPicker("Colours", app->sceneManager.fieldMan.fieldColour2->colours).width = guiWidth;
	gui.addTitle("Colour 3 field - Double Blue").width = guiWidth;
	gui.addToggle("Enabled 3", app->sceneManager.fieldMan.fieldColour3->isEnabled).width = guiWidth;
	gui.addSlider("Play Area 3", app->sceneManager.fieldMan.fieldColour3->playArea, 20, 150.0).width = guiWidth;
	gui.addSlider("Speed 3", app->sceneManager.fieldMan.fieldColour3->movementSpeed, 0.01, 0.1).width = guiWidth;
	gui.addSlider("Area size 3", app->sceneManager.fieldMan.fieldColour3->lengthSquaredMin, 100, 1000.0).width = guiWidth;
	gui.addColorPicker("Colours 3", app->sceneManager.fieldMan.fieldColour3->colours).width = guiWidth;


	gui.addPage("Kinect debug").width = guiWidth;
	gui.addTitle("Skeleton").width = guiWidth;
	gui.addToggle("Show Joint Spheres", app->sceneManager.userManager.isJointSpheres);
	gui.addToggle("Show Joint Lines", app->sceneManager.userManager.isJointLines);
    gui.addToggle("Show Joint Pos Data", app->sceneManager.userManager.isPositionDataDisplayed);
    gui.addToggle("Show User Data", app->sceneManager.userManager.isUserDataDisplayed);

	gui.addPage("Skeleton calibration All").width = guiWidth;
	gui.addTitle("Perspective X offset").width = guiWidth;
	gui.addSlider("Range Min", app->sceneManager.userManager.xCorrectionOffsetRangeMin, -300, 0).width = guiWidth;
	gui.addSlider("Range Max", app->sceneManager.userManager.xCorrectionOffsetRangeMax, 0, 300).width = guiWidth;
	gui.addSlider("Offset Min", app->sceneManager.userManager.xCorrectionOffsetMin, -350, 350).width = guiWidth;
	gui.addSlider("Offset Max", app->sceneManager.userManager.xCorrectionOffsetMax, -350, 350).width = guiWidth;
	gui.addSlider("Offset Modifier", app->sceneManager.userManager.xCorrectionOffsetModifier, 0, 500).width = guiWidth;

	gui.addPage("Skeleton calibration Client 0").width = guiWidth;
	gui.addTitle("Scale").width = guiWidth;
	gui.addSlider("Scale", app->sceneManager.userManager.skeletonScale[0], 0.01, 2.0).width = guiWidth;
	gui.addTitle("Rotation").width = guiWidth;
    gui.addSlider("Degrees", app->sceneManager.userManager.skeletonRotDegrees[0], -50, 50).width = guiWidth;
    gui.addSlider("Rotation X", app->sceneManager.userManager.skeletonRotX[0], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Y", app->sceneManager.userManager.skeletonRotY[0], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Z", app->sceneManager.userManager.skeletonRotZ[0], 0.0, 1.0).width = guiWidth;
	gui.addTitle("Position Offset").width = guiWidth;
    gui.addSlider("Pos offset X", app->sceneManager.userManager.skeletonPosOffsetX[0], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Y", app->sceneManager.userManager.skeletonPosOffsetY[0], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Z", app->sceneManager.userManager.skeletonPosOffsetZ[0], -500.0, 500.0).width = guiWidth;

	gui.addPage("Skeleton calibration Client 1").width = guiWidth;
	gui.addTitle("Scale").width = guiWidth;
	gui.addSlider("Scale", app->sceneManager.userManager.skeletonScale[1], 0.01, 2.0).width = guiWidth;
	gui.addTitle("Rotation").width = guiWidth;
    gui.addSlider("Degrees", app->sceneManager.userManager.skeletonRotDegrees[1], -50, 50).width = guiWidth;
    gui.addSlider("Rotation X", app->sceneManager.userManager.skeletonRotX[1], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Y", app->sceneManager.userManager.skeletonRotY[1], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Z", app->sceneManager.userManager.skeletonRotZ[1], 0.0, 1.0).width = guiWidth;
	gui.addTitle("Position Offset").width = guiWidth;
    gui.addSlider("Pos offset X", app->sceneManager.userManager.skeletonPosOffsetX[1], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Y", app->sceneManager.userManager.skeletonPosOffsetY[1], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Z", app->sceneManager.userManager.skeletonPosOffsetZ[1], -500.0, 500.0).width = guiWidth;

	gui.addPage("Skeleton calibration Client 2").width = guiWidth;
	gui.addTitle("Scale").width = guiWidth;
	gui.addSlider("Scale", app->sceneManager.userManager.skeletonScale[2], 0.01, 2.0).width = guiWidth;
	gui.addTitle("Rotation").width = guiWidth;
    gui.addSlider("Degrees", app->sceneManager.userManager.skeletonRotDegrees[2], -50, 50).width = guiWidth;
    gui.addSlider("Rotation X", app->sceneManager.userManager.skeletonRotX[2], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Y", app->sceneManager.userManager.skeletonRotY[2], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Z", app->sceneManager.userManager.skeletonRotZ[2], 0.0, 1.0).width = guiWidth;
	gui.addTitle("Position Offset").width = guiWidth;
    gui.addSlider("Pos offset X", app->sceneManager.userManager.skeletonPosOffsetX[2], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Y", app->sceneManager.userManager.skeletonPosOffsetY[2], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Z", app->sceneManager.userManager.skeletonPosOffsetZ[2], -500.0, 500.0).width = guiWidth;

	gui.addPage("Skeleton calibration Client 3").width = guiWidth;
	gui.addTitle("Scale").width = guiWidth;
	gui.addSlider("Scale", app->sceneManager.userManager.skeletonScale[3], 0.01, 2.0).width = guiWidth;
	gui.addTitle("Rotation").width = guiWidth;
    gui.addSlider("Degrees", app->sceneManager.userManager.skeletonRotDegrees[3], -50, 50).width = guiWidth;
    gui.addSlider("Rotation X", app->sceneManager.userManager.skeletonRotX[3], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Y", app->sceneManager.userManager.skeletonRotY[3], 0.0, 1.0).width = guiWidth;
    gui.addSlider("Rotation Z", app->sceneManager.userManager.skeletonRotZ[3], 0.0, 1.0).width = guiWidth;
	gui.addTitle("Position Offset").width = guiWidth;
    gui.addSlider("Pos offset X", app->sceneManager.userManager.skeletonPosOffsetX[3], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Y", app->sceneManager.userManager.skeletonPosOffsetY[3], -500.0, 500.0).width = guiWidth;
    gui.addSlider("Pos offset Z", app->sceneManager.userManager.skeletonPosOffsetZ[3], -500.0, 500.0).width = guiWidth;

	gui.addPage("Duplicates").width = guiWidth;
	gui.addTitle("Debug").width = guiWidth;
    gui.addToggle("Display average line", app->sceneManager.userManager.isAverageLineDisplayed);
    //gui.addToggle("Display only dominant users", app->sceneManager.userManager.?????????/);
	gui.addTitle("Vars").width = guiWidth;
    gui.addSlider("Length Squared Min", app->sceneManager.userManager.duplicateLengthSquaredMin, 1, 30.0).width = guiWidth;

	gui.addPage("Non Kinect User Degug").width = guiWidth;
	gui.addTitle("User").width = guiWidth;
    gui.addToggle("Pause User", app->sceneManager.userManager.isNonKinectUserPaused);
	gui.addTitle("Cloud").width = guiWidth;
    gui.addToggle("Pause Cloud Tag Attraction", app->sceneManager.cloudTagMan.isTagAttractionPaused);
	// randomise demographics
	// add user
	// remove user


	gui.addPage("Grid").width = guiWidth;
    gui.addToggle("Grid active", app->sceneManager.nodeGrid.isActive);
    gui.addSlider("Alpha", app->sceneManager.nodeGrid.alpha, 0, 255).width = guiWidth;

	gui.addPage("Edge blackour").width = guiWidth;
    gui.addSlider("height", app->topBlockHeight, 1, 200).width = guiWidth;
    gui.addSlider("bottom", app->topBlockBot, 1, 200).width = guiWidth;
    gui.addSlider("left", app->leftBlockW, 1, 100).width = guiWidth;
    gui.addSlider("right", app->rightBlockW, 1, 100).width = guiWidth;




	// background (including affectedByLight boolean)

	// Cloud Tag ambient behaviours

	// Cloud Tag area

	// Grid setup

	//gui.show();

	gui.loadFromXML();

	//gui.show();
}




void MyGui::draw()
{
	gui.draw();
}



void MyGui::keyPressed(int key)
{
	if(key>='0' && key<='9')
	{
		gui.setPage(key - '0');
		gui.show();
	}
	else
	{
		switch(key)
		{
			case ' ': gui.toggleDraw(); break;
			case 'g': gui.toggleDraw(); break;
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
			case 'f': ofToggleFullscreen() ; break;
			case 'c': ofSaveScreen("screenshot.jpg") ; break;
		}
	}

}
