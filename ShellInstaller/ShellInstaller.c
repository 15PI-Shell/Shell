#include "ShellInstaller.h"

void MakeInstaller(char* pathWithFiles)
{
	printf("MakeInstaller logic: %s\n", pathWithFiles);
}

void Install()
{
	char copyInstalPath[MAX_PATH];
	printf("Installer logic\n");
	printf("Please, enter installation path: ");
	scanf("%s", &InstallationPath);
	//добавление пути в PATH
	//RegFunStatus = RegGetValueA(pathRegKey, 0, "PATH", RRF_RT_ANY, 0, BufPath, &SizeBufPath);
	strcpy(BufPath,getenv("PATH"));
	strcat(BufPath, ";");
	strcat(BufPath, InstallationPath);
	RegCloseKey(pathRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &pathRegKey);
	RegFunStatus = RegSetValueExA(pathRegKey, "PATH", NULL, REG_EXPAND_SZ, BufPath, sizeof(BufPath));
	if (RegFunStatus == ERROR_SUCCESS)
		printf("Update Path\n");
	else
	{
		printf("Updating PATH error\n");
		return;
	}
	RegCloseKey(pathRegKey);
	//Создание раздела 15PI-SHELL
	RegCloseKey(shellRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_CREATE_SUB_KEY | KEY_WOW64_64KEY, &shellRegKey);
	RegFunStatus = RegCreateKeyA(shellRegKey, "15PI-SHELL", &shellRegKey);
	if (RegFunStatus == ERROR_SUCCESS)
		printf("Section registry successfully created\n");
	else
	{
		printf("Creating a Registry Key Failed\n");
		return;
	}
	RegCloseKey(shellRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\15PI-SHELL", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &shellRegKey);
	//добавление DisplayIcon
	strcpy(copyInstalPath, InstallationPath);
	strcat(copyInstalPath, "\\15PI-SHELL.exe");
	RegSetValueExA(shellRegKey, "DisplayIcon", NULL, REG_SZ, copyInstalPath, strlen(copyInstalPath)+1);
	//добавление DisplayName
	RegSetValueExA(shellRegKey, "DisplayName", NULL, REG_SZ, "15PI-SHELL", sizeof("15PI-SHELL"));	
	//добавление InstallLocation
	RegSetValueExA(shellRegKey, "InstallLocation", NULL, REG_SZ, InstallationPath, strlen(InstallationPath)+1);	
	//добавление UninstallString
	strcpy(copyInstalPath, InstallationPath);
	strcat(copyInstalPath, "\\Uninstall.exe");
	RegSetValueExA(shellRegKey, "UninstallString", NULL, REG_SZ, copyInstalPath, strlen(copyInstalPath)+1);	
	//добавление Publisher 
	RegSetValueExA(shellRegKey, "Publisher", NULL, REG_SZ, "15PI", sizeof("15PI"));
	//добавление EstimatedSize
	DWORD size = 0xFFFFFF;
	RegSetValueExA(shellRegKey, "EstimatedSize", NULL, REG_DWORD, &size, sizeof(DWORD));
	printf("The data was successfully written to the registry\n");
}

void Uninstall()
{
	printf("Uninstaller logic\n");
	//удаление пути из PATH
	GetCurrentDirectoryA(sizeof(InstallationPath), InstallationPath);
	//strcpy(InstallationPath, "C:\\Game"); На проверку путь вводить без последнего слеша (GetCurrentDirectory получает такой путь)
	RegGetValueA(pathRegKey, 0, "PATH", RRF_RT_ANY, 0, BufPath, &SizeBufPath);
	char CurPath[MAX_PATH], copyPath[10000] = "";
	int i = -1, j = -1, CheckOnePath = 0;
	strcat(BufPath, ";");
	while (BufPath[++i])
	{
		if (BufPath[i] != ';')
			CurPath[++j] = BufPath[i];
		else
		{
			if (CurPath[j] != '\\')
				j++;
			CurPath[j] = 0;
			j = -1;
			if (strcmp(CurPath, InstallationPath) || CheckOnePath)
			{
				strcat(copyPath, CurPath);
				strcat(copyPath, ";");
			}
			else
				CheckOnePath = 1;
		}
	}
	copyPath[strlen(copyPath) - 1] = 0;
	RegCloseKey(pathRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &pathRegKey);
	RegFunStatus = RegSetValueExA(pathRegKey, "PATH", NULL, REG_EXPAND_SZ, copyPath, sizeof(copyPath));
	if (RegFunStatus == ERROR_SUCCESS)
		printf("Update PATH\n");
	else
	{
		printf("Updating PATH error\n");
		return;
	}
	RegCloseKey(pathRegKey);
	//удаление раздела из реестра
	RegCloseKey(shellRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &shellRegKey);
	RegFunStatus = RegDeleteTreeA(shellRegKey, "15PI-SHELL");
	if (RegFunStatus == ERROR_SUCCESS)
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
		RegFunStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &pathRegKey);
		if (RegFunStatus == ERROR_ACCESS_DENIED)
		{
			printf("Admin access required\n");
			return 0;
		}
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
