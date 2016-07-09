#ifndef Execute
#define Execute

#include <string.h>
#include <windows.h>

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
char CurrentDirectory[MAX_PATH]; 

//функция запуска любых файлов (обязательно указывать расширение)
ExecResult FileExecute(char * nameProc); 

#endif
