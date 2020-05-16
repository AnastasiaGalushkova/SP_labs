#include <stdio.h>
#include "windows.h"

int main()
{
	TCHAR sMutexName[]{ L"OneInstanceMutex" };
	HANDLE hOneInstMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, sMutexName);
	if (!hOneInstMutex)
	{
		hOneInstMutex = CreateMutex(NULL, TRUE, sMutexName);
		wprintf(L"Is Running");
		while (1) {}
	}
	else
	{
		wprintf(L"Only 1 app instance available \n");
		Sleep(5000);
	}
}