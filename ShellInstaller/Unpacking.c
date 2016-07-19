#include "Unpacking.h"

char* DirExist(char* path)
{
	char* folder;
	int i = 0;
	int len = 0;
	while (path)
	{ 
		len++;
		if (path[i] == "\\")
		{
			strncpy(folder,path,len);
			return folder;
		}
	}
	return 0;
}

int UnpackingFile(char* dir)
{
	HANDLE hFile, hUnInstFile;
	DWORD ptr1;
	hFile = CreateFileA("15PIShellInstaller.exe", GENERIC_READ,  //открываем наш архив
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
	int skp = KB_TO_SKIP + lSign;
	int j;
	int i;
	do
	{
		if (ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL))
		{
			char* ptr;							
			for ( i=0;i<dwBytesRead -lSign;i++)
			{
				 j = 0;
				for (;j<lSign;j++)
				{
					if (buff[i + j] != sign[j]) break;
				}
				if (j == lSign) break;
			}
			if (i != dwBytesRead - lSign)
			{
				SetFilePointer(hFile, i + lSign, NULL, FILE_CURRENT);
				break;
			}
			else
			{
				WriteFile(hUnInstFile, buff, dwBytesRead, &dwBytesWritten, NULL);	//нет - записываем в файл
			}
		}
		else break;
	} while (1);
	
	//начиная с ptr пропускаем сигнатуру и 40 кб считываем два байта
	SetFilePointer( hFile, skp, NULL, FILE_CURRENT);
	int SkpTwoBytes = 2;
	int SkpFiveHBytes = 516;
	char buff1[2];
	char buff2[516];
	int NumOfFiles;
	if (ReadFile(hFile, buff1, sizeof(buff1), &dwBytesRead, NULL))
	{
		WriteFile(hUnInstFile, buff1, dwBytesRead, &dwBytesWritten, NULL);	
		SetFilePointer(hFile, SkpTwoBytes, NULL, FILE_CURRENT);
	}
	NumOfFiles = buff1[1]*256 + buff1[0];
	//считываем размер каждого файла и его путь
	int i = 0;
	SingleListStringNode * ListOfPaths;
	SingleListStringNode * ListOfSize;
	int k = 0;
	while (i<NumOfFiles)
	{
		if (ReadFile(hFile, buff2, sizeof(buff2), &dwBytesRead, NULL))
		{
			char* PathFile;
			for (int j = 0; buff2[j] != "\0"; k++, j++)
			{
				PathFile[k] = buff2[j];
			}
			PathFile[k] = "\0";
			SingleStrlistAddUpmost(ListOfPaths, PathFile);	//добавляем пути к файлам в список
			
		//	char* folder;
		//	DirExist(ListOfPaths->value);
			
			int Size;
			Size = buff2[515] + (buff2[514]<<8) + (buff2[513]<<16) + (buff2[512]<<24);
			char* s;
			itoa(Size, s, 10);
			SingleStrlistAddUpmost(ListOfSize, s);				//добавляем размер файлов в список
			WriteFile(hUnInstFile, buff2, dwBytesRead, &dwBytesWritten, NULL);
			SetFilePointer(hFile, SkpFiveHBytes, NULL, FILE_CURRENT);
			i++;
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
		strcat(PathFileDir, GetName);

		HANDLE Paths;
		Paths = CreateFile(GetName, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		int ByteToSkip = atoi(ListOfSize->value);
		char* buff3=(char*)malloc(ByteToSkip);
		if (ReadFile(hFile, buff3, sizeof(buff3), &dwBytesRead, NULL))
		{
			WriteFile(Paths, buff3, dwBytesRead, &dwBytesWritten, NULL);
			SetFilePointer(hFile, ByteToSkip, NULL, FILE_CURRENT);
		}
		CloseHandle(Paths);
		free(buff3);
		ListOfPaths = ListOfPaths->up;
		ListOfSize = ListOfSize->up;
	}
	CloseHandle(hFile);
	return 1;
}


