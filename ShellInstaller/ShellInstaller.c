#include "ShellInstaller.h"

void MakeInstaller(char* pathWithFiles)
{
	printf("MakeInstaller logic: %s\n", pathWithFiles);
}

HKEY shellRegKey;

void Install()
{
	printf("Installer logic\n");
}

void Uninstall()
{
	printf("Uninstaller logic\n");
}

int main(int argc, char* argv[])
{
	if (1 == argc)
	{
		LSTATUS kStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\15PI-Shell", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &shellRegKey);
		if (kStatus == ERROR_ACCESS_DENIED)
		{
			printf("Admin access required\n");
			return 0;
		}
		if (kStatus == ERROR_FILE_NOT_FOUND)
			Install();
		else if (kStatus == ERROR_SUCCESS)
			Uninstall();
		else
			printf("%d registry error\n", kStatus);
	}
	else if (3 == argc && 0 == strcmp(argv[1], "-mkinstaller"))
		MakeInstaller(argv[2]);

	system("pause");//press any key...
	return 0;
}
