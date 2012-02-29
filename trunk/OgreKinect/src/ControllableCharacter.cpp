
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
void ControllableCharacter::setupCharacter(SceneManager* mSceneManager, KinectController* controller)
{
	this->mSceneManager = mSceneManager;
	this->controller = controller;

	this->bodyEntity = mSceneManager->createEntity("SinbadBody", "Sinbad.mesh");
	this->bodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	this->bodyNode->attachObject(bodyEntity);

	skeleton = this->bodyEntity->getSkeleton();
}

//-------------------------------------------------------------------------------------
void ControllableCharacter::updatePerFrame(Ogre::Real elapsedTime)
{
	Bone* root = skeleton->getBone("Root");
	NUI_SKELETON_DATA* skeletonData = controller->getSkeletonData();
	Vector3 position = Vector3(skeletonData->Position.x, skeletonData->Position.y, skeletonData->Position.z);
	root->translate(position * 10);

	std::cout << position << "\n";

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
}