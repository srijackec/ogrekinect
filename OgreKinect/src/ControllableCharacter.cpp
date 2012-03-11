
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
	
	/*
	for(int i = 0; i < 20; i++)
	{
		char name[20];
		sprintf(name, "Body%d", i);

		Ogre::Entity* body = mSceneManager->createEntity(name, "ogrehead.mesh");
		Ogre::SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode();
		node->scale(Ogre::Vector3(0.3));
		node->attachObject(body);
		//node->setPosition(Ogre::Vector3::ZERO);

		bodyEntities.push_back(body);
		bodyNodes.push_back(node);
	}
	*/
			
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
	/*Bone* root = skeleton->getBone("Root");
	NUI_SKELETON_DATA* skeletonData = controller->getSkeletonData();
	Vector3 position = Vector3(skeletonData->Position.x, skeletonData->Position.y, skeletonData->Position.z);
	root->translate(position * 10);*/

	//NUI_SKELETON_DATA* skeletonData = controller->getSkeletonData();
	//std::cout << controller->getSkeletonPosition(NuiManager::HAND_RIGHT) << "\n";

	//BYTE* colorBuffer = controller->getColorBuffer();
	//BYTE* depthBuffer = controller->getDepthBuffer();

	
	//for(int a = 0; a < 20; a++)
	//{
	//	bodyNodes[a]->setPosition(controller->getSkeletonPosition((NuiManager::NuiSkeletonIndex)a) * 100);
		
		/*
		int state = skeletonData->eSkeletonPositionTrackingState[a];
		if(state == 0) std::cout << "BAD\n";
		else if(state == 1) std::cout << "INFERRED\n";
		else if(state == 2) std::cout << "GOOOD\n";*/

		///std::cout << pos02 << "\n";
	//}

	/*
	skeleton->getBone("Ulna.L")->resetToInitialState();
	skeleton->getBone("Ulna.R")->resetToInitialState();
	skeleton->getBone("Humerus.L")->resetToInitialState();
	skeleton->getBone("Humerus.R")->resetToInitialState();
	skeleton->getBone("Stomach")->resetToInitialState();
	skeleton->getBone("Chest")->resetToInitialState();
	skeleton->getBone("Thigh.L")->resetToInitialState();
	skeleton->getBone("Thigh.R")->resetToInitialState();
	skeleton->getBone("Calf.L")->resetToInitialState();
	skeleton->getBone("Calf.R")->resetToInitialState();
	skeleton->getBone("Root")->resetToInitialState();*/

	transformBone("Stomach", NuiManager::NuiJointIndex::HIP_CENTER);
	transformBone("Waist", NuiManager::NuiJointIndex::HIP_CENTER);
	transformBone("Root", NuiManager::NuiJointIndex::HIP_CENTER);
	transformBone("Chest", NuiManager::NuiJointIndex::SPINE);
	transformBone("Humerus.L", NuiManager::NuiJointIndex::ELBOW_LEFT);
	transformBone("Humerus.R", NuiManager::NuiJointIndex::ELBOW_RIGHT);
	transformBone("Ulna.L", NuiManager::NuiJointIndex::WRIST_LEFT);
	transformBone("Ulna.R", NuiManager::NuiJointIndex::WRIST_RIGHT);
	transformBone("Thigh.L", NuiManager::NuiJointIndex::KNEE_LEFT);
	transformBone("Thigh.R", NuiManager::NuiJointIndex::KNEE_RIGHT);
	transformBone("Calf.L", NuiManager::NuiJointIndex::ANKLE_LEFT);
	transformBone("Calf.R", NuiManager::NuiJointIndex::ANKLE_RIGHT);
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::transformBone(Ogre::String boneName, NuiManager::NuiJointIndex skeletonIdx)
{
	// Get the model skeleton bone info
	Ogre::Bone* bone = skeleton->getBone(boneName);
	Ogre::Quaternion qI = bone->getInitialOrientation();
	Ogre::Quaternion newQ = Ogre::Quaternion::IDENTITY;
		
	bone->resetOrientation(); //in order for the conversion from world to local to work.
	newQ = bone->convertWorldToLocalOrientation(newQ);

	bone->setOrientation(newQ*qI);	
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
		
		v1 = controller->getJointPosition(NuiJointIndex::SPINE).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::HIP_CENTER).normalise();
		v3 = (hipRight + hipLeft).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::SPINE)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::SPINE).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::HIP_CENTER).normalise();		
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_CENTER)	// has three children
	{
		Ogre::Vector3 shoulderRight = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT);
		Ogre::Vector3 shoulderLeft = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT);
				
		v1 = (shoulderRight + shoulderLeft).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::SPINE).normalise();		
	}
	else if(skeletonIdx == NuiJointIndex::HEAD)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::ELBOW_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_LEFT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::WRIST_LEFT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::WRIST_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::ELBOW_LEFT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::WRIST_LEFT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::HAND_LEFT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::HAND_LEFT)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::SHOULDER_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_CENTER).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::ELBOW_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::SHOULDER_RIGHT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::WRIST_RIGHT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::WRIST_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::ELBOW_RIGHT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::WRIST_RIGHT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::HAND_RIGHT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::HAND_RIGHT)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::HIP_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_CENTER).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::HIP_LEFT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::KNEE_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_LEFT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::ANKLE_LEFT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::ANKLE_LEFT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::KNEE_LEFT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::ANKLE_LEFT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::FOOT_LEFT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::FOOT_LEFT)
	{
		return normal;
	}
	else if(skeletonIdx == NuiJointIndex::HIP_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_CENTER).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::HIP_RIGHT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::KNEE_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::HIP_RIGHT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::ANKLE_RIGHT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::ANKLE_RIGHT)
	{
		v1 = controller->getJointPosition(NuiJointIndex::KNEE_RIGHT).normalise();
		v2 = controller->getJointPosition(NuiJointIndex::ANKLE_RIGHT).normalise();
		v3 = controller->getJointPosition(NuiJointIndex::FOOT_RIGHT).normalise();
	}
	else if(skeletonIdx == NuiJointIndex::FOOT_RIGHT)
	{
		return normal;
	}

	Ogre::Vector3 upper = v1 - v2;
	Ogre::Vector3 lower = v3 - v2;

	normal = upper.crossProduct(lower).normalise();

	return normal;
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::setupBone(const Ogre::String& name,const Ogre::Radian& angle, const Ogre::Vector3 axis)
{

	Ogre::Quaternion q;
	q.FromAngleAxis(angle,axis);	 
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

	//Matrix3 mat = bone->getLocalAxes();
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