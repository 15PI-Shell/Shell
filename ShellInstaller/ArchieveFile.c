#include "ArchieveFile.h"

void PasteFile(HANDLE File, HANDLE pFile)
{
	DWORD  dwBytesRead, dwBytesWritten, dwPos;
	char   buff[4096];

	do
	{
		if (ReadFile(pFile, buff, sizeof(buff), &dwBytesRead, NULL))
		{
			dwPos = SetFilePointer(File, 0, NULL, FILE_END);
			LockFile(File, dwPos, 0, dwBytesRead, 0);
			WriteFile(File, buff, dwBytesRead, &dwBytesWritten, NULL);
			UnlockFile(File, dwPos, 0, dwBytesRead, 0);
		}
	} while (dwBytesRead == sizeof(buff));
}

int CreateFileArch(SingleListStringNode* FilesPaths, char * mas, int Size)
{
	DWORD NumOfBytes;
	HANDLE File, Paths, ShellFile;

	File = CreateFile("15PIShellInstaller.exe", GENERIC_READ || GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ShellFile = CreateFile("ShellInstaller.exe", GENERIC_READ || GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if ((File == INVALID_HANDLE_VALUE) || (ShellFile == INVALID_HANDLE_VALUE))
	{
		printf("Could not open 15PIShellInstaller.exe or ShellInstaller.exe");  // обработка ошибки 
		return 0;
	}
	PasteFile(File, ShellFile);
	CloseHandle(ShellFile);
	WriteFile(File, mas, Size, NumOfBytes, NULL);

	while (FilesPaths)
	{
		char * GetName;
		GetName = FilesPaths->value;

		Paths = CreateFile(GetName, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (Paths == INVALID_HANDLE_VALUE)
		{
			printf("Could not open %s", GetName);  // обработка ошибки 
			return 0;
		}

		PasteFile(File, Paths);
		CloseHandle(Paths);
		FilesPaths = FilesPaths->up;
	}
	CloseHandle(File);
	return 1;
}
