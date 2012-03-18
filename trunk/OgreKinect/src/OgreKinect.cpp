/*
-----------------------------------------------------------------------------
Filename:    OgreKinect.cpp
-----------------------------------------------------------------------------
*/

#include "Stdafx.h"
#include "OgreKinect.h"

//-------------------------------------------------------------------------------------
OgreKinect::OgreKinect(void)
	:kinectController(0),
	character(0)
{

}
//-------------------------------------------------------------------------------------
OgreKinect::~OgreKinect(void)
{
	if(kinectController)
	{
		kinectController->unInitController();
		delete kinectController;
	}

	if(character) delete character;
}

//-------------------------------------------------------------------------------------
void OgreKinect::createScene(void)
{
	this->setUpKinect();

	character = new ControllableCharacter();
	character->setupCharacter(this->mSceneMgr, this->kinectController);

	// set shadow properties
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTextureSize(2048);
	mSceneMgr->setShadowTextureCount(1);
	
	// use a small amount of ambient lighting
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

	// add a bright light above the scene
	Ogre::Light* light = mSceneMgr->createLight();
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(-100, 400, 200);
	light->setSpecularColour(Ogre::ColourValue::White);

	// Fog
	this->mSceneMgr->getCurrentViewport()->setBackgroundColour(Ogre::ColourValue(0.26f, 0.627f, 1.0f));
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(1.0f, 1.0f, 0.8f), 0, 150, 500);

	// Floor
	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 10, 10, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* floor = mSceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName("Examples/Steel");
	floor->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->attachObject(floor);

	// Color Data	
	/*texRenderTarget = Ogre::TextureManager::getSingleton().createManual("texRenderTarget", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D, 320, 240, 0, Ogre::PF_B8G8R8A8, Ogre::TU_DEFAULT);

	Ogre::Rectangle2D *mMiniScreen = new Ogre::Rectangle2D(true);
	mMiniScreen->setCorners(0.5f, -0.5f, 1.0f, -1.0f);
	mMiniScreen->setBoundingBox(Ogre::AxisAlignedBox(-100000.0f * Ogre::Vector3::UNIT_SCALE, 100000.0f * Ogre::Vector3::UNIT_SCALE));

	Ogre::SceneNode* miniScreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode");
	miniScreenNode->attachObject(mMiniScreen);*/
	
}

//-------------------------------------------------------------------------------------
void OgreKinect::setUpKinect()
{	
	kinectController = new KinectController();
	kinectController->initController();
}

//-------------------------------------------------------------------------------------
bool OgreKinect::frameRenderingQueued(const Ogre::FrameEvent& arg)
{
	if(!BaseApplication::frameRenderingQueued(arg)) return false;
	
	kinectController->updatePerFrame(arg.timeSinceLastFrame);
	character->updatePerFrame(arg.timeSinceLastFrame);

	// Update Color Data
	this->kinectController->showColorData(this->texRenderTarget);

	return true;
}




