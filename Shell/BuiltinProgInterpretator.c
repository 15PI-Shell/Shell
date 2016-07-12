#include "BuiltinProgInterpretator.h"

void analisator(char* mas)
{
	int p = 0, lenname = 0, lenarg = 0, flag = 0;
	BPC_Returns TypeOfResult;
	char* result1 = NULL, *result2 = NULL;

	// flag=0 - первый проход; 1 - если есть "|"; 2- выход
	while ((flag == 0) || (flag == 1))
	{
		while ( mas[p] == ' ')
			p++;

		//находим длину названия команды
		if ( mas[p] == '"')
		{
			p++;
			lenname = p;
			while ( mas[lenname] != '"')
				lenname++;
			lenname = lenname - p;
		} 
		else
		{
			lenname = p;
			while ( mas[lenname] != ' ')
				lenname++;
			lenname = lenname - p;
		}
		//указатель на строку с названием команды
		char* ptrname = NULL;
		ptrname = (char*)malloc(lenname * sizeof(char));

		ptrname[lenname] = '\0';
		int i = 0;
		while (i != lenname)
		{
			ptrname[i] = mas[p];
			p++;
			i++;
		}
		p++;
		if (flag == 0)
		{
			//находим длину строки аргументов
			lenarg = p;
			while ((mas[lenarg] != '\0') && (mas[lenarg] != '|'))
				lenarg++;
			lenarg = lenarg - p;

			// указатель на строку с аругментами
			char* ptrarg = (char*)malloc(lenarg * sizeof(char));
			i = 0;
			while (i != lenarg)
			{
				ptrarg[i] = mas[p];
				p++;
				i++;
			}
			ptrarg[lenarg] = '\0';

			// ищем имя команды во встроенных 
			result1 = BPC_Execute(ptrname, ptrarg, &TypeOfResult); 

			// ищем имя команды во внешних
			if (result1 == -2)
			{
				ExecResult fileresult = FileExecute(ptrname, ptrarg);
				if (fileresult != ExecResult_Success)
				{
					printf("_This Builtin program doesn't exist_\n");

					// search builtin programs with the same prefix
					lenname--;
					mas[lenname] = '\0';
					SingleListStringNode* ptrListOfProg = BPC_GetHints(mas);
					while ((ptrListOfProg == 0) && (lenname > 0))
					{
						lenname--;
						mas[lenname] = '\0';
						ptrListOfProg = BPC_GetHints(mas);
					}

					if (lenname != 0)
					{
						printf("Builtin programs with the same prefix '%s'\n",mas);

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
				//printf("_Extern file was launched successfully_\n");
				result1 = 0;
			}
			
			if (result1 == -1)
			{
				printf("_Error_\n");
				return;
			}

			if (mas[p] == '\0')
				flag = 2;
			else
			{
				p++;
				flag = 1;
			}
		}
		else
		{
			// помещаем результат предыдущей функции в строку аргументов следующей 
			result2 = BPC_Execute(ptrname, result1, &TypeOfResult);

			if (result2 == -1)
			{
				printf("_Error_\n");
				return;
			}

			if (result2 == -2)
			{
				ExecResult fileresult = FileExecute(ptrname, result1);
				if (fileresult != ExecResult_Success)
				{
					printf("_This Builtin program '");
					i = 0;
					while (ptrname[i] != '\0')
					{
						printf("%c", ptrname[i]);
						i++;
					}
					printf("' not exist_\n");

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
				//printf("_Extern file was launched successfully_\n");
				result2 = 0;
			}
			result1 = result2;
			while ((mas[p] != '\0') && (mas[p] != '|'))
				p++;
			if (mas[p] == '\0')
				flag = 2;
			else
				p++;
		}
	}

	
	if (result1 != 0)
	{
		printf("\nRESULT:  ");
		while (*result1 != '\0')
		{
			printf("%c", *result1);
			result1++;
		}
	}

	return;
}
