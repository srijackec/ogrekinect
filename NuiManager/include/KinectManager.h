
#ifndef __KinectSkeleton_h__
#define __KinectSkeleton_h__

#include "NuiApi.h"

namespace NuiManager 
{
	class KinectManager
	{
	public:
		KinectManager(void);
		virtual ~KinectManager(void);

		HRESULT InitNui(void);
		void UnInitNui(void);
		size_t getDeviceCount(void);

	protected:
		INuiSensor*            nuiSensor;
		BSTR                   instanceId;

	private:
		//HANDLE        m_hThNuiProcess;
		//HANDLE        m_hEvNuiProcessStop;

		HANDLE        hNextDepthFrameEvent;
		HANDLE        hNextColorFrameEvent;
		HANDLE        hNextSkeletonEvent;
		HANDLE        pDepthStreamHandle;
		HANDLE        pVideoStreamHandle;

	};
}

#endif