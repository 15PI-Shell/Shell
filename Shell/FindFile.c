#include "FindFile.h"


SingleListStringNode* FindFiles(char* PrefixFile)
{
	WIN32_FIND_DATA FileData;
	SingleListStringNode *last = NULL;
	char * pathPTR;
	int i = -1, j = -1;
	HANDLE HandleFile;
	char CopyCurrentDir[MAX_PATH];
	strcat(PrefixFile, "*");
	strcpy(CopyCurrentDir, CurrentDirectory);
	//GetCopyCurrentDirA(sizeof(CopyCurrentDir), CopyCurrentDir); //получение текущей директории 

	if (CopyCurrentDir[strlen(CopyCurrentDir) - 1] != '\\') //если текущий путь не заканчивается слешом и введенный им не начинается, то его нужно добавить
		strcat(CopyCurrentDir, "\\"); //создаем путь для функции
	strcat(CopyCurrentDir, PrefixFile); //добавляем наш префикс			
	HandleFile = FindFirstFileA(CopyCurrentDir, &FileData); //пытаемся получить дескриптор файла. В FileData отправляются данные файла
	if (HandleFile != INVALID_HANDLE_VALUE) //если подходящий файл или папка найдены
	{
		//printf("\n%s\n", CopyCurrentDir);
		do
		{
			//printf("%s\n", FileData.cFileName);
			SingleStrlistAddDownmost(&last, FileData.cFileName); //добавляем имя файла/папки в список
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);

	pathPTR = getenv("PATH");

	while (pathPTR[++i]) //парсим PATH
	{
		if (pathPTR[i] != ';') //проходим все пути, указанные в PATH и ищем походящие файлы
		{
			CopyCurrentDir[++j] = pathPTR[i];
		}

		else
		{
			CopyCurrentDir[j+1] = 0;
			j = -1;
			if (CopyCurrentDir[strlen(CopyCurrentDir) - 1] != '\\') //если текущий путь не заканчивается слешом и введенный им не начинается, то его нужно добавить
				strcat(CopyCurrentDir, "\\");
			strcat(CopyCurrentDir, PrefixFile);
			HandleFile = FindFirstFileA(CopyCurrentDir, &FileData);
			if (HandleFile != INVALID_HANDLE_VALUE)
			{
				//printf("\n%s\n", CopyCurrentDir);
				do
				{
					//printf("%s\n", FileData.cFileName);
					SingleStrlistAddDownmost(&last, FileData.cFileName);
				} while (FindNextFileA(HandleFile, &FileData) != 0);
			}
		}
	}
	FindClose(HandleFile);
	return last;
}