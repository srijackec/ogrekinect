
#ifndef __KinectController_h_
#define __KinectController_h_

#include "Ogre.h"
#include "KinectManager.h"

using namespace NuiManager;

class KinectController
{
public:
	KinectController(void);
	virtual ~KinectController(void);

	virtual void initController(void);
	virtual void unInitController(void);
	virtual void updatePerFrame(Ogre::Real elapsedTime);
	
	NUI_SKELETON_DATA* getSkeletonData();

protected:
	KinectManager* kinectManager;
	NUI_SKELETON_DATA* skeleton;

	Ogre::SceneManager *mSceneManager;
};

#endif