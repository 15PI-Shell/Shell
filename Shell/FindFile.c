#include "FindFile.h"


SingleListStringNode* FindFilesAndDirsPrefix(char* PrefixFile)
{
	char* cpyPrefix = (char*)malloc(MAX_PATH);
	strcpy(cpyPrefix, PrefixFile);
	PrefixFile = cpyPrefix;

	WIN32_FIND_DATA FileData;
	SingleListStringNode *last = NULL;
	char * pathPTR;
	int i = -1, j = -1, k = 0;
	HANDLE HandleFile;
	char CopyCurrentDir[MAX_PATH];
	strcat(PrefixFile, "*");
	if ((tolower(PrefixFile[0]) >= 'a') && (tolower(PrefixFile[0]) <= 'z') && (PrefixFile[1] == ':'))
	{
		strcpy(CopyCurrentDir, PrefixFile);
		k = 1;
	}
	else
	{
		strcpy(CopyCurrentDir, CurrentDirectory);
		//GetCopyCurrentDirA(sizeof(CopyCurrentDir), CopyCurrentDir); //получение текущей директории 

		if (CopyCurrentDir[strlen(CopyCurrentDir) - 1] != '\\') //если текущий путь не заканчивается слешом и введенный им не начинается, то его нужно добавить
			strcat(CopyCurrentDir, "\\"); //создаем путь для функции
		strcat(CopyCurrentDir, PrefixFile); //добавляем наш префикс
	}
	HandleFile = FindFirstFileA(CopyCurrentDir, &FileData); //пытаемся получить дескриптор файла. В FileData отправляются данные файла
	if (HandleFile != INVALID_HANDLE_VALUE) //если подходящий файл или папка найдены
	{
		//printf("\n%s\n", CopyCurrentDir);
		do
		{
			SingleStrlistAddDownmost(&last, FileData.cFileName);
			//printf("%s\n", FileData.cFileName);
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);
	if (!k)
	{
		pathPTR = getenv("PATH");

		while (pathPTR[++i]) //парсим PATH
		{
			if (pathPTR[i] != ';') //проходим все пути, указанные в PATH и ищем походящие файлы
			{
				CopyCurrentDir[++j] = pathPTR[i];
			}

			else
			{
				CopyCurrentDir[j + 1] = 0;
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
	}
	free(PrefixFile);
	return last;
}

SingleListStringNode* FindFilesAndDirsMask(char* FileMask, char* WhereFind, FindType type)
{
	WIN32_FIND_DATA FileData;
	SingleListStringNode *last = NULL;
	HANDLE HandleFile;
	char* CopyDirectory = (char*)malloc(MAX_PATH);
	strcpy(CopyDirectory, WhereFind);
	WhereFind = CopyDirectory;
	if ((tolower(FileMask[0]) >= 'a') && (tolower(FileMask[0]) <= 'z') && (FileMask[1] == ':')) //если указан путь C:\...
		strcpy(WhereFind, FileMask);
	else
	{
		strcpy(WhereFind, WhereFind);
		if (WhereFind[strlen(WhereFind) - 1] != '\\' && FileMask[0] != "\\") //если текущий путь не заканчивается слешом и введенный им не начинается то его нужно добавить
			strcat(WhereFind, "\\"); //создаем путь для функции
		strcat(WhereFind, FileMask); //добавляем нашу маску	
	}
	HandleFile = FindFirstFileA(WhereFind, &FileData); //пытаемся получить дескриптор файла. В FileData отправляются данные файла
	if (HandleFile != INVALID_HANDLE_VALUE) //если подходящий файл или папка найдены
	{
		//printf("\n%s\n", CopyCurrentDir);
		do
		{
			//printf("%s\n", FileData.cFileName);
			if (strcmp(FileData.cFileName, ".") != 0 && strcmp(FileData.cFileName, "..") != 0)
				switch (type)
				{
				case FilesAndFolders:
					SingleStrlistAddDownmost(&last, FileData.cFileName);//добавляем имя файла/папки в список
					break;
				case Files:
					if (FileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
						SingleStrlistAddDownmost(&last, FileData.cFileName);
					break;
				case Folders:
					if (FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
						SingleStrlistAddDownmost(&last, FileData.cFileName);
					break;
				}
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);
	free(WhereFind);
	return last;
}