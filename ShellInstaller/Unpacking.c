#include "Unpacking.h"


int UnpackingFile(char* dir)
{
	HANDLE hFile, hUnInstFile;
	DWORD ptr1;
	hFile = CreateFileA(CurrentFile, GENERIC_READ,  //открываем наш архив
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open 15PIShellInstaller.exe");
		return 0;
	}

	int lDir = strlen(dir);
	char* UnInstFile = (char*)malloc(MAX_PATH);
	strcpy(UnInstFile,dir);				//копируем нашу директорию в переменную
	char* FileName = "\\uninstall.exe";
	strcat(UnInstFile + lDir, FileName,strlen(FileName));

	hUnInstFile = CreateFileA( UnInstFile, GENERIC_WRITE,   //создаем файл в указанной директории
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
	//читаем и записываем файл пока не встретим сигнатуру
	do
	{
		if (ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL))
		{						
			for ( i=0;i<dwBytesRead - lSign;i++)
			{
				 j = 0;
				for (; j<lSign;j++)
				{
					if (buff[i + j] != sign[j]) break;
				}
				if (j == lSign) break;
			}
			if (i != dwBytesRead - lSign)
			{
				WriteFile(hUnInstFile, buff, i, &dwBytesWritten, NULL);
				SetFilePointer(hFile, i - dwBytesRead, NULL, FILE_CURRENT);
				break;
			}
			else
			{
				WriteFile(hUnInstFile, buff, dwBytesRead, &dwBytesWritten, NULL);	//нет - записываем в файл
			}
		}
		else break;
	} while (dwBytesRead == sizeof(buff));
		//сигнатура и 40 кб, считываем два байта
	char* buff3=(char*)malloc(skp);
	if (ReadFile(hFile, buff3, skp, &dwBytesRead, NULL))
	{
		WriteFile(hUnInstFile, buff3, dwBytesRead, &dwBytesWritten, NULL);
	}

	int SkpTwoBytes = 2;
	int SkpFiveHBytes = 516;
	char buff1[2];
	unsigned char buff2[516];
	int NumOfFiles;
	if (ReadFile(hFile, buff1, sizeof(buff1), &dwBytesRead, NULL))
	{
		WriteFile(hUnInstFile, buff1, dwBytesRead, &dwBytesWritten, NULL);	
	}
	NumOfFiles = buff1[0]*256 + buff1[1];
	//считываем размер каждого файла и его путь
	int i1 = 0;
	SingleListStringNode * ListOfPaths = NULL;
	SingleListStringNode * ListOfSize = NULL;
	int k;
	while (i1<NumOfFiles)
	{
		k = 0;
		if (ReadFile(hFile, buff2, sizeof(buff2), &dwBytesRead, NULL))
		{
			char* PathFile=(char*)malloc(300);
			for (int j1 = 0;; k++, j1++)
			{
				PathFile[k] = buff2[j1];
				if (buff2[j1] == '\0') break;
			}
			
			SingleStrlistAddUpmost(&ListOfPaths, PathFile);	//добавляем пути к файлам в список
			
		//	char* folder;
		//	DirExist(ListOfPaths->value);
			unsigned int bmas[4] = {(int)buff2[512],(int)buff2[513],(int)buff2[514], (int)buff2[515] };
			unsigned int Size;
			Size = bmas[3] + (bmas[2]<<8) + (bmas[1]<<16) + (bmas[0]<<24);
			char* s = (char*)malloc(10);
			itoa(Size, s, 10);
			SingleStrlistAddUpmost(&ListOfSize, s);				//добавляем размер файлов в список
			WriteFile(hUnInstFile, buff2, dwBytesRead, &dwBytesWritten, NULL);
			i1++;
		}
	}
	CloseHandle(hUnInstFile);
	//распаковываем файлы
	while (ListOfPaths)
	{
		char * GetName;
		GetName = ListOfPaths->value;
		char* PathFileDir = (char*)malloc(MAX_PATH);
		strcpy(PathFileDir, dir);				//копируем нашу директорию в переменную
		strcat(PathFileDir, "\\");
		strcat(PathFileDir, GetName);

		HANDLE Paths;
		Paths = CreateFileA(PathFileDir, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		int ByteToSkip = atoi(ListOfSize->value);
		char* buff3=(char*)malloc(ByteToSkip);
		if (ReadFile(hFile, buff3, ByteToSkip, &dwBytesRead, NULL))
		{
			WriteFile(Paths, buff3, dwBytesRead, &dwBytesWritten, NULL);
		}
		CloseHandle(Paths);
		free(buff3);
		ListOfPaths = ListOfPaths->up;
		ListOfSize = ListOfSize->up;
	}
	CloseHandle(hFile);
	return 1;
}


