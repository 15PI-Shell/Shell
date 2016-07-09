#include "Execute.h"

char CurrentDirectory[]="C:";

ExecResult Execution(char * nameProc)
{
	UINT result;
	char check[] = ".exe";
	char CopyNameProcces[MAX_PATH];
	strcpy(CopyNameProcces, nameProc);
	int length = strlen(CopyNameProcces);
	int i;
	for (i = 0; i < 4; i++)
	{
		if (check[3 - i] != CopyNameProcces[length - i - 1])
		{
			i = -1;
			break;
		}
	}
	if (i == -1)
		strcat(CopyNameProcces, ".exe");
	if ((tolower(CopyNameProcces[0]) >= 'a') && (tolower(CopyNameProcces[0]) >= 'z') && (CopyNameProcces[1] == ':'))
		result = WinExec(CopyNameProcces, SW_SHOW);
	else
	{
			char CurDir[MAX_PATH];
			strcpy(CurDir,CurrentDirectory);
			if (CurDir[strlen(CurDir) - 1] != '\\')
				strcat(CurDir, "\\");
			strcat(CurDir, CopyNameProcces);
			result = WinExec(CurDir, SW_SHOW);
			if (result<31)
				result = WinExec(CopyNameProcces, SW_SHOW);
	}
	switch (result)
	{
	case 0:
		return ExecResult_OutOfResources;
	case ERROR_BAD_FORMAT:
		return ExecResult_BadFormat;
	case ERROR_FILE_NOT_FOUND:
		return ExecResult_FileNotFound;
	case ERROR_PATH_NOT_FOUND:
		return ExecResult_PathNotFound;
	default:
		return ExecResult_Success;
	}
}
	
