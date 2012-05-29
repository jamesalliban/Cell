#pragma once

#include "ofMain.h"
#include "ofxMSKinect.h"
#include "ofxMSKinectDevice.h"

#define RED  0xFF0000
#define YELLOW  0xFFFF00
#define GREEN 0x00FF00
#define BLACK 0x0 

//example drawer to render Skeletons
class ofxSkeletonRenderer 
{
	public :

// ***********************************************************************
// Draws all skeletons in vector skeletons
// ***********************************************************************
		void DrawWithoutBox( int pad, ofVec2f inDepthSize, bool hasSkel, vector<ofxMSKinectSkeleton> *inskeletons )
		{
			padding = pad ;
			DepthSize = inDepthSize;
			hasSkeleton = hasSkel ;
			skeletons = inskeletons;
			joint_color =  RED;
			limb_colour = YELLOW;

			drawSkeletons(false) ;
		}
		
	
// ***********************************************************************
// Draws all skeletons in vector skeletons
// ***********************************************************************
		void Draw( int pad, ofVec2f inDepthSize, bool hasSkel, vector<ofxMSKinectSkeleton> *inskeletons )
		{
			padding = pad ;
			DepthSize = inDepthSize;
			hasSkeleton = hasSkel ;
			skeletons = inskeletons;
			joint_color =  RED;
			limb_colour = YELLOW;

			drawSkeletons(true) ;
		}

// ***********************************************************************
// Draws all skeletons in vector skeletons
// ***********************************************************************
		void DrawSkeletonsPostions( int pad, ofVec2f inDepthSize, bool hasSkel, vector<ofxMSKinectSkeleton> *inskeletons )
		{
			padding = pad ;
			DepthSize = inDepthSize;
			hasSkeleton = hasSkel ;
			skeletons = inskeletons;
			joint_color = GREEN ;
			limb_colour = YELLOW ;

			drawSkeletonsPositions() ;
		}

	

// ***********************************************************************
// Draws just the joints of all skeletons in vector skeletons
// ***********************************************************************
		void DrawJoints( int pad, ofVec2f inDepthSize, bool hasSkel, vector<ofxMSKinectSkeleton> *inskeletons )
		{
			padding = pad ;
			DepthSize = inDepthSize;
			hasSkeleton = hasSkel ;
			skeletons = inskeletons;
			joint_color = RED;
			limb_colour = YELLOW;
			drawSkeletonsJoints() ;
		}

// ***********************************************************************
// Draws just the limbs of all skeletons in vector skeletons
// ***********************************************************************
		void DrawLimbs( int pad, ofVec2f inDepthSize, bool hasSkel, vector<ofxMSKinectSkeleton> *inskeletons )
		{
			padding = pad ;
			DepthSize = inDepthSize;
			hasSkeleton = hasSkel ;
			skeletons = inskeletons;
			joint_color = RED;
			limb_colour = YELLOW;
			drawSkeletonsLimbs() ;
		}

// ***********************************************************************
// Draws box to display skels on 
// ***********************************************************************
		void DrawBox( int pad, ofVec2f inDepthSize )
		{
			padding = pad ;
			DepthSize = inDepthSize;
			hasSkeleton = false ;
			skeletons = NULL ;
			joint_color = RED;
			limb_colour = YELLOW;
			drawSkeletons(true) ;
		}

private :

	 void drawSkeletonSegment(int skelIndex, int index1, int index2)
		{
			ofxMSKinectSkeleton skeletonData = skeletons->at(skelIndex);
			int dwidth  = DepthSize.x;
			int dheight = DepthSize.y;

			int skeletonBoxLeft = padding;
			int skeletonBoxTop = padding + dheight + padding;
			int skeletonBoxWidth = (dwidth * 2) + padding;
			int skeletonBoxHeight = (dheight * 2) + padding;
	
			ofVec3f pos  = skeletonData.SkeletonPositions[index1];
			ofVec3f pos2 = skeletonData.SkeletonPositions[index2];
			
			float x;
			float y;

			ofxMSKinectDevice::TransFormToDepth(pos, &x, &y);
			pos.x = skeletonBoxWidth * x;
			pos.y = skeletonBoxHeight * y;

			ofxMSKinectDevice::TransFormToDepth(pos2, &x, &y);
			pos2.x = x * skeletonBoxWidth + 0.5f;
			pos2.y = y * skeletonBoxHeight + 0.5f;

			pos.x+=skeletonBoxLeft;
			pos.y+=skeletonBoxTop;

			pos2.x+=skeletonBoxLeft;
			pos2.y+=skeletonBoxTop;

			ofLine(pos.x, pos.y, pos2.x, pos2.y);
		}

// ***********************************************************************
// Draws whole skeleton in vector skeletons
// ***********************************************************************

		void drawSkeletons( bool drawbox )
		{
			int dwidth  = DepthSize.x;
			int dheight = DepthSize.y;

			int skeletonBoxLeft = padding;
			int skeletonBoxTop = padding + dheight + padding;
			int skeletonBoxWidth = (dwidth * 2) + padding;
			int skeletonBoxHeight = (dheight * 2) + padding;
		
			ofSetHexColor(BLACK);
			//Draw a black box
			if( drawbox )
				ofRect(skeletonBoxLeft, skeletonBoxTop, 0, (dwidth * 2) + padding, (dheight * 2) + padding);
			
			if(hasSkeleton)
			{
				int numOfSkelsTracked = skeletons->size();
				for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++)
				{

					for(int x = 0; x < 20; x++)
					{
						drawSkeletonJoint(skelIndex, x, skeletonBoxLeft, skeletonBoxTop, skeletonBoxWidth, skeletonBoxHeight);
					}
	
					ofSetHexColor(limb_colour);
					drawAllSegments ( skelIndex );
				}
			}
		}

	void drawSkeletonsLimbs()
	{
			int dwidth  = DepthSize.x;
			int dheight = DepthSize.y;

			int skeletonBoxLeft = padding;
			int skeletonBoxTop = padding + dheight + padding;
			int skeletonBoxWidth = (dwidth * 2) + padding;
			int skeletonBoxHeight = (dheight * 2) + padding;
			//Draw a black box
			ofSetHexColor(BLACK);
			ofRect(skeletonBoxLeft, skeletonBoxTop, 0, (dwidth * 2) + padding, (dheight * 2) + padding);

			if(hasSkeleton)
			{
				int numOfSkelsTracked = skeletons->size();
				for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++)
				{
					ofSetHexColor(limb_colour);
					drawAllSegments ( skelIndex );
				}
			}
	}

	void drawAllSegments ( int skelIndex )
	{
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);

		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
		drawSkeletonSegment(skelIndex, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);
	}

	void drawSkeletonsJoints() 
		{
			int dwidth  = DepthSize.x;
			int dheight = DepthSize.y;

			int skeletonBoxLeft = padding;
			int skeletonBoxTop = padding + dheight + padding;
			int skeletonBoxWidth = (dwidth * 2) + padding;
			int skeletonBoxHeight = (dheight * 2) + padding;
			//Draw a black box
			ofSetHexColor(BLACK);
			ofRect(skeletonBoxLeft, skeletonBoxTop, 0, (dwidth * 2) + padding, (dheight * 2) + padding);

			if(hasSkeleton)
			{
				int numOfSkelsTracked = skeletons->size();
				for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++)
				{
					for(int x = 0; x < 20; x++)
					{
						drawSkeletonJoint(skelIndex, x, skeletonBoxLeft, skeletonBoxTop, skeletonBoxWidth, skeletonBoxHeight);
					}
				}
			}
		}
	
	
	void drawSkeletonsPositions() 
		{
			int dwidth  = DepthSize.x;
			int dheight = DepthSize.y;

			int skeletonBoxLeft = padding;
			int skeletonBoxTop = padding + dheight + padding;
			int skeletonBoxWidth = (dwidth * 2) + padding;
			int skeletonBoxHeight = (dheight * 2) + padding;
		
		    if(hasSkeleton)
			{
				int numOfSkelsTracked = skeletons->size();
				for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++)
				{
					drawSkeletonPosition(skelIndex, skeletonBoxLeft, skeletonBoxTop, skeletonBoxWidth, skeletonBoxHeight);
				}
			}
		}


	void drawSkeletonPosition(int skelIndex,  int drawSpaceX, int drawSpaceY, int drawSpaceWidth, int drawSpaceHeight)
	{
		ofxMSKinectSkeleton skeletonData = skeletons->at(skelIndex);
		ofVec3f pos = skeletonData.Position;

		float fx=0;
		float fy=0;

		ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);
		pos.x = fx * drawSpaceWidth  + 0.5f;
		pos.y = fy * drawSpaceHeight + 0.5f;
		pos.x+=drawSpaceX;
		pos.y+=drawSpaceY;

		drawCenter(pos, joint_color);
		
	}

	void drawSkeletonJoint(int skelIndex, int id, int drawSpaceX, int drawSpaceY, int drawSpaceWidth, int drawSpaceHeight)
	{
		ofxMSKinectSkeleton skeletonData = skeletons->at(skelIndex);
		ofVec3f pos = skeletonData.SkeletonPositions[id];

		float fx=0;
		float fy=0;

		ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);
		pos.x = fx * drawSpaceWidth  + 0.5f;
		pos.y = fy * drawSpaceHeight + 0.5f;
		pos.x+=drawSpaceX;
		pos.y+=drawSpaceY;

		drawJoint(pos, joint_color);
		
	}

	void drawJoint(ofVec3f pos, int color)
	{
		int jointSize = 16;
		ofSetHexColor(color);
		ofRect(pos.x, pos.y, 0, jointSize, jointSize);
	}

	void drawCenter(ofVec3f pos, int color)
	{
	
		ofSetHexColor(color);
		
		ofPoint p ;
		p.x = pos.x;
		p.y = pos.y;
		p.z = pos.z; 
		ofCircle(p,12);
	
	}

	vector<ofxMSKinectSkeleton> *skeletons;
	int padding;
	ofVec2f DepthSize;
	bool hasSkeleton;
	int joint_color ;
	int limb_colour ;
};
