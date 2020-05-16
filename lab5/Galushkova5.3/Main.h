#include <stdio.h>
#include "iostream"
#include "windows.h"
#include <ctime>

#define ARRAY_SIZE 10000000
#define ARRAY_NUM_MIN_BOUNDARY 10 
#define ARRAY_NUM_MAX_BOUNDARY 100000 
#define SUM_OPERATION 0
#define BUBBLE_SORT_OPERATION 1
#define AVARAGE_OPERATION 2

DWORD WINAPI StartNewThreadWithCriticalSection(LPVOID param);
DWORD WINAPI StartNewThreadWithoutCriticalSection(LPVOID param);
void GenereteArrayNums(int* arr);
int ArrNumsSum(int* arr); 
void BubbleSort(int* arr);
LONG64 CalcAvarageValue(int* arr);
CRITICAL_SECTION gCriticalSection; 
HANDLE* threadHandles;
CONST DWORD COUNT_THREADS = 3;
int arrayForCalculating[ARRAY_SIZE]{ 0 };