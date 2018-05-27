// sysinfo_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "sysinfo.h"

using namespace std;

BOOL Is_WinXP_SP2_or_Later () 
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	int op=VER_GREATER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osvi.dwMajorVersion = 6;
	osvi.dwMinorVersion = 3;
	osvi.wServicePackMajor = 0;
	osvi.wServicePackMinor = 0;

	// Initialize the condition mask.

	VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, op );
	VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, op );
	VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
	VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMINOR, op );

	// Perform the test.

	return VerifyVersionInfo(
		&osvi, 
		VER_MAJORVERSION | VER_MINORVERSION | 
		VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
		dwlConditionMask);
}

void _checkOperatingSystemVersion()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindows7orLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	bIsWindows7orLater = 
		( (osvi.dwMajorVersion > 6) ||
		( (osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 1) ));

	if(bIsWindows7orLater==false)
	{
		std::cout << "不是win7" << std::endl;
	}
	else
	{
		std::cout << "是win7" << std::endl;
	}
}

size_t sysinfo_getOSVersion_2(char* strOsVersion, size_t& length)
{
	if(strOsVersion==NULL || length<=0)
		return 0;
	OSVERSIONINFO osVersionInfo;
	ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFO));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(0>GetVersionEx((OSVERSIONINFO*)&osVersionInfo))
	{
		return 0;
	}
	char tmp[128] = {0};
	sprintf_s(tmp, 128, "%d.%d.%d", osVersionInfo.dwMajorVersion, osVersionInfo.dwMinorVersion, osVersionInfo.dwBuildNumber);
	size_t tmpLen = strlen(tmp);
	size_t copyedLen = length >= tmpLen ? tmpLen : length;
	strncpy_s(strOsVersion, length, tmp, copyedLen);
	return copyedLen;
}

int _tmain(int argc, _TCHAR* argv[])
{
	BOOL result = Is_WinXP_SP2_or_Later();
	char sysVersion[512] = {0};
	size_t strVerLen = 512;
	if(0!=sysinfo_getOSVersion(sysVersion, strVerLen))
	{
		cout << "EX: 操作系统版本号：" << sysVersion << std::endl;
	}
	strVerLen = 512;
	if(0!=sysinfo_getOSVersion_2(sysVersion, strVerLen))
	{
		cout << "操作系统版本号：" << sysVersion << std::endl;
	}
	_checkOperatingSystemVersion();
	system("pause");

	char dxLevel[512] = {0};
	size_t dxLevelLen = 512;
	if(0!=sysinfo_getDXLevel(dxLevel, dxLevelLen))
	{
		cout << "DirectX Level surpported:" << dxLevel << std::endl;
	}
	system("pause");
	return 0;
}
