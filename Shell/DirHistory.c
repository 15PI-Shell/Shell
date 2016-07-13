#include "DirHistory.h"

char* GetAndCheckPathHistory()
{
	HKEY userRegKey;
	char user[100];
	DWORD sizeUser = sizeof(user);
	char HistoryDir[MAX_PATH] = "C:";
	RegOpenKeyExA(HKEY_CURRENT_USER, "Volatile Environment", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &userRegKey);
	RegGetValueA(userRegKey, 0, "HOMEPATH", RRF_RT_ANY, 0, user, &sizeUser);
	strcat(HistoryDir, user);
	//strcat(HistoryDir, "\\Documents\\15PI-SHELL");
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\15PI-SHELL", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &userRegKey);
	sizeUser = sizeof(user);
	RegGetValueA(userRegKey, 0, "History", RRF_RT_ANY, 0, user, &sizeUser);
	strcat(HistoryDir, user + strlen("%USERPROFILE%"));
	CreateDirectoryA(HistoryDir, NULL);
	return HistoryDir;
}
