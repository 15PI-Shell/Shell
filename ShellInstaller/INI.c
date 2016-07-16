#include "INI.h"

int ReadInstConfig(char* PathToFolder, InstallerConfig* Config)
{
	DWORD StringRes;
	char str[1000];
	char str2[MAX_PATH + 1];
	int size = sizeof(str);
	int size2 = sizeof(str2);
	SingleListStringNode* list;
	char Path[MAX_PATH+1];
	strcpy(Path, PathToFolder);
	PathToFolder = Path;
	strcat(PathToFolder, "\\InstallerConfig.ini");
	StringRes = GetPrivateProfileStringA("Message", "Hellomsg", NULL, str, size, PathToFolder);
	if (StringRes == errno)
		return 1;
	else
	{
		char* mes = (char*)malloc(strlen(str));
		strcpy(mes, str);
		Config->msg = mes;
	}
	StringRes = GetPrivateProfileStringA("Icon", "IcoPath", NULL, str, size, PathToFolder);
	if (StringRes == errno)
		return 1;
	else
	{
		char* icon = (char*)malloc(strlen(str));
		strcpy(icon, str);
		Config->PathToIcon = icon;
	}
	Config->NumOfFiles = 0;
	StringRes = GetPrivateProfileStringA("Path", NULL, NULL, str, size, PathToFolder);
	if (StringRes == errno)
		return 1;
	else
	{
		int j = -1;
		for (int i = 0; i < StringRes; i++)
		{
			if (str[i] == 0)
			{
				str2[++j] = str[i];
				j = -1;
				GetPrivateProfileStringA("Path", str2, NULL, str2, size2, PathToFolder);
				SingleStrlistAddDownmost(&Config->FilePath, str2);
				Config->NumOfFiles++;
			}
			else
				str2[++j] = str[i];
		}
	}
	return 0;
}