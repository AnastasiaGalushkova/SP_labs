#include "Main.h"

using namespace std;

int main()
{
	HANDLE* hThreads;
	DWORD dwCountThreads;
	cout << "Count of file handles:" << FILE_HANDLES_COUNT << endl;
	cout << "Input count of threads:\n";
	scanf_s("%d", &dwCountThreads);
	hThreads = (HANDLE*)malloc(sizeof(HANDLE) * dwCountThreads);
	hFilesAccessSemaphore = CreateSemaphore(NULL, FILE_HANDLES_COUNT, FILE_HANDLES_COUNT, L"FilesSemaphore");
	if (!hFilesAccessSemaphore || hFilesAccessSemaphore == INVALID_HANDLE_VALUE)
	{
		cout << "Error! Cant create semaphore\n";
		return 1;
	}
	DWORD dwRandRange = MAX_THREAD_SLEEPING_TIME_IN_SECONDS - MIN_THREAD_SLEEPING_TIME_IN_SECONDS + 1;
	for (int i = 0; i < dwCountThreads; i++)
	{
		srand(clock());
		NewThreaParams param = { 0 };
		param.id = i;
		param.sleepingTime = (rand() % dwRandRange) + MIN_THREAD_SLEEPING_TIME_IN_SECONDS; 
		hThreads[i] = CreateThread(NULL, NULL, StartNewThread, (LPVOID)&param, NULL, NULL); 
		if ((!hThreads[i]) || (hThreads[i] == INVALID_HANDLE_VALUE))
		{
			cout << "Error! Unable to start such thread number: " << i << endl;
			return 2;
		}
		Sleep(100);
	}
	WaitForMultipleObjects(dwCountThreads, hThreads, TRUE, INFINITE);
	CloseHandle(hFilesAccessSemaphore); 
	for (int i = 0; i < dwCountThreads; i++)
		CloseHandle(hThreads[i]);
	free(hThreads);
	CloseHandle(hFilesAccessSemaphore);
	cout << "All threads complete\n";
	getchar();
	return 0;
}
DWORD WINAPI StartNewThread(LPVOID param)
{
	DWORD dwThreadUserId = ((LPNewThreadParams)param)->id;
	DWORD dwThreadUserSleepingTime = ((LPNewThreadParams)param)->sleepingTime;

	HANDLE hFileForSaving;
	DWORD dwWaitingRes = ERROR_SUCCESS;
	clock_t  startTime = clock(), endTime;
	WaitForSingleObject(hFilesAccessSemaphore, INFINITE);
	printf_s("Thread %d get resourse\n", dwThreadUserId);
	printf_s("Thread %d will sleep for %d sec\n", dwThreadUserId, dwThreadUserSleepingTime);
	Sleep(dwThreadUserSleepingTime * 1000); 
	if ((hFileForSaving = CreateFile(ctsFileForSaving,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL)) == INVALID_HANDLE_VALUE)
	{
		cout << "Error from thread " << dwThreadUserId << ", can`t open file\n";
		ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
		ExitThread(1);
	}
	if ((dwWaitingRes = SetFilePointer(hFileForSaving, 0, NULL, FILE_END)) == INVALID_SET_FILE_POINTER)
	{
		cout << "Error from thread " << dwThreadUserId << ", can`t set a file pointer in the file\n";
		ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
		ExitThread(2);
	}
	endTime = clock();
	TCHAR buff[BUFF_FOR_WRITING_TO_LOG_FILE_SIZE] { '\0' };
	DWORD dwWritenBytes;
	_sntprintf_s(buff, BUFF_FOR_WRITING_TO_LOG_FILE_SIZE, L"Thread %d ends in %d seconds\n", dwThreadUserId, ((endTime - startTime) / 1000));
	if (!WriteFile(hFileForSaving, buff, _tcslen(buff)*sizeof(TCHAR) , &dwWritenBytes, NULL))
	{
		cout << "Error from thread " << dwThreadUserId << ", can`t write into the file\n";
		ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
		ExitThread(3);
	}
	CloseHandle(hFileForSaving);
	printf_s("Thread %d complete, bytes writen to file:%d\n", dwThreadUserId, dwWritenBytes);
	ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
	return 0;
}
