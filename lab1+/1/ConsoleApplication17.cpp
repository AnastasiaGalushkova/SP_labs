// ConsoleApplication17.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "windows.h"
#include "stdio.h"

using namespace std;
void causeError();
void printErrorMessage(HRESULT hResult = NULL);
void printSystemInfo();
int main(int argc, char** argv)
{

	setlocale(LC_ALL, "Russian");

	if (argc != 2)
	{
		printf("\n");
	}
	else if (!strcmp(argv[1], "-e"))
	{
		causeError();
		printErrorMessage();
	}
	else if (!strcmp(argv[1], "-s"))
	{
		printSystemInfo();
	}
	else
	{
		printf("Invalid arguments\n");
	}
	return 0;
}
void causeError()
{
	CreateFile(L"", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}
void printErrorMessage(HRESULT hResult)
{
	if (hResult == NULL)
	{
		hResult = GetLastError();
	}
	LPTSTR errorText = NULL;
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);
	if (NULL != errorText)
	{
		wprintf(L"%s", errorText);
		LocalFree(errorText);
	}
}

void printSystemInfo()
{
	printf("=-Processor info-=\n");
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	CHAR szProcArch[21];
	switch (sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		strcpy_s(szProcArch, "x64 (AMD or Intel)");
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		strcpy_s(szProcArch, "ARM");
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		strcpy_s(szProcArch, "Intel Itanium-based");
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		strcpy_s(szProcArch, "x86");
		break;
	default:
		strcpy_s(szProcArch, "Unknown");
		break;
	}
	printf("Processor architecture: %s\n", szProcArch);
	printf("Processor level: %u\n", sysInfo.wProcessorLevel);
	printf("Page size: %u\n", sysInfo.dwPageSize);
	printf("Number of processors: %u\n", sysInfo.dwNumberOfProcessors);
	printf("Active processors: ");
	for (unsigned int i = 0; i < sysInfo.dwNumberOfProcessors; i++)
	{
		if ((sysInfo.dwActiveProcessorMask >> i) & 0x00000001)
		{
			printf("%u", i);
		}
		if (i < sysInfo.dwNumberOfProcessors - 1)
		{
			printf(", ");
		}
	}
	printf("\n\n");
	printf("Granularity: %u\n", sysInfo.dwAllocationGranularity);
	printf("=-Memory status-=\n");
	MEMORYSTATUSEX lpMemStat;
	lpMemStat.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&lpMemStat);
	printf("Memory load: %u%%\n", lpMemStat.dwMemoryLoad);
	printf("Total: %llu MB (%llu bytes)\n", lpMemStat.ullTotalPhys / 0x100000, lpMemStat.ullTotalPhys);
	printf("Available: %llu MB (%llu bytes)\n", lpMemStat.ullAvailPhys / 0x100000, lpMemStat.ullAvailPhys);
}
