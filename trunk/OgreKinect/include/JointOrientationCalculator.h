
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
		Ogre::Matrix3 populateMatrix(Ogre::Vector3 xCol, Ogre::Vector3 yCol, Ogre::Vector3 zCol);

		Ogre::Matrix3 makeMatrixFromX(Ogre::Vector3 v1);
		Ogre::Matrix3 makeMatrixFromY(Ogre::Vector3 v1);
		Ogre::Matrix3 makeMatrixFromZ(Ogre::Vector3 v1);

		Ogre::Matrix3 makeMatrixFromXY(Ogre::Vector3 xUnnormalized, Ogre::Vector3 yUnnormalized);
		Ogre::Matrix3 makeMatrixFromYX(Ogre::Vector3 xUnnormalized, Ogre::Vector3 yUnnormalized);
		Ogre::Matrix3 makeMatrixFromYZ(Ogre::Vector3 yUnnormalized, Ogre::Vector3 zUnnormalized);

		Ogre::Vector3 getPositionBetweenIndices(NuiManager::NuiJointIndex p1, NuiManager::NuiJointIndex p2);

		Ogre::Matrix3 getSkeletonJointOrientation(NuiManager::NuiJointIndex idx);
		
	protected:
		KinectController* controller;
	};
}

#endif