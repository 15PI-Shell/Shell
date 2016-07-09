#include "FindFile.h"
#include "Execute.h"


STRLIST_NODE* FindFiles(char* PrefixFile)
{
	WIN32_FIND_DATA FileData;
	STRLIST_NODE *last = NULL;
	char * pathPTR;
	int i = -1, j = -1;
	HANDLE HandleFile;

	//GetCurrentDirectoryA(sizeof(CurrentDirectory), CurrentDirectory); //получение текущей директории 
	strcat(CurrentDirectory, "\\"); //создаем путь для функции
	strcat(CurrentDirectory, PrefixFile); //добавляем наш префикс
	HandleFile = FindFirstFileA(CurrentDirectory, &FileData); //пытаемся получить дескриптор файла. В FileData отправляются данные файла
	if (HandleFile != INVALID_HANDLE_VALUE) //если подходящий файл или папка найдены
	{
		//printf("\n%s\n", CurrentDirectory);
		do
		{
				//printf("%s\n", FileData.cFileName);
				StrlistAdd(&last, FileData.cFileName); //добавляем имя файла/папки в список
		} while (FindNextFileA(HandleFile, &FileData) != 0); //продолжаем поиск
	}
	FindClose(HandleFile);

	pathPTR = getenv("PATH");

	while (pathPTR[++i]) //парсим PATH
	{
		if (pathPTR[i] != ';') //проходим все пути, указанные в PATH и ищем походящие файлы
		{
			CurrentDirectory[++j] = pathPTR[i];
		}

		else
		{
			CurrentDirectory[j] = 0;
			j = -1;
			strcat(CurrentDirectory, "\\");
			strcat(CurrentDirectory, PrefixFile);
			strcat(CurrentDirectory, "*");
			HandleFile = FindFirstFileA(CurrentDirectory, &FileData);
			if (HandleFile != INVALID_HANDLE_VALUE)
			{
				//printf("\n%s\n", CurrentDirectory);
				do
				{
						//printf("%s\n", FileData.cFileName);
						StrlistAdd(&last, FileData.cFileName);
				} while (FindNextFileA(HandleFile, &FileData) != 0);
			}
		}
	}
	FindClose(HandleFile);
	return last;
}