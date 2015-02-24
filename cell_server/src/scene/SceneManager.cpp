/*
 *  SceneManager.cpp
 *  cell
 *
 *  Created by James Alliban on 16/08/2011.
 *  Copyright 2011 Augmatic. All rights reserved.
 *
 */

#include "SceneManager.h"
#include "ofApp.h"
#include "KinectManager.h"


void SceneManager::init(KinectManager *kinectManager, ResourceManager *resourceManager)
{
	shader.load("shaders/TestShader");  //TestShader");

	userManager.init(kinectManager, resourceManager);
	fieldMan.init();
	cloudTagMan.init(&shader, resourceManager);

	isCamMouseInput = false;
	isCamMouseInputPaused = false;

	//camEasyCam.setTarget(cloudTagMan);
	camEasyCam.setDistance(100);
	camEasyCam.disableMouseInput();
	camEasyCam.setNearClip(0);

	setupViewports();

	smoothMouseX = smoothMouseY = 0;


	//testTagTexture.init();
}


void SceneManager::setupViewports()
{
	viewPort.x = 0;
	viewPort.y = 0;
    viewPort.width = ofGetWidth();
    viewPort.height = ofGetHeight();
//	viewPort.width = 1600;
//	viewPort.height = 900;
}


void SceneManager::update()
{
    camEasyCam.setFov(fov);
    
    userManager.update();
    fieldMan.update();
    cloudTagMan.update();

	if (isCamMouseInput)
	{
		if (!isCamMouseInputPaused)
		{
			smoothMouseX -= (smoothMouseX - ofGetMouseX()) * mouseSmoothAmount;
			smoothMouseY -= (smoothMouseY - ofGetMouseY()) * mouseSmoothAmount;
		}

		float mouseCamX = ofMap(smoothMouseX, 0, ofGetWidth(), -(camZ*2), (camZ*2), true) * camDistanceMultiplier;
		float mouseCamY = ofMap(smoothMouseY, 0, ofGetHeight(), camZ, -camZ, true) * camDistanceMultiplier;
		float mouseCamZ = cos(ofMap(smoothMouseX, 0, ofGetWidth(), -2, 2, true)) * camZ * camDistanceMultiplier;
        
        printf("mouseX:%i, mouseY:%i \n", ofGetMouseX(), ofGetMouseY());
        
		camEasyCam.setPosition(ofVec3f(mouseCamX, mouseCamY, mouseCamZ));
		camEasyCam.lookAt(nodeGrid, ofVec3f(0, 1, 0));
		camEasyCam.lookAt(ofVec3f(camDirectionX, camDirectionY, camDirectionZ), ofVec3f(0, 1, 0));
	}
	else
	{
		camEasyCam.setPosition(ofVec3f(camX, camY, camZ));
//		ofQuaternion q;
//		q.set(camRotX, camRotY, camRotZ, 1.0);
//		camEasyCam.setGlobalOrientation(q);
		camEasyCam.lookAt(ofVec3f(camDirectionX, camDirectionY, camDirectionZ), ofVec3f(0, 1, 0));
	}
}


void SceneManager::draw()
{
	glDisable(GL_DEPTH_TEST);
	ofPushStyle();
	ofSetColor(0, 0, 0);
	ofRect(viewPort);
	ofPopStyle();
	glEnable(GL_DEPTH_TEST);

	camEasyCam.begin(viewPort);

        if (isGridVisible)
        {
            ofPushStyle();
            ofPushMatrix();
            ofRotateZ(90);
            ofSetColor(255, gridAlpha);
            ofDrawGridPlane(100);
            ofPopMatrix();
            ofPopStyle();
        }
        userManager.draw();
		//nodeGrid.draw();
		fieldMan.draw();
		cloudTagMan.draw();

	camEasyCam.end();
}
