#include <stdio.h>
#include "windows.h"
#include "iostream"
#include "tchar.h"
#include "processthreadsapi.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

using namespace std;

typedef struct {
	TCHAR    achKey[MAX_KEY_LENGTH];  
	DWORD    cbName;                  
	TCHAR    achClass[MAX_PATH] = TEXT("");  
	DWORD    cchClassName = MAX_PATH;  
	DWORD    cSubKeys = 0;             
	DWORD    cbMaxSubKey;              
	DWORD    cchMaxClass;              
	DWORD    cValues;              
	DWORD    cchMaxValue;          
	DWORD    cbMaxValueData;       
	DWORD    cbSecurityDescriptor; 
	FILETIME ftLastWriteTime;      
} KEY_INFO, * pKEY_INFO;

void PrintMenu();
bool OpenKey(HKEY** hKey, DWORD dwOpenAccess, LPSTR fullPath);
void ReadStringWithWhitespaces(CHAR sBuffNewPath[], DWORD maxBuffSize, BOOL isUsedBeforeInputChar);
bool GetKeyInfo(HKEY key, KEY_INFO* keyInfo);
void PrintListSubkeysByKey(HKEY key);
void PrintListParamsByKey(HKEY key);
bool FindStringInReg(HKEY hKey, LPCSTR reqStr, LPSTR fullPath);
bool SaveKeyIntoFile(HKEY hKey);
BOOL SetPrivilege(
	HANDLE hToken,          
	LPCTSTR lpszPrivilege,  
	BOOL bEnablePrivilege   
);