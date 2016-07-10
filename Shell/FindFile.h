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

//хранит путь к текущей папке
char *CurrentDirectory;

//функция поиска файла по префиксу
SingleListStringNode* FindFiles(char* PrefixFile);

//функция запуска любых файлов (обязательно указывать расширение)
ExecResult FileExecute(char * nameProc);

#endif