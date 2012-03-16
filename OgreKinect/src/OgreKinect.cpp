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
	
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);

	// Fog
	this->mSceneMgr->getCurrentViewport()->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 0.8f));
	mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(1.0f, 1.0f, 0.8f), 0, 150, 500);

	// Floor
	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 10, 10, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* floor = mSceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName("Examples/Rockwall");
	floor->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->attachObject(floor);
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

	return true;
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#ifdef _DEBUG
	int main()
#else
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#endif	
#else
	int main(int argc, char *argv[])
#endif
	{
        // Create application object
        OgreKinect app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
