#include <intrin.h>

#include "d3d9.h"
#include "VsdkInvokerEvent.h"
#include "miniyy/miniyy.h"

typedef IDirect3D9* (WINAPI *FnDirect3DCreate9)(UINT SDKVersion);
#define SAFE_FREE_LIBRARY(x) if(x) { ::FreeLibrary(x); x = NULL; }
#define ASAFE_RELEASE_POINTER(x) { if (x) x->Release(); x = NULL; }


class LocalHardWare
{
public:
	static vector<int> getLocalEncodeAbility()
	{
		vector<int> vecEncoder;

		ETGetEncoderList evt( vecEncoder );
		getMiniYY()->invoke(&evt);
		return vecEncoder;
	}

	static map<QString,QString> getGPUInfo()
	{
		map<QString,QString> mapInfo;

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

		return mapInfo;
	}

	static QString getGPUList()
	{
		QString strGPUList;
		map<QString,QString> mapGPU = getGPUInfo();
		for ( map<QString,QString>::const_iterator iter = mapGPU.begin();iter != mapGPU.end();++ iter )
		{
			if( strGPUList.isEmpty() )
				strGPUList += iter->first;
			else
				strGPUList += QString(";%1").arg( iter->first );
		}
		return strGPUList;
	}

	typedef BOOL (WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
	static int getCoreCnt()
	{
		LPFN_GLPI glpi = NULL;
		BOOL done = FALSE;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
		DWORD returnLength = 0;
		DWORD logicalProcessorCount = 0;
		DWORD numaNodeCount = 0;
		DWORD processorCoreCount = 0;
		DWORD byteOffset = 0;

		glpi = (LPFN_GLPI) GetProcAddress(
			GetModuleHandle(TEXT("kernel32")),
			"GetLogicalProcessorInformation");
		if (NULL == glpi) 
		{
			return (-1);
		}

		while (!done)
		{
			DWORD rc = glpi(buffer, &returnLength);

			if (FALSE == rc) 
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
				{
					if (buffer) 
						free(buffer);

					buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
						returnLength);

					if (NULL == buffer) 
					{
						return (-2);
					}
				} 
				else 
				{
					return (-3);
				}
			} 
			else
			{
				done = TRUE;
			}
		}

		ptr = buffer;

		while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength) 
		{
			switch (ptr->Relationship) 
			{
			case RelationNumaNode:
				numaNodeCount++;
				break;

			case RelationProcessorCore:
				processorCoreCount++;
				logicalProcessorCount += countSetBits(ptr->ProcessorMask);
				break;
			default:
				break;
			}
			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			ptr++;
		}
		free(buffer);
		return processorCoreCount;
	}

	static DWORD countSetBits(ULONG_PTR bitMask)
	{
		DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
		DWORD bitSetCount = 0;
		ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;    
		DWORD i;

		for (i = 0; i <= LSHIFT; ++i)
		{
			bitSetCount += ((bitMask & bitTest)?1:0);
			bitTest/=2;
		}

		return bitSetCount;
	}

	static void getCPUName(QString& strCPUName)
	{
		int cpuInfo[4] = {-1};
		__cpuid(cpuInfo, 0x80000000);
		int nExIds_ = cpuInfo[0];
		char szInfo[17] = {0};
		for (int i = 0x80000000; i <= nExIds_; ++i)
		{
			__cpuid(cpuInfo, i);
			if (i == 0x80000002 || i == 0x80000003 || i == 0x80000004)
			{
				memcpy_s((void*)szInfo,16,(void*)cpuInfo,16);
				strCPUName.append(szInfo);
			}
		}
	}
};