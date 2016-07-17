#include "FindFile.h"

ExecResult FileExecute(char* nameProc, char* parametrs)
{
	UINT result; //переменная для возвращения результата запуска файла
	char check[] = ".exe";
	int lenName = strlen(nameProc);
	int i;
	for (i = 0; i < 4; i++)
	{
		if (check[3 - i] != nameProc[lenName - i - 1])
		{
			i = -1;
			break;
		}
	}
	if (i == -1)
		return FileExecuteShell(nameProc, parametrs);
	else
		return FileExecuteCreate(nameProc, parametrs);

}

ExecResult FileExecuteShell(char* nameProc, char* parametrs)
{
	UINT result;
	SingleLinklistNode* list = NULL;
	int Find = 0;
	if ((tolower(nameProc[0]) >= 'a') && (tolower(nameProc[0]) <= 'z') && (nameProc[1] == ':')) //если указан путь C:\...
	{
		Find = FileExistenceCheck(nameProc);
		if (Find)
			return ExecResult_ExecuteFailed;
		result = ShellExecuteA(0, "open", nameProc, parametrs, NULL, 1); //запускаем файл по переданному пути
	}
	else //если полный путь не указан (либо просто имя файла, либо относителный путь)
	{
		char CurDir[MAX_PATH] = "";
		strcpy(CurDir, CurrentDirectory);
		if ((CurDir[strlen(CurDir) - 1] != '\\') && (nameProc[0] != '\\')) //если текущий путь не заканчивается слешом и введенный им не начинается, то его нужно добавить
			strcat(CurDir, "\\");
		strcat(CurDir, nameProc); //добавляем к текущей дирректории то, что было введено пользователем
		Find = FileExistenceCheck(CurDir);
		if (Find)
			return ExecResult_ExecuteFailed;
		result = ShellExecuteA(0, "open", CurDir, parametrs, NULL, 1);
		if (result < 33) //если файл не был запущен
		{
			//делаем проверку на то, что nameProc не является относительным путем
			int i = -1;
			while (nameProc[++i])
			{
				if (nameProc[i] == '\\')
				{
					i = -1;
					break;
				}
			}
			if (i != -1) //если передано только имя файла, то ищем его по путям в PATH
			{
				Find = FileExistenceCheck(nameProc);
				if (Find)
					return ExecResult_ExecuteFailed;
				result = ShellExecuteA(0, "open", nameProc, parametrs, NULL, 1);
			}
		}
	}
	if (result > 32)
		return ExecResult_Success;
	else
		return ExecResult_ExecuteFailed;
}

ExecResult FileExecuteCreate(char* nameProc, char* parametrs)
{
	char ProcAndParam[MAX_PATH + 1] = "";
	strcpy(ProcAndParam, nameProc);
	if (parametrs)
	{
		strcat(ProcAndParam, " ");
		strcat(ProcAndParam, parametrs);
	}
	if ((tolower(nameProc[0]) >= 'a') && (tolower(nameProc[0]) <= 'z') && (nameProc[1] == ':')) //если указан путь C:\...
		return AttemptCreation(nameProc);
	else //если полный путь не указан (либо просто имя файла, либо относителный путь)
	{
		char CurDir[MAX_PATH] = "";
		strcpy(CurDir, CurrentDirectory);
		if ((CurDir[strlen(CurDir) - 1] != '\\') && (nameProc[0] != '\\')) //если текущий путь не заканчивается слешом и введенный им не начинается, то его нужно добавить
			strcat(CurDir, "\\");
		strcat(CurDir, nameProc); //добавляем к текущей дирректории то, что было введено пользователем
		ExecResult result = AttemptCreation(CurDir);
		if (result != ExecResult_ExecuteFailed)
			return result;
		else //если файл не запустился
		{
			//делаем проверку на то, что nameProc не является относительным путем
			int i = -1;
			while (nameProc[++i])
			{
				if (nameProc[i] == '\\')
				{
					i = -1;
					break;
				}
			}
			if (i != -1) //если передано только имя файла, то ищем его по путям в PATH
				return AttemptCreation(nameProc);
			else
				return ExecResult_ExecuteFailed;
		}
	}
}

ExecResult AttemptCreation(char* FileName)
{
	STARTUPINFOA StartInfo;
	ZeroMemory(&StartInfo, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION ProcInfo;
	if (CreateProcessA(NULL, FileName, NULL, NULL, FALSE, NULL, NULL, NULL, &StartInfo, &ProcInfo))
	{
		WaitForSingleObject(ProcInfo.hProcess, INFINITE);
		DWORD exitCode = 0;
		if (GetExitCodeProcess(ProcInfo.hProcess, &exitCode))
			if (exitCode == 0)
				return ExecResult_Success;
			else
				return ExecResult_BadReturnCode;
		else
			return ExecResult_UnknownError;
	}
	else return ExecResult_ExecuteFailed;
}

int FileExistenceCheck(char *Path)
{
	int k = 1;
	SingleLinklistNode* list = FindFilesAndDirsPrefix(Path);
	char Name[100];
	int i = -1, j = -1;
	while (Path[++i])
	{
		if (Path[i] == '\\')
			j = -1;
		else
			Name[++j] = Path[i];
	}
	Name[++j] = 0;
	if (list == NULL)
		return k;
	else
	{
		while (list != NULL)
		{
			if (strcmp((char*)list->value, Name) == 0)
			{
				k = 0;
				break;
			}
			else
				SingleLinklistRemoveDownmost(&list);
		}
	}
	return k;
}