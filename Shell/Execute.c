#include "FindFile.h"


ExecResult FileExecute(char* nameProc, char* parametrs)
{
	UINT result; //переменная для возвращения результата открытия файла
	if ((tolower(nameProc[0]) >= 'a') && (tolower(nameProc[0]) <= 'z') && (nameProc[1] == ':')) //если указан путь C:\...
		result = ShellExecuteA(0, "open", nameProc, parametrs, NULL, 1); //запускаем файл по переданному пути
	else //если полный путь не указан (либо просто имя файла, либо относителный путь)
	{
		char CurDir[MAX_PATH];
		strcpy(CurDir, CurrentDirectory);
		if ((CurDir[strlen(CurDir) - 1] != '\\') && (nameProc[0] != '\\')) //если текущий путь не заканчивается слешом и введенный им не начинается, то его нужно добавить
			strcat(CurDir, "\\");
		strcat(CurDir, nameProc); //добавляем к текущей дирректории то, что было введено пользователем
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
				result = ShellExecuteA(0, "open", nameProc, parametrs, NULL, 1);
		}
	}
	if (result > 32)
		return ExecResult_Success;
	else
		switch (result)
		{
		case 0:
		case SE_ERR_OOM:
			return ExecResult_NotEnoughResources;
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			return ExecResult_FileNotFound;
		case ERROR_BAD_FORMAT:
			return ExecResult_WrongExe;
		case SE_ERR_ACCESSDENIED:
			return ExecResult_AccessDenied;
		case SE_ERR_ASSOCINCOMPLETE:
		case SE_ERR_NOASSOC:
			return ExecResult_WrongAssociation;
		default:
			return ExecResult_UnknownError;
		}
}
