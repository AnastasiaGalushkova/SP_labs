// ConsoleApplication18.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "windows.h"
#include "stdio.h"
#include<TCHAR.H>

void printErrorMessage(HRESULT hResult = NULL);
LPSTR UnicodeToANSI(LPCWSTR src);
LPWSTR ANSIToUnicode(LPCSTR src);
void ANSIToUnicodeFile(LPCTSTR szSourceFile, LPCTSTR szDestFile);
void UnicodeToANSIFile(LPCTSTR szSourceFile, LPCTSTR szDestFile);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hFileSrc = NULL;
	if (argc != 4)
	{
		printf("Invalid arguments\n");
	}
	else if (!lstrcmp(argv[1], L"-a"))
	{
		ANSIToUnicodeFile(argv[2], argv[3]);
	}
	else if (!lstrcmp(argv[1], L"-u"))
	{
		UnicodeToANSIFile(argv[2], argv[3]);
	}
	else
	{
		printf("Invalid arguments\n");
	}
	return 0;
}

void ANSIToUnicodeFile(LPCTSTR szSourceFile, LPCTSTR szDestFile)
{
	HANDLE hFile;
	if (!(hFile = CreateFile(szSourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		printErrorMessage();
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwBytesRead;
	CHAR* szBuf = new CHAR[dwFileSize + 1]; szBuf[dwFileSize] = '\0';
	ReadFile(hFile, szBuf, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!(hFile = CreateFile(szDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		printErrorMessage();
		return;
	}
	WCHAR* szBufW = ANSIToUnicode(szBuf);
	DWORD dwBytesToWrite = lstrlenW(szBufW) * sizeof(WCHAR);
	DWORD dwBytesWritten;
	WriteFile(hFile, szBufW, dwBytesToWrite, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	delete[] szBuf;
}

void UnicodeToANSIFile(LPCTSTR szSourceFile, LPCTSTR szDestFile)
{
	HANDLE hFile;
	if (!(hFile = CreateFile(szSourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		printErrorMessage();
		return;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwBytesRead;
	WCHAR* szBufW = new WCHAR[dwFileSize / sizeof(WCHAR) + 1]; szBufW[dwFileSize / sizeof(WCHAR)] = 0;
	ReadFile(hFile, szBufW, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);

	if (!(hFile = CreateFile(szDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		printErrorMessage();
		return;
	}
	CHAR* szBuf = UnicodeToANSI(szBufW);
	DWORD dwBytesToWrite = strlen(szBuf);
	DWORD dwBytesWritten;
	WriteFile(hFile, szBuf, dwBytesToWrite, &dwBytesWritten, NULL);
	CloseHandle(hFile);
	delete[] szBuf;
}

LPWSTR ANSIToUnicode(LPCSTR src)
{
	if (!src) return 0;
	int srcLen = strlen(src);

	wchar_t* w = new wchar_t[srcLen + 1];
	w[srcLen] = 0;

	int retval = MultiByteToWideChar(CP_ACP, 0, src, srcLen, w, srcLen);

	if (!retval)
	{
		delete[] w;
		return 0;
	}

	return w;
}

LPSTR UnicodeToANSI(LPCWSTR src)
{
	if (!src) return 0;

	int srcLen = wcslen(src);

	char *x = new char[srcLen + 1];
	x[srcLen] = 0;

	int retval = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, src, srcLen, x, srcLen, 0, 0);

	if (!retval)
	{
		delete[] x;
		return 0;
	}

	return x;
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
