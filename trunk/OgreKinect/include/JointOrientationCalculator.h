
#ifndef __JointOrientationCalculator_h__
#define __JointOrientationCalculator_h__

#include "Stdafx.h"
#include "NuiApi.h"
#include "NuiDataType.h"
#include "KinectController.h"

namespace NuiManager 
{
	class JointOrientationCalculator
	{
	public:
		JointOrientationCalculator(void);
		virtual ~JointOrientationCalculator(void);

		void setupController(KinectController* controller);

		bool areNearCollinear(Ogre::Vector3 v1, Ogre::Vector3 v2);
		Ogre::Quaternion buildQuaternion(Ogre::Vector3 xAxis, Ogre::Vector3 yAxis, Ogre::Vector3 zAxis);

		Ogre::Quaternion makeOrientationFromX(Ogre::Vector3 v1);
		Ogre::Quaternion makeOrientationFromY(Ogre::Vector3 v1);
		Ogre::Quaternion makeOrientationFromZ(Ogre::Vector3 v1);

		Ogre::Quaternion makeOrientationFromXY(Ogre::Vector3 xUnnormalized, Ogre::Vector3 yUnnormalized);
		Ogre::Quaternion makeOrientationFromYX(Ogre::Vector3 xUnnormalized, Ogre::Vector3 yUnnormalized);
		Ogre::Quaternion makeOrientationFromYZ(Ogre::Vector3 yUnnormalized, Ogre::Vector3 zUnnormalized);

		Ogre::Vector3 getDirection(NuiManager::NuiJointIndex p1, NuiManager::NuiJointIndex p2);

		Ogre::Quaternion getSkeletonJointOrientation(NuiManager::NuiJointIndex idx);

	public:
		std::vector<Ogre::Vector3> jointYAxes;	// debug only
		
	protected:
		KinectController* controller;
	};
}

#endif