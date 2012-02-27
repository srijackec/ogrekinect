
#include "Stdafx.h"
#include "KinectManager.h"
#include <mmsystem.h>
#include <assert.h>
#include <strsafe.h>

//-------------------------------------------------------------------------------------
NuiManager::KinectManager::KinectManager(void)
	:nuiSensor(0)
{
}

//-------------------------------------------------------------------------------------
NuiManager::KinectManager::~KinectManager(void)
{	
}

//-------------------------------------------------------------------------------------
HRESULT NuiManager::KinectManager::InitNui(void)
{	
	HRESULT hr;

	if(!nuiSensor)
	{
		HRESULT hr = NuiCreateSensorByIndex(0, &nuiSensor);
		if(FAILED(hr)) return hr;

		//SysFreeString(instanceId);
		instanceId = nuiSensor->NuiDeviceConnectionId();
	}

	hNextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	hNextColorFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	hNextSkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	// Initialize Nui
	DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON |  NUI_INITIALIZE_FLAG_USES_COLOR;
	hr = nuiSensor->NuiInitialize( nuiFlags );

	if ( E_NUI_SKELETAL_ENGINE_BUSY == hr )
	{
		nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH |  NUI_INITIALIZE_FLAG_USES_COLOR;
		hr = nuiSensor->NuiInitialize( nuiFlags) ;
	}

	// Skeleton Tracking
	if(FAILED(hr)) return hr;

	if ( HasSkeletalEngine( nuiSensor ) )
	{
		hr = nuiSensor->NuiSkeletonTrackingEnable( hNextSkeletonEvent, 0 );
		if(FAILED(hr)) return hr;
	}

	// RGB Image
	hr = nuiSensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,
		NUI_IMAGE_RESOLUTION_640x480,
		0,
		2,
		hNextColorFrameEvent,
		&pVideoStreamHandle );

	if(FAILED(hr)) return hr;
		
	// Depth Image
	hr = nuiSensor->NuiImageStreamOpen(
		HasSkeletalEngine(nuiSensor) ? NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX : NUI_IMAGE_TYPE_DEPTH,
		NUI_IMAGE_RESOLUTION_320x240,
		0,
		2,
		hNextDepthFrameEvent,
		&pDepthStreamHandle );

	if(FAILED(hr)) return hr;
	
	// Start the Nui processing thread
	//m_hEvNuiProcessStop = CreateEvent( NULL, FALSE, FALSE, NULL );
	//m_hThNuiProcess = CreateThread( NULL, 0, Nui_ProcessThread, this, 0, NULL );
		
	return hr;
}

//-------------------------------------------------------------------------------------
void NuiManager::KinectManager::UnInitNui(void)
{
	if ( nuiSensor )
	{
		nuiSensor->NuiShutdown( );
	}

	if ( hNextSkeletonEvent && ( hNextSkeletonEvent != INVALID_HANDLE_VALUE ) )
	{
		CloseHandle( hNextSkeletonEvent );
		hNextSkeletonEvent = NULL;
	}

	if ( hNextDepthFrameEvent && ( hNextDepthFrameEvent != INVALID_HANDLE_VALUE ) )
	{
		CloseHandle( hNextDepthFrameEvent );
		hNextDepthFrameEvent = NULL;
	}

	if ( hNextColorFrameEvent && ( hNextColorFrameEvent != INVALID_HANDLE_VALUE ) )
	{
		CloseHandle( hNextColorFrameEvent );
		hNextColorFrameEvent = NULL;
	}

	if ( nuiSensor )
	{
		nuiSensor->Release();
		nuiSensor = NULL;
	}   
}

//-------------------------------------------------------------------------------------
size_t NuiManager::KinectManager::getDeviceCount(void)
{
	int result(0); 
	NuiGetSensorCount(&result);
	
	return (size_t) result;
}