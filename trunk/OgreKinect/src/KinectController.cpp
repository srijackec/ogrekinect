
#include "Stdafx.h"
#include "KinectController.h"

//-------------------------------------------------------------------------------------
KinectController::KinectController(void)
	: kinectManager(0),
	skeleton(0)
{
}

//-------------------------------------------------------------------------------------
KinectController::~KinectController(void)
{	
}

//-------------------------------------------------------------------------------------
void KinectController::initController(void)
{
	kinectManager = new KinectManager();
	kinectManager->InitNui();
}

//-------------------------------------------------------------------------------------
void KinectController::unInitController(void)
{
	//if(skeleton) delete skeleton;

	if(kinectManager)
	{
		kinectManager->UnInitNui();
		delete kinectManager;
	}
}

//-------------------------------------------------------------------------------------
void KinectController::updatePerFrame(Ogre::Real elapsedTime)
{
	skeleton = kinectManager->getSkeleton();
}

//-------------------------------------------------------------------------------------
NUI_SKELETON_DATA* KinectController::getSkeletonData()
{
	return skeleton;
}