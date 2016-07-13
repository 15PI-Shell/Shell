#include "ModifyingRegistry.h"



int InstallModifyPath()
{
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &pathRegKey);
	DWORD SizeBufPath = sizeof(BufPath);
	RegFunStatus = RegGetValueA(pathRegKey, 0, "PATH", RRF_RT_ANY, 0, BufPath, &SizeBufPath);
	strcat(BufPath, ";");
	strcat(BufPath, InstallationPath);
	RegCloseKey(pathRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &pathRegKey);
	RegFunStatus = RegSetValueExA(pathRegKey, "PATH", NULL, REG_EXPAND_SZ, BufPath, sizeof(BufPath));
	RegCloseKey(pathRegKey);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	return 1;
}

int InstallKeyRegistry()
{
	char user[100];
	char HistoryDir[MAX_PATH + 1] = "C:";
	DWORD sizeUser = sizeof(user);
	char copyInstalPath[MAX_PATH + 1];
	RegCloseKey(shellRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_CREATE_SUB_KEY | KEY_WOW64_64KEY, &shellRegKey);
	RegFunStatus = RegCreateKeyA(shellRegKey, "15PI-SHELL", &shellRegKey);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	RegCloseKey(shellRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\15PI-SHELL", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &shellRegKey);
	//добавление DisplayIcon
	strcpy(copyInstalPath, InstallationPath);
	strcat(copyInstalPath, "\\15PI-SHELL.exe");
	RegFunStatus = RegSetValueExA(shellRegKey, "DisplayIcon", NULL, REG_SZ, copyInstalPath, strlen(copyInstalPath) + 1);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//добавление DisplayName
	RegFunStatus = RegSetValueExA(shellRegKey, "DisplayName", NULL, REG_SZ, "15PI-SHELL", sizeof("15PI-SHELL"));
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//добавление InstallLocation
	RegFunStatus = RegSetValueExA(shellRegKey, "InstallLocation", NULL, REG_SZ, InstallationPath, strlen(InstallationPath) + 1);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//добавление UninstallString
	strcpy(copyInstalPath, InstallationPath);
	strcat(copyInstalPath, "\\ShellInstaller.exe");
	RegFunStatus = RegSetValueExA(shellRegKey, "UninstallString", NULL, REG_SZ, copyInstalPath, strlen(copyInstalPath) + 1);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//добавление Publisher 
	RegFunStatus = RegSetValueExA(shellRegKey, "Publisher", NULL, REG_SZ, "15PI", sizeof("15PI"));
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//добавление EstimatedSize
	DWORD size = 0xFFFFFF;
	RegFunStatus = RegSetValueExA(shellRegKey, "EstimatedSize", NULL, REG_DWORD, &size, sizeof(size));
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//добавление History (путь к папке с историей)
	RegOpenKeyExA(HKEY_CURRENT_USER, "Volatile Environment", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &userRegKey);
	RegGetValueA(userRegKey, 0, "HOMEPATH", RRF_RT_ANY, 0, user, &sizeUser);
	strcat(HistoryDir, user);
	strcat(HistoryDir, "\\Documents\\15PI-SHELL");
	RegFunStatus = RegSetValueExA(shellRegKey, "History", NULL, REG_SZ, HistoryDir, strlen(HistoryDir)+1);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//создание папки в документах для истории
	int FunRes = CreateDirectoryA(HistoryDir, NULL);
	if (!FunRes)
		return 0;
	return 1;
}

int UninstallModifyPath()
{
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &pathRegKey);
	DWORD SizeBufPath = sizeof(BufPath);
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
	RegCloseKey(pathRegKey);
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	return 1;
}

int UninstallKeyRegistry()
{
	char user[100];
	char HistoryDir[MAX_PATH + 1] = "C:";
	DWORD sizeUser = sizeof(user);
	RegCloseKey(shellRegKey);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &shellRegKey);
	RegFunStatus = RegDeleteTreeA(shellRegKey, "15PI-SHELL");
	if (RegFunStatus != ERROR_SUCCESS)
		return 0;
	//удаление папки с историей
	RegOpenKeyExA(HKEY_CURRENT_USER, "Volatile Environment", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &userRegKey);
	RegGetValueA(userRegKey, 0, "HOMEPATH", RRF_RT_ANY, 0, user, &sizeUser);
	strcat(HistoryDir, user);
	strcat(HistoryDir, "\\Documents\\15PI-SHELL");
	int FunRes = RemoveDirectoryA(HistoryDir);
	if (!FunRes)
		return 0;
	return 1;
}