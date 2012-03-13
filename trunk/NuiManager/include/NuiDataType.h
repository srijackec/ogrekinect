

#ifndef __NuiDataType_h_
#define __NuiDataType_h_

namespace NuiManager 
{
	//-------------------------------------------------------------------------------------
	typedef enum NuiJointIndex
	{
		HIP_CENTER,
		SPINE,
		SHOULDER_CENTER,
		HEAD,
		SHOULDER_LEFT,
		ELBOW_LEFT,
		WRIST_LEFT,
		HAND_LEFT,
		SHOULDER_RIGHT,
		ELBOW_RIGHT,
		WRIST_RIGHT,
		HAND_RIGHT,
		HIP_LEFT,
		KNEE_LEFT,
		ANKLE_LEFT,
		FOOT_LEFT,
		HIP_RIGHT,
		KNEE_RIGHT,
		ANKLE_RIGHT,
		FOOT_RIGHT
	} NuiJointIndex;

	////-------------------------------------------------------------------------------------
	//typedef enum NuiImageType
	//{
	//	DEPTH_AND_PLAYER_INDEX,
	//	COLOR,
	//	COLOR_YUV,
	//	COLOR_RAW_YUV,
	//	DEPTH
	//} NuiImageType;

	////-------------------------------------------------------------------------------------
	//typedef struct NuiImageViewArea
	//{
	//	int eDigitalZoom;
	//	long lCenterX;
	//	long lCenterY;
	//} NuiImageViewArea;

	////-------------------------------------------------------------------------------------
	//typedef struct NuiTransformSmoothParameters
	//{
	//	float fSmoothing;
	//	float fCorrection;
	//	float fPrediction;
	//	float fJitterRadius;
	//	float fMaxDeviationRadius;
	//} NuiTransformSmoothParameters;

	////-------------------------------------------------------------------------------------
	//typedef struct NuiSurfaceDesc
	//{
	//	uint Width;
	//	uint Height;
	//} NuiSurfaceDesc;

	//-------------------------------------------------------------------------------------
	typedef enum NuiSkeletonPositionTrackingState
	{	NOT_TRACKED = 0,
		INFERRED = 1,
		TRACKED = 2 
	} NuiSkeletonPositionTrackingState;

	////-------------------------------------------------------------------------------------
	//typedef enum NuiSkeletonTrackingState
	//{	NOT_TRACKED,
	//	POSITION_ONLY,
	//	TRACKED 
	//} NuiSkeletonTrackingState;

	////-------------------------------------------------------------------------------------
	//typedef struct NuiSkeletonData
	//{
	//	NuiSkeletonTrackingState eTrackingState;
	//	DWORD dwTrackingID;
	//	DWORD dwEnrollmentIndex;
	//	DWORD dwUserIndex;
	//	Vector4 Position;
	//	Vector4 SkeletonPositions[ 20 ];
	//	NuiSkeletonPositionTrackingState eSkeletonPositionTrackingState[20];
	//	DWORD dwQualityFlags;
	//} NuiSkeletonData;

	//-------------------------------------------------------------------------------------
}

#endif