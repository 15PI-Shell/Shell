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
	//
}

void Uninstall()
{
	printf("Uninstaller logic\n");
	//удаление пути из PATH
	GetCurrentDirectoryA(sizeof(InstallationPath), InstallationPath);
	//strcpy(InstallationPath, "C:\\Game"); На проверку путь вводить без последнего слеша (GetCurrentDirectory получает такой путь)
	RegGetValueA(pathRegKey, 0, "PATH", RRF_RT_ANY, 0, BufPath, &SizeBufPath);
	char CurPath[MAX_PATH], copyPath[10000]="";
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
	//
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
