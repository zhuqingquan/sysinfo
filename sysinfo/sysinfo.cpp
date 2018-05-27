#include "sysinfo.h"
#include <Windows.h>
#include <stdio.h>
#include "d3d11.h"
#include "d3d9.h"

#define SAFE_FREE_LIBRARY(x) if(x) { ::FreeLibrary(x); x = NULL; }
#define ASAFE_RELEASE_POINTER(x) { if (x) x->Release(); x = NULL; }

size_t sysinfo_getOSVersion(char* strOsVersion, size_t& length)
{
	if(strOsVersion==NULL || length<=0)
		return 0;
	OSVERSIONINFOEX osVersionInfo;
	ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(0>GetVersionEx((OSVERSIONINFO*)&osVersionInfo))
	{
		return 0;
	}
	char tmp[128] = {0};
	sprintf_s(tmp, 128, "%d.%d.%d sp %d.%d", osVersionInfo.dwMajorVersion, osVersionInfo.dwMinorVersion, osVersionInfo.dwBuildNumber, osVersionInfo.wServicePackMajor, osVersionInfo.wServicePackMinor);
	size_t tmpLen = strlen(tmp);
	size_t copyedLen = length >= tmpLen ? tmpLen : length;
	strncpy_s(strOsVersion, length, tmp, copyedLen);
	return copyedLen;
}

typedef IDirect3D9* (WINAPI *FnDirect3DCreate9)(UINT SDKVersion);
typedef HRESULT (WINAPI *FnD3D11CreateDevice)(
								 __in_opt IDXGIAdapter* pAdapter,
								 D3D_DRIVER_TYPE DriverType,
								 HMODULE Software,
								 UINT Flags,
								 __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
								 UINT FeatureLevels,
								 UINT SDKVersion,
								 __out_opt ID3D11Device** ppDevice,
								 __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
								 __out_opt ID3D11DeviceContext** ppImmediateContext );

size_t sysinfo_getDXLevel(char* strDxLevel, size_t& length)
{
	if(length<4 || NULL==strDxLevel)
		return 0;

	HMODULE hD3D11Module = NULL;
	hD3D11Module = ::LoadLibraryA("d3d11.dll");
	if( !hD3D11Module )	return 0;
	FnD3D11CreateDevice fnD3D11CreateDevice = NULL;
	fnD3D11CreateDevice = (FnD3D11CreateDevice)GetProcAddress(hD3D11Module, "D3D11CreateDevice");
	if( !fnD3D11CreateDevice ) 
	{
		SAFE_FREE_LIBRARY(hD3D11Module);
		return 0;
	}

	UINT flags = 0;
	D3D_FEATURE_LEVEL feaLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	D3D_FEATURE_LEVEL featureLv = D3D_FEATURE_LEVEL_9_1;
	HRESULT rsult = fnD3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, feaLevels, ARRAYSIZE(feaLevels), D3D11_SDK_VERSION, &pDevice, &featureLv, &pContext);
	
	if(FAILED(rsult) || pDevice==NULL || NULL==pContext)
	{
		SAFE_FREE_LIBRARY(hD3D11Module);
		return 0;
	}
	ASAFE_RELEASE_POINTER(pContext);
	ASAFE_RELEASE_POINTER(pDevice);
	SAFE_FREE_LIBRARY(hD3D11Module);
	char* resultLevel = NULL;
	size_t resultLevelLen = 0;
	switch(featureLv)
	{
	case D3D_FEATURE_LEVEL_11_0:
		resultLevel = "11_0";
		resultLevelLen = strlen(resultLevel);
		strncpy_s(strDxLevel, length, resultLevel, resultLevelLen);
		length = resultLevelLen;
		break;
	case D3D_FEATURE_LEVEL_10_1:
		resultLevel = "10_1";
		resultLevelLen = strlen(resultLevel);
		strncpy_s(strDxLevel, length, resultLevel, resultLevelLen);
		length = resultLevelLen;
		break;
	case D3D_FEATURE_LEVEL_10_0:
		resultLevel = "01_0";
		resultLevelLen = strlen(resultLevel);
		strncpy_s(strDxLevel, length, resultLevel, resultLevelLen);
		length = resultLevelLen;
		break;
	case D3D_FEATURE_LEVEL_9_3:
		resultLevel = "9_3";
		resultLevelLen = strlen(resultLevel);
		strncpy_s(strDxLevel, length, resultLevel, resultLevelLen);
		length = resultLevelLen;
		break;
	case D3D_FEATURE_LEVEL_9_2:
		resultLevel = "9_2";
		resultLevelLen = strlen(resultLevel);
		strncpy_s(strDxLevel, length, resultLevel, resultLevelLen);
		length = resultLevelLen;
		break;
	case D3D_FEATURE_LEVEL_9_1:
		resultLevel = "9_1";
		resultLevelLen = strlen(resultLevel);
		strncpy_s(strDxLevel, length, resultLevel, resultLevelLen);
		length = resultLevelLen;
		break;
	default:
		return 0;
	}
	return length;
}

//static map<QString,QString> getGPUInfo()
//{
/*	map<QString,QString> mapInfo;

	HMODULE hD3DModule = NULL;
	FnDirect3DCreate9 fnDirect3DCreate9 = NULL;

	hD3DModule = ::LoadLibraryA("d3d9.dll");
	if( !hD3DModule )	return mapInfo;

	fnDirect3DCreate9 = (FnDirect3DCreate9)GetProcAddress(hD3DModule, "Direct3DCreate9");
	if( !fnDirect3DCreate9 ) 
	{
		SAFE_FREE_LIBRARY(hD3DModule);
		return mapInfo;
	}

	LPDIRECT3D9 pDev = fnDirect3DCreate9(D3D_SDK_VERSION);
	if( pDev == NULL )
	{
		SAFE_FREE_LIBRARY(hD3DModule);
		return mapInfo;
	}

	UINT numAdapters = pDev->GetAdapterCount();
	for( UINT adapterOrdinal = 0; adapterOrdinal < numAdapters; adapterOrdinal++ )
	{
		D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
		pDev->GetAdapterIdentifier( adapterOrdinal, 0, &AdapterIdentifier);

		WORD Product = HIWORD(AdapterIdentifier.DriverVersion.HighPart);
		WORD Version = LOWORD(AdapterIdentifier.DriverVersion.HighPart);
		WORD SubVersion = HIWORD(AdapterIdentifier.DriverVersion.LowPart);
		WORD Build = LOWORD(AdapterIdentifier.DriverVersion.LowPart);

		mapInfo.insert( make_pair<QString,QString>( QString("%1").arg(AdapterIdentifier.Description),QString("%1.%2.%3.%4").arg( Product ).arg( Version ).arg( SubVersion ).arg( Build )  ));
	}

	ASAFE_RELEASE_POINTER(pDev);
	SAFE_FREE_LIBRARY(hD3DModule);

	return mapInfo;*/
//}