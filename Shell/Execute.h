#ifndef Execute
#define Execute

#include <string.h>
#include <windows.h>

typedef enum //результаты, возвращаемые функцией FileExecute
{
	ExecResult_Success,
	ExecResult_OutOfResources,
	ExecResult_BadFormat,
	ExecResult_FileNotFound,
	ExecResult_PathNotFound,
}ExecResult;

char CurrentDirectory[MAX_PATH]; //хранит путь к текущей папке

ExecResult FileExecute(char * nameProc); //функция запуска .exe файлов

#endif
