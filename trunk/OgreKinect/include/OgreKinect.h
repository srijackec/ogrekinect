/*
-----------------------------------------------------------------------------
Filename:    OgreKinect.h
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (July 2011)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#ifndef __OgreKinect_h_
#define __OgreKinect_h_

#include "BaseApplication.h"
#include "KinectController.h"
#include "ControllableCharacter.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

class OgreKinect : public BaseApplication
{
public:
    OgreKinect(void);
    virtual ~OgreKinect(void);

protected:	
	KinectController* kinectController;
	ControllableCharacter* character;
	Ogre::TexturePtr texRenderTarget;

protected:
    virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& arg);

	void setUpKinect();
};

#endif // #ifndef __OgreKinect_h_
