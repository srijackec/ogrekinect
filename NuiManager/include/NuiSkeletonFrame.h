
#ifndef __NuiSkeletonFrame_h__
#define __NuiSkeletonFrame_h__

#include "NuiApi.h"

namespace NuiManager 
{
	struct NuiSkeletonFrame
	{
		LARGE_INTEGER litTimeStamp;
		DWORD dwFrameNumber;
		DWORD dwFlags;
		Vector4 vFloorClipPlane;
		Vector4 vNormalToGravity;
		NUI_SKELETON_DATA skeletonData[NUI_SKELETON_COUNT];
	};
}

#endif