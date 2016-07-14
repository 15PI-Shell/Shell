#ifndef FindFile
#define FindFile

#include <windows.h>
#include "LinkedList.h"
#include <string.h>

//результаты, возвращаемые функцией FileExecute
typedef enum
{
	ExecResult_BadReturnCode,
	ExecResult_ExecuteFailed,
	ExecResult_UnknownError,
	ExecResult_Success,
}ExecResult;

typedef enum
{
	Files = 1,
	Folders,
	FilesAndFolders,
}FindType;

//хранит путь к текущей папке
char *CurrentDirectory;

//функция поиска файлов и папок по префиксу
SingleLinklistNode* FindFilesAndDirsPrefix(char* PrefixFile);

//функция поиска файлов по маске в указанной директории с указанием типа 
SingleLinklistNode* FindFilesAndDirsMask(char* FileMask, char* WhereFind, FindType type, SingleLinklistNode** last);

//функция запуска любых файлов (обязательно указывать расширение)
ExecResult FileExecute(char* nameProc, char* parametrs);
//функция запуска любых файлов, кромe exe
ExecResult FileExecuteShell(char* nameProc, char* parametrs);
//функция запуска exe файлов
ExecResult FileExecuteCreate(char* nameProc, char* parametrs);
//попытка запуска exe файла и ожидание возвращаемого значения
ExecResult AttemptCreation(char* FileName);
#endif