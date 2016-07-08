#include "FindFile.h"


STRLIST_NODE* FindFile(char* FileMask)
{
	STRLIST_NODE *last = NULL;
	char way[MAX_PATH];
	char * pathPTR;
	int i = -1, j = -1, k = 0;
	HANDLE HandleFile;
	while (FileMask[++i]) //проверка на маску. Если просто введен префикс, то добавляем * и ищем по маске
	{
		if (FileMask[i] == '*'|| FileMask[i] == '?')
		{
			k = 1;
			break;
		}
	}
	if (!k)
		strcat(FileMask, "*");

	GetCurrentDirectoryA(sizeof(way), way); //получение текущей директории 
	strcat(way, "\\"); //создаем путь для функции
	strcat(way, FileMask); //добавляем нашу маску
	HandleFile = FindFirstFileA(way, &FileData); //пытаемся получить дескриптор файла. В FileData отправляются данные файла
	if (HandleFile != INVALID_HANDLE_VALUE) //если подходящий файл найден
	{
		//printf("\n%s\n", way);
		do
		{
			if (strcmp(FileData.cFileName, ".") && strcmp(FileData.cFileName, "..")) //проверяем, что это не 2 стандартные папки: . и ..
			{
				//printf("%s\n", FileData.cFileName);
				StrlistAdd(&last, FileData.cFileName); //добавляем имя файла в список
			}
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);

	pathPTR = getenv("PATH");

	i = -1;
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
			strcat(way, FileMask);
			HandleFile = FindFirstFileA(way, &FileData);
			if (HandleFile != INVALID_HANDLE_VALUE)
			{
				//printf("\n%s\n", way);
				do
				{
					if (strcmp(FileData.cFileName, ".") && strcmp(FileData.cFileName, ".."))
					{
						//printf("%s\n", FileData.cFileName);
						StrlistAdd(&last, FileData.cFileName);
					}
				} while (FindNextFileA(HandleFile, &FileData) != 0);
			}
		}
	}
	FindClose(HandleFile);
	return last;
}