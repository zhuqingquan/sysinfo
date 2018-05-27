/**
 *	@brief	sysinfo库中包含一些用于获取当前运行机器信息的api
 **/

#ifndef _TOOLS_SYS_INFO_H_
#define _TOOLS_SYS_INFO_H_

#ifdef SYS_INFO_LIB_EXPORT
#define SYS_INFO_EXPORT __declspec(dllexport)
#else
#define SYS_INFO_EXPORT __declspec(dllimport)
#endif

/***
 * @name	sysinfo_getOSVersion
 * @brief	获取当前机器的系统版本号
 * @param[in,out]	char* strOsVersion 保存获取到的版本号字符串，in--提供保存版本号的内存，out--存放获取到的系统版本号
 *					字符格式为 <主版本号>.<次版本号>.<build版本号> SP<SP版本号>.<SP次版本号>
 * @param[in,out]	size_t& length 保存获取到的版本号字符串长度，in--提供的内存的长度，out--系统版本号长度
 * @return			size_t 系统版本号字符串长度
 **/
SYS_INFO_EXPORT size_t sysinfo_getOSVersion(char* strOsVersion, size_t& length);

/***
 * @name			sysinfo_getDXLevel
 * @brief			获取当前机器的支持的DirectX的最高Level，比如11_1, 11_0, 9_0
 * @param[in,out]	char* strOsVersion 保存获取到DirectX的Level字符串，in--提供保存DirectX的Level的内存，out--存放获取到的DirectX的Level
 *					字符串格式为: 11_1 或者 9_0。前面的数字代表DirectX的主要级别，后面的数字为小级别
 * @param[in,out]	size_t& length 保存获取到的DirectX的Level字符串长度，in--提供的内存的长度，out--DirectX的Level长度
 * @return			size_t DirectX的Level字符串长度
 **/
SYS_INFO_EXPORT size_t sysinfo_getDXLevel(char* strDxLevel, size_t& length);

typedef struct _GPUInfo
{
	char gpu[256];	//GPU名称、型号
	char ver[256];	//GPU驱动版本号
}GPUInfo;

#endif //_TOOLS_SYS_INFO_H_