#include "ShellInstaller.h"

void MakeInstaller(char* pathWithFiles)
{
	printf("MakeInstaller logic: %s\n", pathWithFiles);
	Pack(pathWithFiles);
}

void Install()
{
	printf("Installer logic\n");
	printf("Please, enter installation path: ");
	scanf("%s", &InstallationPath);
	UnpackingFile(InstallationPath);
	/*InstallResult = InstallModifyPath();
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
	}*/
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
	//демо архива
	
	//
	/*InstallerConfig* Conf= (InstallerConfig*)malloc(sizeof(InstallerConfig));
	char PathToFolder[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(PathToFolder), PathToFolder);
	ReadInstConfig(PathToFolder, Conf);*/

	//SingleListStringNode* listOfFiles = 0;
	/*SingleStrlistAddDownmost(&listOfFiles, "StringList.c");
	SingleStrlistAddDownmost(&listOfFiles, "StringList.h");
	SingleStrlistAddDownmost(&listOfFiles, "ShellInstaller.c");
	SingleStrlistAddDownmost(&listOfFiles, "ShellInstaller.h");*/
	//char infoPart[] = {1,2,3,4,5,6,7,8,9,10};
	//CreateFileArch(listOfFiles, infoPart, 10);
	/*InstallerConfig* Conf = (InstallerConfig*)malloc(sizeof(InstallerConfig));
	Conf->FilePath = listOfFiles;
	Conf->msg = "Hello";
	Conf->NumOfFiles = 4;*/
	//Conf->PathToIcon = "fdfdf.c";
	//int s;
	//InfSect(&s,Conf);
	//return 0;

	if (1 == argc)
	{
		//открытие раздела Environment и проверка на запуск от администратора
		RegFunStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &pathRegKey);
		if (RegFunStatus == ERROR_ACCESS_DENIED)
		{
			printf("Admin access required\n");
			system("pause");
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
