#ifndef FindFile
#define FindFile

#include <windows.h>
#include "StringList.h"
#include <string.h>

//результаты, возвращаемые функцией FileExecute
typedef enum
{
	ExecResult_NotEnoughResources,
	ExecResult_FileNotFound,
	ExecResult_WrongExe,
	ExecResult_AccessDenied,
	ExecResult_WrongAssociation,
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
char CurrentDirectory[MAX_PATH];

//функция поиска файлов и папок по префиксу
SingleListStringNode* FindFilesAndDirsPrefix(char* PrefixFile);

//функция поиска файлов по маске в указанной директори
SingleListStringNode* FindFilesAndDirsMask(char* FileMask, char* WhereFind, FindType type);

//функция запуска любых файлов (обязательно указывать расширение)
ExecResult FileExecute(char* nameProc, char* parametrs);

#endif