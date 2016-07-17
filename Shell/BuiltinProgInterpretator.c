﻿#include "BuiltinProgInterpretator.h"

void CmdInterpretator(char* mas)
{
	mas = MkMemCopy(mas, strlen(mas));
	int p = 0, lenname = 0, lenarg = 0, s = 0, flag = 2, FlagOfResult; // flag for SLSProc; 2 - strt value; 0 - &&; 1 - ||
	BPC_Returns TypeOfResult;
	char strresult[100] = "\0";
	char* result = NULL;
	const char* del = ">\0";
	char* cmd;

	if ((strchr(mas, '|')) && (strchr(mas, '&')))
	{
		printf("_Invalid written data_");
		return;
	}

	if ((strchr(mas, '>')) && ((strchr(mas, '|')) || (strchr(mas, '&'))))
	{
		printf("_Invalid written data_");
		return;
	}

	cmd = SLSProc(mas, &s, &flag);
	while (cmd != 0)
	{
		p = 0;
		while (cmd[p] == ' ')
			p++;

		// search name
		if (cmd[p] == '"')
		{
			p++;
			lenname = p;
			while (cmd[lenname] && cmd[lenname] != '"')
				lenname++;
			lenname = lenname - p;
		}
		else
		{
			lenname = p;
			while (cmd[lenname] && cmd[lenname] != ' ')
				lenname++;
			lenname = lenname - p;
		}
		char* ptrname = NULL;
		ptrname = (char*)malloc((lenname + 1) * sizeof(char));// указатель на строку с именем программы

		strncpy(ptrname, cmd + p, lenname);
		ptrname[lenname] = '\0';
		p += lenname;
		if (cmd[p] == '"') p++;

		// search arguments
		lenarg = strlen(cmd) - p;
		char* ptrarg = (char*)malloc((lenarg + 1) * sizeof(char));// указатель на строку с аругментами
		strncpy(ptrarg, cmd + p, lenarg);
		ptrarg[lenarg] = '\0';

		// search ptrname in builtinprogram and start this program
		result = BPC_Execute(ptrname, ptrarg, &TypeOfResult);

		// search ptrname in extern program and start this program
		if (result == -2)
		{
			ExecResult fileresult = FileExecute(ptrname, ptrarg);
			if (fileresult != ExecResult_Success)
			{
				FlagOfResult = 1;
				printf("Extern program '%s' not successful\n", ptrname);
			}
			else
			{
				result = 0;
				FlagOfResult = 0;
				printf("Extern program '%s' was completed successfully\n", ptrname);
			}
		}
		else
			if (result == -1)
			{
				FlagOfResult = 1;
				printf("Builtin program '%s' not successful\n", ptrname);
			}
			else
				if (result == 0)
				{
					FlagOfResult = 0;
					printf("Builtin program '%s' was completed successfully\n", ptrname);
				}
		if ((result != 0) && (result != -1) && (result != -2))
		{
			printf("Builtin program '%s' was completed successfully\n", ptrname);
			printf("RESULT: %s\n", result);
			FlagOfResult = 0;
		}

		free(ptrarg);
		free(ptrname);
		free(cmd);
		if (FlagOfResult != flag) return;
		cmd = SLSProc(mas, &s, &flag); // search the following comand 

	}
	//---------------------------------------------------------------------
	if (flag == 2)	cmd = strtok(mas, del); // выделение одной команды из строки аргументов

	while (cmd != 0)
	{
		p = 0;
		while (cmd[p] == ' ')
			p++;

		// search name
		if (cmd[p] == '"')
		{
			p++;
			lenname = p;
			while (cmd[lenname] && cmd[lenname] != '"')
				lenname++;
			lenname = lenname - p;
		}
		else
		{
			lenname = p;
			while (cmd[lenname] && cmd[lenname] != ' ')
				lenname++;
			lenname = lenname - p;
		}
		char* ptrname = NULL;
		ptrname = (char*)malloc((lenname + 1) * sizeof(char));// указатель на строку с именем программы

		strncpy(ptrname, cmd + p, lenname);
		ptrname[lenname] = '\0';
		p += lenname;
		if (cmd[p] == '"') p++;

		// search arguments
		lenarg = strlen(cmd) - p;
		char* ptrarg = (char*)malloc((lenarg + 2 + strlen(strresult)) * sizeof(char));// указатель на строку с аругментами
		strncpy(ptrarg, cmd + p, lenarg);
		ptrarg[lenarg] = ' ';
		ptrarg[lenarg + 1] = '\0';
		strcat(ptrarg, strresult); // add result of program to argaments

		// search ptrname in builtinprogram and start this program
		result = BPC_Execute(ptrname, ptrarg, &TypeOfResult);

		// search ptrname in extern program and start this program
		if (result == -2)
		{
			ExecResult fileresult = FileExecute(ptrname, ptrarg);
			if (fileresult != ExecResult_Success)
			{
				printf("_This Builtin program '%s' doesn't exist_\n", ptrname);

				// search builtin programs with the same prefix
				SingleLinklistNode* ptrListOfProg = BPC_GetHints(ptrname);
				while ((ptrListOfProg == 0) && (lenname > 0))
				{
					lenname--;
					ptrname[lenname] = '\0';
					ptrListOfProg = BPC_GetHints(ptrname);
				}

				if (lenname != 0)
				{
					printf("Builtin programs with the same prefix '%s'\n", ptrname);

					// вывод комманд  с тем же префиксом:
					while (ptrListOfProg != NULL)
					{
						printf("%s\n", ptrListOfProg->value);
						ptrListOfProg = ptrListOfProg->up;
					}
				}
				else
					printf("Builtin programs with the same prefix don't exist\n");

				// errors of extern files
				switch (fileresult)
				{
				case ExecResult_BadReturnCode:
					printf("_Extern file completion error_\n");
					break;
				case ExecResult_ExecuteFailed:
					printf("_Extern file run error (Extern file not exist)_\n");
					break;
				case ExecResult_UnknownError:
					printf("_Unknown Error to open external file_\n");
					break;
				}
				return;
			}
			result = 0;
		}

		if (result == -1)
		{
			printf("_Error_\n");
			return;
		}

		if (result == 0)
			strresult[0] = '\0';

		if ((result != 0) && (result != -1) && (result != -2))
			strcpy(strresult, result);

		free(ptrname);
		//printf("%s",ptrarg);
		free(ptrarg);
		cmd = strtok('\0', del); // search the following comand 
	}

	if (strlen(strresult) != 0)
	{
		printf("RESULT: %s\n", strresult);
	}
	return;
}
