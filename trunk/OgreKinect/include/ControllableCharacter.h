
#ifndef __ControllableCharacter_h_
#define __ControllableCharacter_h_

#include "Ogre.h"
#include "KinectController.h"
#include "JointOrientationCalculator.h"
#include "OgreMatrix3.h"

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
	JointOrientationCalculator* jointCalc;

	Ogre::SceneManager *mSceneManager;	
	Ogre::Entity* bodyEntity;
	Ogre::SceneNode* bodyNode;
	Ogre::Skeleton* skeleton;

	void transformBone(Ogre::String boneName, NuiManager::NuiJointIndex childIdx, NuiManager::NuiJointIndex parentIdx);
	Ogre::Vector3 getJointNormal(NuiManager::NuiJointIndex skeletonIdx);
	Ogre::Radian getJointAngle(NuiManager::NuiJointIndex skeletonIdx);
	
	Ogre::Quaternion getJointOrientation(NuiManager::NuiJointIndex parentIdx, NuiManager::NuiJointIndex childIdx);

	void setupBone(const Ogre::String& name);
	void setupBone(const Ogre::String& name,const Ogre::Radian& angle, const Ogre::Vector3 axis);
	void setupBone(const Ogre::String& name,const Ogre::Degree& yaw,const Ogre::Degree& pitch,const Ogre::Degree& roll);
	void setupBone(const Ogre::String& name,const Ogre::Quaternion& q);

	// for debug only
	//std::vector<Ogre::Entity*> bodyEntities;
	//std::vector<Ogre::SceneNode*> bodyNodes;

};

#endif