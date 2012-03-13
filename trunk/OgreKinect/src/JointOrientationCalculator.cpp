
#include "Stdafx.h"
#include "JointOrientationCalculator.h"

//-------------------------------------------------------------------------------------
NuiManager::JointOrientationCalculator::JointOrientationCalculator(void)
{

}

//-------------------------------------------------------------------------------------
NuiManager::JointOrientationCalculator::~JointOrientationCalculator(void)
{

}

//-------------------------------------------------------------------------------------
void NuiManager::JointOrientationCalculator::setupController(KinectController* controller)
{
	this->controller = controller;
}

//-------------------------------------------------------------------------------------
// check whether the two vectors lie in the same line (and also parallel)
bool NuiManager::JointOrientationCalculator::areNearCollinear(Ogre::Vector3 v1, Ogre::Vector3 v2)
{
	v1.normalise();
	v2.normalise();

	Ogre::Real product = v1.dotProduct(v2);

	return product >= 0.8f;		// [0, 1]
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 NuiManager::JointOrientationCalculator::getPositionBetweenIndices(NuiManager::NuiJointIndex p1, NuiManager::NuiJointIndex p2)
{
	Ogre::Vector3 pVec1 = controller->getJointPosition(p1);
	Ogre::Vector3 pVec2 = controller->getJointPosition(p2);

	return pVec2 - pVec1;
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::populateMatrix(Ogre::Vector3 xCol, Ogre::Vector3 yCol, Ogre::Vector3 zCol)
{
	Ogre::Matrix3 jointOrientation;

	jointOrientation = Ogre::Matrix3(xCol.x, -yCol.x, zCol.x,
									 -xCol.y, yCol.y, -zCol.y,
									 xCol.z, -yCol.z, zCol.z);

	return jointOrientation;
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::makeMatrixFromX(Ogre::Vector3 v1)
{
	// matrix column
	Ogre::Vector3 xCol;
	Ogre::Vector3 yCol;
	Ogre::Vector3 zCol;

	Ogre::Vector3 v1Normalized = v1.normalisedCopy();

	xCol = v1Normalized;

	yCol.x = 0.0f;
	yCol.y = xCol.z;
	yCol.x = -xCol.y;

	yCol.normalise();

	zCol = xCol.crossProduct(yCol);

	return this->populateMatrix(xCol, yCol, zCol);
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::makeMatrixFromY(Ogre::Vector3 v1)
{
	// matrix column
	Ogre::Vector3 xCol;
	Ogre::Vector3 yCol;
	Ogre::Vector3 zCol;

	Ogre::Vector3 v1Normalized = v1.normalisedCopy();
	yCol = v1Normalized;

	xCol.x = yCol.y;
	xCol.y = -yCol.x;
	xCol.z = 0.0f;

	xCol.normalise();

	zCol = xCol.crossProduct(yCol);

	return this->populateMatrix(xCol, yCol, zCol);
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::makeMatrixFromZ(Ogre::Vector3 v1)
{
	// matrix column
	Ogre::Vector3 xCol;
	Ogre::Vector3 yCol;
	Ogre::Vector3 zCol;

	Ogre::Vector3 v1Normalized = v1.normalisedCopy();
	zCol = v1Normalized;

	xCol.x = zCol.y;
	xCol.y = -zCol.x;
	xCol.z = 0.0f;

	xCol.normalise();

	yCol = zCol.crossProduct(xCol);

	return this->populateMatrix(xCol, yCol, zCol);
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::makeMatrixFromXY(Ogre::Vector3 xUnnormalized, Ogre::Vector3 yUnnormalized)
{
	// matrix column
	Ogre::Vector3 xCol;
	Ogre::Vector3 yCol;
	Ogre::Vector3 zCol;

	Ogre::Vector3 xn = xUnnormalized.normalisedCopy();
	Ogre::Vector3 yn = yUnnormalized.normalisedCopy();

	xCol = xn;
	zCol = xCol.crossProduct(yn).normalise();
	yCol = zCol.crossProduct(xCol);

	return this->populateMatrix(xCol, yCol, zCol);
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::makeMatrixFromYX(Ogre::Vector3 xUnnormalized, Ogre::Vector3 yUnnormalized)
{
	// matrix column
	Ogre::Vector3 xCol;
	Ogre::Vector3 yCol;
	Ogre::Vector3 zCol;

	Ogre::Vector3 xn = xUnnormalized.normalisedCopy();
	Ogre::Vector3 yn = yUnnormalized.normalisedCopy();

	yCol = yn;
	zCol = xn.crossProduct(yCol).normalise();
	xCol = yCol.crossProduct(zCol);

	return this->populateMatrix(xCol, yCol, zCol);
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::makeMatrixFromYZ(Ogre::Vector3 yUnnormalized, Ogre::Vector3 zUnnormalized)
{
	// matrix column
	Ogre::Vector3 xCol;
	Ogre::Vector3 yCol;
	Ogre::Vector3 zCol;

	Ogre::Vector3 yn = yUnnormalized.normalisedCopy();
	Ogre::Vector3 zn = zUnnormalized.normalisedCopy();

	yCol = yn;
	xCol = yCol.crossProduct(zn).normalise();
	zCol = xCol.crossProduct(yCol);

	return this->populateMatrix(xCol, yCol, zCol);
}

//-------------------------------------------------------------------------------------
Ogre::Matrix3 NuiManager::JointOrientationCalculator::getSkeletonJointOrientation(NuiManager::NuiJointIndex idx)
{
	Ogre::Matrix3 orientationMatrix;
	
	Ogre::Vector3 vx;
	Ogre::Vector3 vy;
	Ogre::Vector3 vz;

	switch(idx)
	{
	case NuiJointIndex::HIP_CENTER:			/*0*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::HIP_CENTER,		NuiJointIndex::SPINE);
		vx = this->getPositionBetweenIndices(NuiJointIndex::HIP_LEFT,		NuiJointIndex::HIP_RIGHT);
		orientationMatrix = this->makeMatrixFromYX(vx, vy);
		break;

	case NuiJointIndex::SPINE:				/*1*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::SPINE,			NuiJointIndex::SHOULDER_CENTER);
		vx = this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_LEFT,	NuiJointIndex::SHOULDER_RIGHT);
		orientationMatrix = this->makeMatrixFromYX(vx, vy);
		break;

	case NuiJointIndex::SHOULDER_CENTER:	/*2*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_CENTER,NuiJointIndex::HEAD);
		vx = this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_LEFT,	NuiJointIndex::SHOULDER_RIGHT);
		orientationMatrix = this->makeMatrixFromYX(vx, vy);
		break;

	case NuiJointIndex::HEAD:				/*3*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_CENTER,NuiJointIndex::HEAD);
		orientationMatrix = this->makeMatrixFromY(vy);
		break;

	case NuiJointIndex::SHOULDER_LEFT:		/*4*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::ELBOW_LEFT,		NuiJointIndex::WRIST_LEFT);
		vx = -this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_LEFT,	NuiJointIndex::ELBOW_LEFT);
		orientationMatrix = this->makeMatrixFromXY(vx, vy);
		break;

	case NuiJointIndex::ELBOW_LEFT:			/*5*/
		vy = -this->getPositionBetweenIndices(NuiJointIndex::ELBOW_LEFT,	NuiJointIndex::WRIST_LEFT);
		vx = -this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_LEFT,	NuiJointIndex::ELBOW_LEFT);
		orientationMatrix = this->makeMatrixFromXY(vx, vy);
		break;

	case NuiJointIndex::WRIST_LEFT:			/*6*/
		vx = -this->getPositionBetweenIndices(NuiJointIndex::WRIST_LEFT,	NuiJointIndex::HAND_LEFT);
		orientationMatrix = this->makeMatrixFromX(vx);
		break;

	case NuiJointIndex::HAND_LEFT:			/*7*/
		vx = -this->getPositionBetweenIndices(NuiJointIndex::WRIST_LEFT, NuiJointIndex::HAND_LEFT);
		orientationMatrix = this->makeMatrixFromX(vx);
		break;

	case NuiJointIndex::HIP_LEFT:			/*8*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::KNEE_LEFT,	NuiJointIndex::HIP_LEFT);
		vx = this->getPositionBetweenIndices(NuiJointIndex::HIP_LEFT,	NuiJointIndex::HIP_RIGHT);
		orientationMatrix = this->makeMatrixFromYX(vx, vy);
		break;

	case NuiJointIndex::KNEE_LEFT:			/*13*/
		vy = -this->getPositionBetweenIndices(NuiJointIndex::KNEE_LEFT,	NuiJointIndex::ANKLE_LEFT);
		vz = -this->getPositionBetweenIndices(NuiJointIndex::ANKLE_LEFT,	NuiJointIndex::FOOT_LEFT);
		orientationMatrix = this->makeMatrixFromYZ(vy, vz);
		break;

	case NuiJointIndex::ANKLE_LEFT:			/*14*/
		vz = this->getPositionBetweenIndices(NuiJointIndex::FOOT_LEFT, NuiJointIndex::ANKLE_LEFT);
		orientationMatrix = this->makeMatrixFromZ(vz);
		break;

	case NuiJointIndex::FOOT_LEFT:			/*15*/
		vz = this->getPositionBetweenIndices(NuiJointIndex::FOOT_LEFT, NuiJointIndex::ANKLE_LEFT);
		orientationMatrix = this->makeMatrixFromZ(vz);
		break;

	case NuiJointIndex::SHOULDER_RIGHT:		/*8*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::ELBOW_RIGHT,	NuiJointIndex::WRIST_RIGHT);
		vx = this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_RIGHT,	NuiJointIndex::ELBOW_RIGHT);
		orientationMatrix = this->makeMatrixFromYX(vx, vy);
		break;

	case NuiJointIndex::ELBOW_RIGHT:		/*9*/
		vx = this->getPositionBetweenIndices(NuiJointIndex::ELBOW_RIGHT,	NuiJointIndex::WRIST_RIGHT);
		vy = -this->getPositionBetweenIndices(NuiJointIndex::SHOULDER_RIGHT,	NuiJointIndex::ELBOW_RIGHT);
		orientationMatrix = this->makeMatrixFromXY(vx, vy);
		break;

	case NuiJointIndex::WRIST_RIGHT:		/*10*/
		vx = this->getPositionBetweenIndices(NuiJointIndex::WRIST_RIGHT, NuiJointIndex::HAND_RIGHT);
		orientationMatrix = this->makeMatrixFromX(vx);
		break;

	case NuiJointIndex::HAND_RIGHT:			/*11*/
		vx = this->getPositionBetweenIndices(NuiJointIndex::WRIST_RIGHT, NuiJointIndex::HAND_RIGHT);
		orientationMatrix = this->makeMatrixFromX(vx);
		break;
		break;

	case NuiJointIndex::HIP_RIGHT:			/*16*/
		vy = this->getPositionBetweenIndices(NuiJointIndex::KNEE_RIGHT,	NuiJointIndex::HIP_RIGHT);
		vx = this->getPositionBetweenIndices(NuiJointIndex::HIP_LEFT,	NuiJointIndex::HIP_RIGHT);
		orientationMatrix = this->makeMatrixFromYX(vx, vy);
		break;

	case NuiJointIndex::KNEE_RIGHT:			/*17*/
		vy = -this->getPositionBetweenIndices(NuiJointIndex::KNEE_RIGHT,	NuiJointIndex::ANKLE_RIGHT);
		vz = -this->getPositionBetweenIndices(NuiJointIndex::ANKLE_RIGHT,	NuiJointIndex::FOOT_RIGHT);
		orientationMatrix = this->makeMatrixFromYZ(vy, vz);
		break;

	case NuiJointIndex::ANKLE_RIGHT:		/*18*/
		vz = this->getPositionBetweenIndices(NuiJointIndex::FOOT_RIGHT, NuiJointIndex::ANKLE_RIGHT);
		orientationMatrix = this->makeMatrixFromZ(vz);
		break;

	case NuiJointIndex::FOOT_RIGHT:			/*19*/
		vz = this->getPositionBetweenIndices(NuiJointIndex::FOOT_RIGHT, NuiJointIndex::ANKLE_RIGHT);
		orientationMatrix = this->makeMatrixFromZ(vz);
		break;

	}

	return orientationMatrix;
}