#include "FindFile.h"


SingleListStringNode* FindFilesAndDirsPrefix(char* PrefixFile)
{
	SingleListStringNode *last = NULL;
	if (strlen(PrefixFile) > MAX_PATH)
		return last;
	char* cpyPrefix = (char*)malloc(MAX_PATH+1);
	strcpy(cpyPrefix, PrefixFile);
	PrefixFile = cpyPrefix;

	char * pathPTR;
	int i = -1, j = -1;
	char PathDir[MAX_PATH+1];
	strcat(PrefixFile, "*");
	FindFilesAndDirsMask(PrefixFile, CurrentDirectory, FilesAndFolders, &last);
	while (PrefixFile[++i])
		if (PrefixFile[i] == '\\')
		{
			i = -1;
			break;
		}
	if (((tolower(PrefixFile[0]) < 'a') || (tolower(PrefixFile[0]) > 'z') || (PrefixFile[1] != ':')) && (i != -1))
	{
		pathPTR = getenv("PATH");
		i = -1;
		while (pathPTR[++i]) //парсим PATH
		{
			if (pathPTR[i] != ';') //проходим все пути, указанные в PATH и ищем походящие файлы
			{
				PathDir[++j] = pathPTR[i];
			}

			else
			{
				PathDir[j + 1] = 0;
				j = -1;
				FindFilesAndDirsMask(PrefixFile, PathDir, FilesAndFolders, &last);
			}
		}
	}
	free(PrefixFile);
	return last;
}

SingleListStringNode* FindFilesAndDirsMask(char* FileMask, char* WhereFind, FindType type, SingleListStringNode** last)
{
	WIN32_FIND_DATA FileData;
	HANDLE HandleFile;
	char* CopyDirectory = (char*)malloc(MAX_PATH+1);
	strcpy(CopyDirectory, WhereFind);
	WhereFind = CopyDirectory;
	if ((tolower(FileMask[0]) >= 'a') && (tolower(FileMask[0]) <= 'z') && (FileMask[1] == ':')) //если указан путь C:\...
		strcpy(WhereFind, FileMask);
	else
	{
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
					SingleStrlistAddDownmost(last, FileData.cFileName);//добавляем имя файла/папки в список
					break;
				case Files:
					if (FileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
						SingleStrlistAddDownmost(last, FileData.cFileName);
					break;
				case Folders:
					if (FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
						SingleStrlistAddDownmost(last, FileData.cFileName);
					break;
				}
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);
	free(WhereFind);
	return *last;
}