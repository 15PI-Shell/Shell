#ifndef Execute
#define Execute

#include <string.h>
#include <windows.h>

typedef enum //����������, ������������ �������� FileExecute
{
	ExecResult_Success,
	ExecResult_OutOfResources,
	ExecResult_BadFormat,
	ExecResult_FileNotFound,
	ExecResult_PathNotFound,
}ExecResult;

char CurrentDirectory[MAX_PATH]; //������ ���� � ������� �����

ExecResult FileExecute(char * nameProc); //������� ������� .exe ������

#endif
