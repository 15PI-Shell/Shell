#include "BuiltinProgInterpretator.h"

void CmdInterpretator(char* mas)
{
	mas = MkStrCopy(mas);
	int p = 0, lenname = 0, lenarg = 0;
	BPC_Returns TypeOfResult;
	char strresult[100] = "\0";
	char* result = NULL;
	const char* del = ">\0";
	char* cmd;
	
	
	cmd = strtok(mas, del); // выделение одной команды из строки аргументов

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
	ptrname = (char*)malloc((lenname+1) * sizeof(char));

	ptrname[lenname] = '\0';
	int i = 0;
	while (i != lenname)
	{
		ptrname[i] = cmd[p];
		p++;
		i++;
	}
	if (cmd[p] == '"') p++;

	// search arguments
	lenarg = strlen(cmd) - p;

	// указатель на строку с аругментами
	char* ptrarg = (char*)malloc((lenarg+2+strlen(strresult)) * sizeof(char));
	i = 0;
	while (i != lenarg)
	{
		ptrarg[i] = cmd[p];
		p++;
		i++;
	}
	ptrarg[lenarg] = ' ';
	ptrarg[lenarg+1] = '\0';
	strcat(ptrarg, strresult); // add result of program to argaments

	// search ptrname in builtinprogram and start this program
	//if (result != NULL) free(result);
	result = BPC_Execute(ptrname, ptrarg, &TypeOfResult);

	// search ptrname in extern program and start this program
	if (result == -2)
	{
		ExecResult fileresult = FileExecute(ptrname, ptrarg);
		if (fileresult != ExecResult_Success)
		{
			printf("_This Builtin program doesn't exist_\n");

			// search builtin programs with the same prefix
			lenname--;
			ptrname[lenname] = '\0';
			SingleListStringNode* ptrListOfProg = BPC_GetHints(ptrname);
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
					i = 0;
					while (*(ptrListOfProg->value + i) != '\0')
					{
						printf("%c", *(ptrListOfProg->value + i));
						i++;
					}
					printf("\n");
					ptrListOfProg = ptrListOfProg->up;
				}

			}
			else
				printf("Builtin programs with the same prefix don't exist\n");

			// errors of extern files
			switch (fileresult)
			{
			case ExecResult_NotEnoughResources:
				printf("_Not enough resources to start external file_\n");
				break;
			case ExecResult_FileNotFound:
				printf("_External File was not found_\n");
				break;
			case ExecResult_WrongExe:
				printf("_The .exe external file is invalid_\n");
				break;
			case ExecResult_AccessDenied:
				printf("_Access denied for open external file_\n");
				break;
			case ExecResult_WrongAssociation:
				printf("_External file name association is incomplete or invalid_\n");
				break;
			case ExecResult_UnknownError:
				printf("_Unknown Error for open external file_\n");
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
		strcpy(strresult,result);
		
	free(ptrarg);
	free(ptrname);
	cmd = strtok('\0', del); // search the following comand 
}
	
if (strresult != 0)
{
	printf("\nRESULT: ");
	printf("%s\n",strresult);
}

return;
}
