#include "ShellInstaller.h"

void MakeInstaller(char* pathWithFiles)
{
	printf("MakeInstaller logic: %s\n", pathWithFiles);
}

void Install()
{
	printf("Installer logic\n");
	printf("Please, enter installation path: ");
	scanf("%s", &InstallationPath);
	InstallResult = InstallModifyPath();
	if (InstallResult)
		printf("Update Path\n");
	else
	{
		printf("Updating PATH error\n");
		return;
	}
	InstallResult = InstallKeyRegistry();
	if (InstallResult)
		printf("Section registry successfully created\n");
	else
	{
		printf("Creating a Registry Key Failed\n");
		return;
	}
}

void Uninstall()
{
	printf("Uninstaller logic\n");
	InstallResult = UninstallModifyPath();
	if (InstallResult)
		printf("Update Path\n");
	else
	{
		printf("Updating PATH error\n");
		return;
	}
	InstallResult = UninstallKeyRegistry();
	if (InstallResult)
		printf("Data successfully deleted from the registry\n");
	else
	{
		printf("Unknown Error\n");
		return;
	}
}

int main(int argc, char* argv[])
{
	if (1 == argc)
	{
		//открытие раздела Environment и проверка на запуск от администратора
		RegFunStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &pathRegKey);
		if (RegFunStatus == ERROR_ACCESS_DENIED)
		{
			printf("Admin access required\n");
			return 0;
		}
		RegCloseKey(pathRegKey);
		//проверка на существование раздела с SHELL
		RegFunStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\15PI-SHELL", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &shellRegKey);
		if (RegFunStatus == ERROR_FILE_NOT_FOUND) //Если раздел не найден, установка
			Install();
		else if (RegFunStatus == ERROR_SUCCESS) //Если раздел найден, удаление
			Uninstall();
		else
			printf("%d registry error\n", RegFunStatus);
	}
	else if (3 == argc && 0 == strcmp(argv[1], "-mkinstaller"))
		MakeInstaller(argv[2]);

	system("pause");//press any key...
	return 0;
}
