#include "DeleteFiles.h"

void Delete()
{
	HANDLE hUnInstFile;
	DWORD ptr1;

	char* dir = (char*)malloc(300);
	GetCurrentDirectory(300, dir);

	hUnInstFile = CreateFileA(CurrentFile, GENERIC_WRITE,   //создаем файл в указанной директории
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hUnInstFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open uninstall.exe");
		return 0;
	}
	char sign[] = " HELL-INSTALLER-INFOSECTION";
	sign[0] = 'S';
	int lSign = strlen(sign);

	DWORD  dwBytesRead, dwBytesWritten, dwPos;
	char   buff[4096];
	int BuffSize = strlen(buff);
	int skp = KB_TO_SKIP + lSign + 1;
	int j;
	int i;
	do
	{
		if (ReadFile(hUnInstFile, buff, sizeof(buff), &dwBytesRead, NULL))
		{
			for (i = 0; i<dwBytesRead - lSign; i++)
			{
				j = 0;
				for (; j<lSign; j++)
				{
					if (buff[i + j] != sign[j]) break;
				}
				if (j == lSign) break;
			}
			if (i != dwBytesRead - lSign)
			{
				SetFilePointer(hUnInstFile, i - dwBytesRead, NULL, FILE_CURRENT);
				break;
			}
			else;
		}
		else break;
	} while (dwBytesRead == sizeof(buff));
	//сигнатура и 40 кб, считываем два байта
	char* buff3 = (char*)malloc(skp);
	ReadFile(hUnInstFile, buff3, skp, &dwBytesRead, NULL);

	int SkpTwoBytes = 2;
	int SkpFiveHBytes = 516;
	char buff1[2];
	unsigned char buff2[516];
	int NumOfFiles;
	ReadFile(hUnInstFile, buff1, sizeof(buff1), &dwBytesRead, NULL);
	NumOfFiles = buff1[0] * 256 + buff1[1];
	//считываем размер каждого файла и его путь
	int i1 = 0;
	SingleListStringNode * ListOfPaths = NULL;
	int k;
	while (i1<NumOfFiles)
	{
		k = 0;
		if (ReadFile(hUnInstFile, buff2, sizeof(buff2), &dwBytesRead, NULL))
		{
			char* PathFile = (char*)malloc(300);
			for (int j1 = 0;; k++, j1++)
			{
				PathFile[k] = buff2[j1];
				if (buff2[j1] == '\0') break;
			}
			SingleStrlistAddUpmost(&ListOfPaths, PathFile);	//добавляем пути к файлам в список
			i1++;
		}
	}
	
	while (ListOfPaths)
	{
		char * GetName;
		GetName = ListOfPaths->value;
		char* PathFileDir = (char*)malloc(MAX_PATH);
		strcpy(PathFileDir, dir);				//копируем нашу директорию в переменную
		strcat(PathFileDir, "\\");
		strcat(PathFileDir, GetName);
		remove(PathFileDir);
		ListOfPaths = ListOfPaths->up;
	}
	
	CloseHandle(hUnInstFile);
	return 1;

}