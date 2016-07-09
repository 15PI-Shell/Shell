#ifndef Execute
#define Execute

#include <string.h>
#include <windows.h>

typedef enum 
{
	ExecResult_Success,
	ExecResult_OutOfResources,
	ExecResult_BadFormat,
	ExecResult_FileNotFound,
	ExecResult_PathNotFound,
}ExecResult;


#endif
