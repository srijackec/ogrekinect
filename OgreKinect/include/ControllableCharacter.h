
#ifndef __ControllableCharacter_h_
#define __ControllableCharacter_h_

#include "Ogre.h"
#include "KinectController.h"

using namespace Ogre;

// Controllable Character
class ControllableCharacter
{

public:
	ControllableCharacter(void);
	virtual ~ControllableCharacter(void);

	virtual void setupCharacter(SceneManager* mSceneManager, KinectController* controller);
	virtual void updatePerFrame(Ogre::Real elapsedTime);
	//virtual void transformBone(Bone* bone);

protected:
	KinectController*  controller;

	Ogre::SceneManager *mSceneManager;	
	Entity* bodyEntity;
	SceneNode* bodyNode;
	Skeleton* skeleton;

};

#endif