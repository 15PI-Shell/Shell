#include "Unpacking.h"

int UnpackingFile(char* dir)
{
	HANDLE hFile, hUnInstFile;

	hFile = CreateFileA("15PIShellInstaller.exe", GENERIC_READ,  //открываем наш архив
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open 15PIShellInstaller.exe");
		return 0;
	}

	hUnInstFile = CreateFileA("uninstall.exe", GENERIC_WRITE,   //создаем файл
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hUnInstFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open uninstall.exe");
		return 0;
	}
	DWORD  dwBytesRead, dwBytesWritten, dwPos;
	char   buff[4096];

	do
	{
		if (ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL))
		{
			WriteFile(hUnInstFile, buff, dwBytesRead, &dwBytesWritten, NULL);
		}
	} while (dwBytesRead == sizeof(buff));


	return 1;
}
