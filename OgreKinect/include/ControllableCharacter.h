
#ifndef __ControllableCharacter_h_
#define __ControllableCharacter_h_

#include "Ogre.h"
#include "KinectController.h"
#include "JointOrientationCalculator.h"
#include "OgreMatrix3.h"
#include "AxisLines.h"

// Controllable Character
class ControllableCharacter
{

public:
	ControllableCharacter(void);
	virtual ~ControllableCharacter(void);

	virtual void setupCharacter(Ogre::SceneManager* mSceneManager, KinectController* controller);
	virtual void updatePerFrame(Ogre::Real elapsedTime);
	Ogre::SceneNode* getEntityNode();

public:
	Ogre::String entityName;

protected:
	KinectController*  controller;
	JointOrientationCalculator* jointCalc;

	std::vector<AxisLines*> axisLines;		// debug

	bool showBoneOrientationAxes;
	bool showJointYAxes;

	Ogre::Real skelCenter;
	Ogre::Vector3 bodyOffset;

	Ogre::SceneManager *mSceneManager;	
	Ogre::Entity* bodyEntity;
	Ogre::SceneNode* bodyNode;
	Ogre::Skeleton* skeleton;

protected:
	void transformBone(Ogre::String boneName, NuiManager::NuiJointIndex jointIdx);

	void setupBone(const Ogre::String& name, NuiManager::NuiJointIndex idx);
	void setupBone(const Ogre::String& name,const Ogre::Radian& angle, const Ogre::Vector3 axis);
	void setupBone(const Ogre::String& name,const Ogre::Degree& yaw,const Ogre::Degree& pitch,const Ogre::Degree& roll);
	void setupBone(const Ogre::String& name,const Ogre::Quaternion& q);

};

#endif