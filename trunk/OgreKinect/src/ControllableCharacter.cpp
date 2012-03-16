
#include "Stdafx.h"
#include "ControllableCharacter.h"

//-------------------------------------------------------------------------------------
ControllableCharacter::ControllableCharacter(void)
{
	this->showBoneOrientationAxes = true;	// debug
	this->showJointYAxes = false;			// debug
}

//-------------------------------------------------------------------------------------
ControllableCharacter::~ControllableCharacter(void)
{	
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::setupCharacter(Ogre::SceneManager* mSceneManager, KinectController* controller)
{
	this->mSceneManager = mSceneManager;
	this->controller = controller;

	jointCalc = new JointOrientationCalculator();
	jointCalc->setupController(controller);
			
	this->bodyEntity = mSceneManager->createEntity("RobotBody", "robot.mesh");
	this->bodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	this->bodyNode->attachObject(bodyEntity);

	for(int i = 0; i < this->bodyEntity->getNumSubEntities(); i++)
	{
		Ogre::SubEntity* sub = this->bodyEntity->getSubEntity(i);
		Ogre::MaterialPtr mat = sub->getMaterial();
		mat->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_COLOUR);
		mat->setCullingMode(Ogre::CullingMode::CULL_CLOCKWISE);
	}

	skeleton = this->bodyEntity->getSkeleton();	
	skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	for(int a = 0; a < NUI_SKELETON_POSITION_COUNT && showBoneOrientationAxes; a++)	// debug
	{ axisLines.push_back(new AxisLines()); }

	for(int a = 0; a < NUI_SKELETON_POSITION_COUNT && showJointYAxes; a++)			// debug
	{
		AxisLines* aLine = new AxisLines();
		aLine->setVisible(false, true, false);
		aLine->color2 = "Line/Yellow";
		jointYLines.push_back(aLine); 
	}

	setupBone("Joint1",		NuiJointIndex::HIP_CENTER);
	setupBone("Joint2",		NuiJointIndex::HIP_LEFT);
	//setupBone("Joint3",		NuiJointIndex::KNEE_LEFT);
	//setupBone("Joint4",		NuiJointIndex::ANKLE_LEFT);
	//setupBone("Joint5",		NuiJointIndex::FOOT_LEFT);
	setupBone("Joint6",		NuiJointIndex::HIP_RIGHT);
	//setupBone("Joint7",		NuiJointIndex::KNEE_RIGHT);
	//setupBone("Joint8",		NuiJointIndex::ANKLE_RIGHT);
	//setupBone("Joint9",		NuiJointIndex::FOOT_RIGHT);
	setupBone("Joint10",	NuiJointIndex::SPINE);
	setupBone("Joint11",	NuiJointIndex::SHOULDER_CENTER);
	setupBone("Joint12",	NuiJointIndex::HEAD);
	setupBone("Joint13",	NuiJointIndex::SHOULDER_LEFT);
	//setupBone("Joint14",	NuiJointIndex::ELBOW_LEFT);
	//setupBone("Joint15",	NuiJointIndex::WRIST_LEFT);
	setupBone("Joint16",	NuiJointIndex::SHOULDER_RIGHT);
	//setupBone("Joint17",	NuiJointIndex::ELBOW_RIGHT);
	//setupBone("Joint18",	NuiJointIndex::WRIST_RIGHT);
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::updatePerFrame(Ogre::Real elapsedTime)
{
	using namespace NuiManager;

	transformBone("Joint1",		NuiJointIndex::HIP_CENTER);
	transformBone("Joint2",		NuiJointIndex::HIP_LEFT);
	//transformBone("Joint3",		NuiJointIndex::KNEE_LEFT);
	//transformBone("Joint4",		NuiJointIndex::ANKLE_LEFT);
	//transformBone("Joint5",		NuiJointIndex::FOOT_LEFT);
	transformBone("Joint6",		NuiJointIndex::HIP_RIGHT);
	//transformBone("Joint7",		NuiJointIndex::KNEE_RIGHT);
	//transformBone("Joint8",		NuiJointIndex::ANKLE_RIGHT);
	//transformBone("Joint9",		NuiJointIndex::FOOT_RIGHT);
	transformBone("Joint10",	NuiJointIndex::SPINE);
	transformBone("Joint11",	NuiJointIndex::SHOULDER_CENTER);
	transformBone("Joint12",	NuiJointIndex::HEAD);
	transformBone("Joint13",	NuiJointIndex::SHOULDER_LEFT);
	//transformBone("Joint14",	NuiJointIndex::ELBOW_LEFT);
	//transformBone("Joint15",	NuiJointIndex::WRIST_LEFT);
	transformBone("Joint16",	NuiJointIndex::SHOULDER_RIGHT);
	//transformBone("Joint17",	NuiJointIndex::ELBOW_RIGHT);
	//transformBone("Joint18",	NuiJointIndex::WRIST_RIGHT);

}

//-------------------------------------------------------------------------------------
void ControllableCharacter::transformBone(Ogre::String boneName, NuiManager::NuiJointIndex jointIdx)
{
	int state = 0;	
	state = (int)controller->getJointStatus(jointIdx);

	if(state == 2)
	{
		Ogre::Bone* bone = skeleton->getBone(boneName);
		//Ogre::Quaternion qI = bone->getInitialOrientation();
		Ogre::Quaternion newQ = jointCalc->getSkeletonJointOrientation(jointIdx);

		bone->resetOrientation();
		newQ = bone->convertWorldToLocalOrientation(newQ);
		bone->setOrientation(newQ);

		if(showBoneOrientationAxes) axisLines[jointIdx]->updateLines(newQ.xAxis(), newQ.yAxis(), newQ.zAxis());									// debug
		if(showJointYAxes) jointYLines[jointIdx]->updateLines(Ogre::Vector3::ZERO, jointCalc->jointYAxes[jointIdx], Ogre::Vector3::ZERO);		// debug	
	}
	else
	{
	}
}


//-------------------------------------------------------------------------------------
void ControllableCharacter::setupBone(const Ogre::String& name, NuiJointIndex idx)
{
	Ogre::Bone* bone = bodyEntity->getSkeleton()->getBone(name);
	bone->setManuallyControlled(true);
	bone->setInheritOrientation(false);

	bone->setInitialState();

	if(showBoneOrientationAxes) axisLines[idx]->initAxis(name, this->bodyEntity, this->mSceneManager);		// debug
	if(showJointYAxes) jointYLines[idx]->initAxis(name, this->bodyEntity, this->mSceneManager);				// debug
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::setupBone(const Ogre::String& name,const Ogre::Radian& angle, const Ogre::Vector3 axis)
{
	Ogre::Quaternion q;
	q.FromAngleAxis(angle, axis);	 
	setupBone(name, q);
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::setupBone(const Ogre::String& name,const Ogre::Degree& yaw,const Ogre::Degree& pitch,const Ogre::Degree& roll)
{
	Ogre::Bone* bone = bodyEntity->getSkeleton()->getBone(name);
	bone->setManuallyControlled(true);
	bone->setInheritOrientation(false);

	bone->resetOrientation();

	bone->yaw(yaw);
	bone->pitch(pitch);
	bone->roll(roll);
	bone->setInitialState();
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::setupBone(const Ogre::String& name, const Ogre::Quaternion& q)
{
	Ogre::Bone* bone = bodyEntity->getSkeleton()->getBone(name);
	bone->setManuallyControlled(true);
	bone->setInheritOrientation(false);

	bone->resetOrientation();
	bone->setOrientation(q);

	bone->setInitialState();	
}