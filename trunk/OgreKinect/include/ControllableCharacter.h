
#ifndef __ControllableCharacter_h_
#define __ControllableCharacter_h_

#include "Ogre.h"
#include "KinectController.h"

//using namespace Ogre;

// Controllable Character
class ControllableCharacter
{

public:
	ControllableCharacter(void);
	virtual ~ControllableCharacter(void);

	virtual void setupCharacter(Ogre::SceneManager* mSceneManager, KinectController* controller);
	virtual void updatePerFrame(Ogre::Real elapsedTime);
	//virtual void transformBone(Bone* bone);

protected:
	KinectController*  controller;

	Ogre::SceneManager *mSceneManager;	
	Ogre::Entity* bodyEntity;
	Ogre::SceneNode* bodyNode;
	Ogre::Skeleton* skeleton;

	void transformBone(Ogre::String boneName, NuiManager::NuiJointIndex skeletonIdx);
	Ogre::Vector3 getJointNormal(NuiManager::NuiJointIndex skeletonIdx);

	void setupBone(const Ogre::String& name,const Ogre::Radian& angle, const Ogre::Vector3 axis);
	void setupBone(const Ogre::String& name,const Ogre::Degree& yaw,const Ogre::Degree& pitch,const Ogre::Degree& roll);
	void setupBone(const Ogre::String& name,const Ogre::Quaternion& q);

	// for debug only
	std::vector<Ogre::Entity*> bodyEntities;
	std::vector<Ogre::SceneNode*> bodyNodes;

};

#endif