#include "FindFile.h"


STRLIST_NODE* FindFiles(char* PrefixFile)
{
	WIN32_FIND_DATA FileData;
	STRLIST_NODE *last = NULL;
	char way[MAX_PATH];
	char * pathPTR;
	int i = -1, j = -1;
	HANDLE HandleFile;

	GetCurrentDirectoryA(sizeof(way), way); //получение текущей директории 
	strcat(way, "\\"); //создаем путь для функции
	strcat(way, PrefixFile); //добавляем наш префикс
	HandleFile = FindFirstFileA(way, &FileData); //пытаемся получить дескриптор файла. В FileData отправляются данные файла
	if (HandleFile != INVALID_HANDLE_VALUE) //если подходящий файл или папка найдены
	{
		printf("\n%s\n", way);
		do
		{
				printf("%s\n", FileData.cFileName);
				StrlistAdd(&last, FileData.cFileName); //добавляем имя файла/папки в список
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);

	pathPTR = getenv("PATH");

	while (pathPTR[++i]) //парсим PATH
	{
		if (pathPTR[i] != ';') //проходим все пути, указанные в PATH и ищем походящие файлы
		{
			way[++j] = pathPTR[i];
		}

		else
		{
			way[j] = 0;
			j = -1;
			strcat(way, "\\");
			strcat(way, PrefixFile);
			strcat(way, "*");
			HandleFile = FindFirstFileA(way, &FileData);
			if (HandleFile != INVALID_HANDLE_VALUE)
			{
				printf("\n%s\n", way);
				do
				{
						printf("%s\n", FileData.cFileName);
						StrlistAdd(&last, FileData.cFileName);
				} while (FindNextFileA(HandleFile, &FileData) != 0);
			}
		}
	}
	FindClose(HandleFile);
	return last;
}