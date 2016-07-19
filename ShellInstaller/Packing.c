#include "Packing.h"

void Pack(char* dir)
{
	InstallerConfig* Config=malloc(sizeof(Config));
	ReadInstConfig(dir, Config); 
	int size;
	char* mas;
	mas = InfSect(&size, Config);
	SingleListStringNode* FilesPaths;
	SingleListStringNode* ptr = FilesPaths;
	for (int i = 0; i < Config->NumOfFiles; i++)
	{
		strcpy(FilesPaths->value, dir);
		strcat(FilesPaths->value, Config->FilePath->value);
	}
	FilesPaths = ptr;
	CreateFileArch(FilesPaths, mas, size);

}

