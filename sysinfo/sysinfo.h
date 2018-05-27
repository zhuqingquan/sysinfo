/**
 *	@brief	sysinfo���а���һЩ���ڻ�ȡ��ǰ���л�����Ϣ��api
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
 * @brief	��ȡ��ǰ������ϵͳ�汾��
 * @param[in,out]	char* strOsVersion �����ȡ���İ汾���ַ�����in--�ṩ����汾�ŵ��ڴ棬out--��Ż�ȡ����ϵͳ�汾��
 *					�ַ���ʽΪ <���汾��>.<�ΰ汾��>.<build�汾��> SP<SP�汾��>.<SP�ΰ汾��>
 * @param[in,out]	size_t& length �����ȡ���İ汾���ַ������ȣ�in--�ṩ���ڴ�ĳ��ȣ�out--ϵͳ�汾�ų���
 * @return			size_t ϵͳ�汾���ַ�������
 **/
SYS_INFO_EXPORT size_t sysinfo_getOSVersion(char* strOsVersion, size_t& length);

/***
 * @name			sysinfo_getDXLevel
 * @brief			��ȡ��ǰ������֧�ֵ�DirectX�����Level������11_1, 11_0, 9_0
 * @param[in,out]	char* strOsVersion �����ȡ��DirectX��Level�ַ�����in--�ṩ����DirectX��Level���ڴ棬out--��Ż�ȡ����DirectX��Level
 *					�ַ�����ʽΪ: 11_1 ���� 9_0��ǰ������ִ���DirectX����Ҫ���𣬺��������ΪС����
 * @param[in,out]	size_t& length �����ȡ����DirectX��Level�ַ������ȣ�in--�ṩ���ڴ�ĳ��ȣ�out--DirectX��Level����
 * @return			size_t DirectX��Level�ַ�������
 **/
SYS_INFO_EXPORT size_t sysinfo_getDXLevel(char* strDxLevel, size_t& length);

typedef struct _GPUInfo
{
	char gpu[256];	//GPU���ơ��ͺ�
	char ver[256];	//GPU�����汾��
}GPUInfo;

#endif //_TOOLS_SYS_INFO_H_