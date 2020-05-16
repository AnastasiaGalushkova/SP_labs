#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "windows.h"
#include <locale.h>
#include <iostream>

using namespace std;

#define ANSI_KEY "-a"
#define UNICODE_KEY "-u"
#define SIZE_BUFF_FOR_READING_FILE 1024

void ANSI_To_Unicode(LPCSTR);
void Unicode_To_ANSI(LPCSTR);
void ShowError();
bool ConcatFileNameWithString(char* FileName, char* StringToConcat, char* resultString, int len_resultString);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
    if (argc != 3)
    {
        fprintf(stderr, "Ошибка!\n Неверное количество аргументов\n");
        fprintf(stderr, "Необходимые ключи для запуска программы: ""Имя файла для перекодирования\n");
        fprintf(stderr, "Ключ: '-a' - ANSI файл\n");
		fprintf(stderr, "Ключ: '-u' - UNICODE файл\n");
        return 0;
    }
    else
    {
        if (strcmp(argv[1], ANSI_KEY) == 0)
        {
            ANSI_To_Unicode(argv[2]);
        }
        else if (strcmp(argv[1], UNICODE_KEY) == 0)
        {
            Unicode_To_ANSI(argv[2]);
        }
        else
        {
            fprintf(stderr, "Неправильные аргументы\n");
        }
    }
    fprintf(stdout, "Запись успешно завершена");
    return 0;
}
void ANSI_To_Unicode(LPCSTR fileName)
{
    HANDLE handleSourceFile, handleDestinationFile; 
    CHAR asciBuff[SIZE_BUFF_FOR_READING_FILE]; 
    WCHAR * unicodeBuff; 
    DWORD countRdChars, countWrWChars;  
    BOOL resultRdFile = true; 
	char additionalTextToFileName[] = "_encoded_To_Unicode";
	char nameDestinationFile[FILENAME_MAX];
	ConcatFileNameWithString((char*)fileName, additionalTextToFileName, nameDestinationFile, FILENAME_MAX);
    printf("Перекодированный файл будет расположен в:  \n %s\n", nameDestinationFile);
    handleSourceFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ,
                                  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handleSourceFile == INVALID_HANDLE_VALUE)
    {
        ShowError();
        exit(EXIT_FAILURE);
    }
    handleDestinationFile = CreateFile(nameDestinationFile, GENERIC_WRITE, 0,
                                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handleDestinationFile == INVALID_HANDLE_VALUE)
    {
        ShowError();
        exit(EXIT_FAILURE);
    }
    while (((resultRdFile = ReadFile(handleSourceFile, &asciBuff, sizeof(asciBuff), &countRdChars, NULL)) != false) && (countRdChars != 0))
    {
		int sizeToAllocate = MultiByteToWideChar(CP_UTF8, 0, asciBuff, countRdChars, NULL, 0);
		unicodeBuff = (WCHAR *)calloc(sizeToAllocate, sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, asciBuff, countRdChars, unicodeBuff, sizeToAllocate);
        if ((!WriteFile(handleDestinationFile, unicodeBuff, sizeToAllocate * sizeof(WCHAR), &countWrWChars, NULL)) ||
			(countWrWChars != sizeToAllocate * sizeof(WCHAR)))
        {
            ShowError();
            exit(EXIT_FAILURE);
        }
		free(unicodeBuff);
    }
    if (resultRdFile == false && countRdChars != 0)
    {
        ShowError();
        exit(EXIT_FAILURE);
    }

    CloseHandle(handleSourceFile);
    CloseHandle(handleDestinationFile);
}
void Unicode_To_ANSI(LPCSTR fileName)
{
    HANDLE handleSourceFile, handleDestinationFile; 
    CHAR rawBytesBuff[SIZE_BUFF_FOR_READING_FILE];
	WCHAR * unicodeBuff;
    CHAR * asciBuff;
    DWORD countReadedBytes, countWritenBytes;
    BOOL resultRdFile = true;
	CHAR additionalTextToFileName[] = "_encoded_To_ANSI";
	CHAR nameDestinationFile[FILENAME_MAX];
	ConcatFileNameWithString((char*)fileName, additionalTextToFileName, nameDestinationFile, FILENAME_MAX);
    printf(" Перекодированный файл будет расположен в:  \n %s\n", nameDestinationFile);
    handleSourceFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ,
								  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handleSourceFile == INVALID_HANDLE_VALUE)
    {
        ShowError();
        exit(EXIT_FAILURE);
    }
    handleDestinationFile = CreateFile(nameDestinationFile, GENERIC_WRITE, 0,
									   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handleDestinationFile == INVALID_HANDLE_VALUE)
    {
        ShowError();
        exit(EXIT_FAILURE);
    }
    if (!ReadFile(handleSourceFile, &rawBytesBuff, 2, &countReadedBytes, NULL))
    {
        ShowError();
        exit(EXIT_FAILURE);
    }
    while (((resultRdFile = ReadFile(handleSourceFile, &rawBytesBuff, sizeof(rawBytesBuff), &countReadedBytes, NULL)) != false) && (countReadedBytes != 0))
    {
		int unicodeLenght = MultiByteToWideChar(CP_UTF8, 0, rawBytesBuff, countReadedBytes, NULL, 0);
		unicodeBuff = (WCHAR*)calloc(unicodeLenght, sizeof(WCHAR));
		MultiByteToWideChar(CP_UTF8, 0, rawBytesBuff, countReadedBytes,
						    unicodeBuff, unicodeLenght);
		int asciLenght = WideCharToMultiByte(CP_ACP, 0, unicodeBuff, unicodeLenght, NULL, 0, NULL, NULL);
		asciBuff = (CHAR *) calloc(asciLenght, sizeof(CHAR));
		WideCharToMultiByte(CP_ACP, 0, unicodeBuff, unicodeLenght, asciBuff,
							asciLenght, NULL, NULL);
        if ((!WriteFile(handleDestinationFile, asciBuff, asciLenght, &countWritenBytes, NULL)) || (countWritenBytes != asciLenght))
        {
            ShowError();
            exit(EXIT_FAILURE);
        }
		free(unicodeBuff);
		free(asciBuff);
    }
    if (resultRdFile == false && countReadedBytes != 0)
    {
        ShowError();
        exit(EXIT_FAILURE);
    }
    CloseHandle(handleSourceFile);
    CloseHandle(handleDestinationFile);
}
bool ConcatFileNameWithString(char * FileName, char * StringToConcat, char * resultString, int len_resultString)
{	
	for (int i = 0; i < len_resultString; i++)
	{
		resultString[i] = '\0';
	}
	if ((strlen(FileName) + strlen(StringToConcat)) > FILENAME_MAX)
	{
		resultString = NULL;
		return false;
	}
	char * refToLastDot = strrchr(FileName, '.');
	int lastDotPositionInFileName = refToLastDot - FileName;
	strncpy(resultString, FileName, lastDotPositionInFileName);
	snprintf(resultString, FILENAME_MAX, "%s%s%s", resultString, StringToConcat, refToLastDot);
	return true;
}
void ShowError()
{
    LPVOID lpMsgBuf;
    DWORD dwLastError = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)& lpMsgBuf, 0, NULL);
    printf("\n%s\n", lpMsgBuf);
    LocalFree(lpMsgBuf);
    return;
}
