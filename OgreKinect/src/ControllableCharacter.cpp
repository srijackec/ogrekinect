
#include "Stdafx.h"
#include "ControllableCharacter.h"

//-------------------------------------------------------------------------------------
ControllableCharacter::ControllableCharacter(void)
{
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
			
	this->bodyEntity = mSceneManager->createEntity("SinbadBody", "Sinbad.mesh");
	this->bodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	this->bodyNode->attachObject(bodyEntity);
	skeleton = this->bodyEntity->getSkeleton();
	
	skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
	
	Ogre::Quaternion q = Ogre::Quaternion::IDENTITY;
	Ogre::Quaternion q2, q3;
	Ogre::Vector3 xAxis, yAxis, zAxis;

	q.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0, 0, -1));
	q.ToAxes(xAxis, yAxis, zAxis);
	q2.FromAngleAxis(Ogre::Degree(90), xAxis);

	setupBone("Humerus.L", q * q2);

	q.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0, 0, 1));
	q.ToAxes(xAxis, yAxis , zAxis);
	q2.FromAngleAxis(Ogre::Degree(90), xAxis);

	setupBone("Humerus.R", q * q2);

	q.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0, 0, -1));	 
	q2.FromAngleAxis(Ogre::Degree(45), Ogre::Vector3(0, -1, 0));

	setupBone("Ulna.L", q * q2);

	q.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0, 0, 1));	 

	setupBone("Ulna.R", q * q2.Inverse());

	q.FromAngleAxis(Ogre::Degree(180), Ogre::Vector3(0, 1, 0));

	setupBone("Chest", q);

	setupBone("Stomach", q);

	q.FromAngleAxis(Ogre::Degree(180), Ogre::Vector3(1, 0, 0));	 	
	q2.FromAngleAxis(Ogre::Degree(180), Ogre::Vector3(0, 1, 0));

	setupBone("Thigh.L", q * q2);

	setupBone("Thigh.R", q * q2);

	setupBone("Calf.L", q * q2);

	setupBone("Calf.R", q * q2);

	setupBone("Root", Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0));
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::updatePerFrame(Ogre::Real elapsedTime)
{
	using namespace NuiManager;

	transformBone("Stomach",	NuiJointIndex::HIP_CENTER,	NuiJointIndex::SPINE);
	transformBone("Waist",		NuiJointIndex::HIP_CENTER,	NuiJointIndex::SPINE);
	transformBone("Root",		NuiJointIndex::HIP_CENTER,	NuiJointIndex::SPINE);
	transformBone("Chest",		NuiJointIndex::SPINE,		NuiJointIndex::SPINE);
	transformBone("Humerus.R",	NuiJointIndex::ELBOW_LEFT,	NuiJointIndex::SHOULDER_LEFT);
	transformBone("Humerus.L",	NuiJointIndex::ELBOW_RIGHT,	NuiJointIndex::SHOULDER_LEFT);
	transformBone("Ulna.R",		NuiJointIndex::WRIST_LEFT,	NuiJointIndex::ELBOW_LEFT);
	transformBone("Ulna.L",		NuiJointIndex::WRIST_RIGHT,	NuiJointIndex::ELBOW_LEFT);
	transformBone("Thigh.R",	NuiJointIndex::KNEE_LEFT,	NuiJointIndex::HIP_LEFT);
	transformBone("Thigh.L",	NuiJointIndex::KNEE_RIGHT,	NuiJointIndex::HIP_RIGHT);
	transformBone("Calf.R",		NuiJointIndex::ANKLE_LEFT,	NuiJointIndex::KNEE_LEFT);
	transformBone("Calf.L",		NuiJointIndex::ANKLE_RIGHT, NuiJointIndex::KNEE_RIGHT);
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::transformBone(Ogre::String boneName, NuiManager::NuiJointIndex childIdx, NuiManager::NuiJointIndex parentIdx)
{
	int state = 0;	
	state = (int)controller->getJointStatus(childIdx);

	if(state == 2)
	{
		std::cout << "tracked" << "\n";

		Ogre::Bone* bone = skeleton->getBone(boneName);
		Ogre::Quaternion qI = bone->getInitialOrientation();
						
		
		Ogre::Matrix3 orientMatrix = jointCalc->getSkeletonJointOrientation(childIdx);
		Ogre::Quaternion newQ;
		newQ.FromRotationMatrix(orientMatrix);

		bone->resetOrientation();
		newQ = bone->convertWorldToLocalOrientation(newQ);

		bone->setOrientation(newQ * qI);
	}
	else
	{
	}
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 ControllableCharacter::getJointNormal(NuiManager::NuiJointIndex skeletonIdx)
{
	using namespace NuiManager;

	Ogre::Vector3 normal = Ogre::Vector3::UNIT_Z;

	Ogre::Vector3 v1 = Ogre::Vector3::ZERO;		// upper
	Ogre::Vector3 v2 = Ogre::Vector3::ZERO;		// middle
	Ogre::Vector3 v3 = Ogre::Vector3::ZERO;		// lower

	if(skeletonIdx == NuiJointIndex::HIP_CENTER)	// has three children
	{
		Ogre::Vector3 hipRight = controller->getJointPosition(NuiJointIndex::HIP_RIGHT);
		Ogre::Vector3 hipLeft = controller->getJointPosition(NuiJointIndex::HIP_LEFT);
		
		v1 = controller->getJointPosition(NuiJointIndex::SPINE);
		v2 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);
		v3 = (hipRight + hipLeft);
	}
	else if(skeletonIdx == NuiJointIndex::SPINE)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::SPINE);
		v3 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);		
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_CENTER)	// has three children
	{
		Ogre::Vector3 shoulderRight = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		Ogre::Vector3 shoulderLeft = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);
				
		v1 = (shoulderRight + shoulderLeft);
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v3 = controller->getJointPosition(NuiJointIndex::SPINE);		
	}
	else if(skeletonIdx == NuiJointIndex::HEAD)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::ELBOW_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::WRIST_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::WRIST_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::WRIST_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::HAND_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::HAND_LEFT)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::ELBOW_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::WRIST_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::WRIST_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::WRIST_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::HAND_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::HAND_RIGHT)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::HIP_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::HIP_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::KNEE_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::ANKLE_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::ANKLE_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::ANKLE_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::FOOT_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::FOOT_LEFT)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::HIP_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::HIP_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::KNEE_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::ANKLE_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::ANKLE_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::ANKLE_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::FOOT_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::FOOT_RIGHT)
	{
		return normal;
	}

	Ogre::Vector3 upper = v1 - v2;
	Ogre::Vector3 lower = v3 - v2;

	normal = upper.crossProduct(lower).normalise();
	normal.normalise();

	return normal;
}

Ogre::Radian ControllableCharacter::getJointAngle(NuiManager::NuiJointIndex skeletonIdx)
{
	using namespace NuiManager;

	Ogre::Radian angle;

	Ogre::Vector3 v1 = Ogre::Vector3::ZERO;		// upper
	Ogre::Vector3 v2 = Ogre::Vector3::ZERO;		// middle
	Ogre::Vector3 v3 = Ogre::Vector3::ZERO;		// lower

	if(skeletonIdx == NuiJointIndex::HIP_CENTER)	// has three children
	{
		Ogre::Vector3 hipRight = controller->getJointPosition(NuiJointIndex::HIP_RIGHT);
		Ogre::Vector3 hipLeft = controller->getJointPosition(NuiJointIndex::HIP_LEFT);

		v1 = controller->getJointPosition(NuiJointIndex::SPINE);
		v2 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);
		v3 = (hipRight + hipLeft);
	}
	else if(skeletonIdx == NuiJointIndex::SPINE)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::SPINE);
		v3 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);		
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_CENTER)	// has three children
	{
		Ogre::Vector3 shoulderRight = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		Ogre::Vector3 shoulderLeft = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);

		v1 = (shoulderRight + shoulderLeft);
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v3 = controller->getJointPosition(NuiJointIndex::SPINE);		
	}
	else if(skeletonIdx == NuiJointIndex::HEAD)
	{
		return angle;
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::ELBOW_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::WRIST_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::WRIST_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::WRIST_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::HAND_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::HAND_LEFT)
	{
		return angle;
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::ELBOW_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::WRIST_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::WRIST_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::WRIST_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::HAND_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::HAND_RIGHT)
	{
		return angle;
	}
	else if(skeletonIdx == NuiJointIndex::HIP_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::HIP_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::KNEE_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::ANKLE_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::ANKLE_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT);
		v2 = controller->getJointPosition(NuiJointIndex::ANKLE_LEFT);
		v3 = controller->getJointPosition(NuiJointIndex::FOOT_LEFT);
	}
	else if(skeletonIdx == NuiJointIndex::FOOT_LEFT)
	{
		return angle;
	}
	else if(skeletonIdx == NuiJointIndex::HIP_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_CENTER);
		v2 = controller->getJointPosition(NuiJointIndex::HIP_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::KNEE_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::ANKLE_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::ANKLE_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT);
		v2 = controller->getJointPosition(NuiJointIndex::ANKLE_RIGHT);
		v3 = controller->getJointPosition(NuiJointIndex::FOOT_RIGHT);
	}
	else if(skeletonIdx == NuiJointIndex::FOOT_RIGHT)
	{
		return angle;
	}

	Ogre::Vector3 upper = v1 - v2;
	Ogre::Vector3 lower = v3 - v2;

	angle = Ogre::Math::ACos(upper.dotProduct(lower));

	return angle;
}


//-------------------------------------------------------------------------------------
Ogre::Quaternion ControllableCharacter::getJointOrientation(NuiManager::NuiJointIndex parentIdx, NuiManager::NuiJointIndex childIdx)
{
	using namespace NuiManager;
	
	Ogre::Vector3 parent = controller->getJointPosition(parentIdx);	
	Ogre::Vector3 child = controller->getJointPosition(childIdx);
	
	Ogre::Radian angle = Ogre::Math::ACos(parent.dotProduct(child) / (parent.length() * child.length()) );
	Ogre::Vector3 axis = parent.crossProduct(child);
	axis.normalise();

	Ogre::Quaternion q;
	q.FromAngleAxis(angle, axis);

	return q;
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::setupBone(const Ogre::String& name)
{
	Ogre::Bone* bone = bodyEntity->getSkeleton()->getBone(name);
	bone->setManuallyControlled(true);
	//bone->setInheritOrientation(false);
	
	//Ogre::Quaternion q = Ogre::Quaternion(0,0,1);
	//bone->setOrientation(q);

	//bone->resetOrientation();
	//bone->setInitialState();
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
void ControllableCharacter::setupBone(const Ogre::String& name,const Ogre::Quaternion& q)
{
	Ogre::Bone* bone = bodyEntity->getSkeleton()->getBone(name);
	bone->setManuallyControlled(true);
	bone->setInheritOrientation(false);

	bone->resetOrientation();
	bone->setOrientation(q);

	bone->setInitialState();
}