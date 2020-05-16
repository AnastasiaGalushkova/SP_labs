#include "Main.h"

using namespace std;

int main(int argc, char* argv[])
{
	clock_t start, end;
	InitializeCriticalSection(&gCriticalSection);
	threadHandles = (HANDLE*)malloc(sizeof(HANDLE) * COUNT_THREADS);
	GenereteArrayNums(arrayForCalculating);
	cout << "\t START CALCULATING WITH CRITICAL SECTION: \n";
	start = clock();
	for (int i = 0; i < COUNT_THREADS; i++)
	{
		threadHandles[i] = CreateThread(NULL,
			NULL,
			StartNewThreadWithCriticalSection,
			(LPVOID)i,
			NULL,
			NULL);
	}
	WaitForMultipleObjects(COUNT_THREADS, threadHandles, TRUE, INFINITE);
	end = clock();
	cout << "Calculating with critical section continued for " << end - start << " ms.\n";
	cout << "..........................................................\n";
	cout << "\t START CALCULATING WITHOUT CRITICAL SECTION: \n";
	start = clock();
	for (int i = 0; i < COUNT_THREADS; i++)
	{
		threadHandles[i] = CreateThread(NULL,
			NULL,
			StartNewThreadWithoutCriticalSection,
			LPVOID(i),
			NULL,
			NULL);
	}
	WaitForMultipleObjects(COUNT_THREADS, threadHandles, TRUE, INFINITE);
	end = clock();
	cout << "Calculating without critical section continued for " << end - start << " ms.\n";
	for (int i = 0; i < COUNT_THREADS; i++)
		CloseHandle(threadHandles[i]);
	DeleteCriticalSection(&gCriticalSection);
	return 0;
}
DWORD WINAPI StartNewThreadWithCriticalSection(LPVOID param)
{
	switch ((int)param)
	{
	case SUM_OPERATION:
	{
		EnterCriticalSection(&gCriticalSection);
		ArrNumsSum(arrayForCalculating);
		LeaveCriticalSection(&gCriticalSection);
	} break;
	case BUBBLE_SORT_OPERATION:
	{
		EnterCriticalSection(&gCriticalSection);
		BubbleSort(arrayForCalculating);
		LeaveCriticalSection(&gCriticalSection);
	} break;
	case AVARAGE_OPERATION:
	{
		EnterCriticalSection(&gCriticalSection);
		CalcAvarageValue(arrayForCalculating);
		LeaveCriticalSection(&gCriticalSection);
	} break;
	default:
		break;
	}
	return 0;
}
DWORD WINAPI StartNewThreadWithoutCriticalSection(LPVOID param)
{
	switch ((int)param)
	{
	case SUM_OPERATION:
	{
		ArrNumsSum(arrayForCalculating);
	} break;
	case BUBBLE_SORT_OPERATION:
	{
		BubbleSort(arrayForCalculating);
	} break;
	case AVARAGE_OPERATION: 
	{
		CalcAvarageValue(arrayForCalculating);
	} break;
	default:
		break;
	}
	return 0;
}
void GenereteArrayNums(int* arr)
{
	DWORD randRange = ARRAY_NUM_MAX_BOUNDARY - ARRAY_NUM_MIN_BOUNDARY;
	for (DWORD i = 0; i < ARRAY_SIZE; i++)
	{
		arr[i] = (rand() % randRange) + ARRAY_NUM_MIN_BOUNDARY;
	}
}
LONG64 CalcAvarageValue(int* arr)
{
	LONG64 res = 0;
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		res += arr[i];
	}
	res /= ARRAY_SIZE;
	cout << "Avarage: " << res << endl;
	return res;
}
void BubbleSort(int* arr)
{

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; i < ARRAY_SIZE - i - 1; i++)
		{
			if (arr[j] > arr[j + 1]) swap(arr[j], arr[j]); 
		}
	}
	cout << "First number: " << arr[0] << "; Last number: " << arr[ARRAY_SIZE - 1] << endl;
	return;
}
int ArrNumsSum(int* arr)
{
	int sum = arr[0];
	for (int i = 1; i < ARRAY_SIZE; i++)
	{
		sum += arr[i];
	}
	cout << "Sum: " << sum << endl;;
	return sum;
}
