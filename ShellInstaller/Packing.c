#include "Packing.h"

void Pack(char* dir)
{
	InstallerConfig* Config=malloc(sizeof(Config));
	ReadInstConfig(dir, Config); 
	int size;
	char* mas;
	SingleListStringNode* FileToPaths = NULL;// malloc(sizeof(SingleListStringNode*));
	SingleListStringNode* ptr = FileToPaths;
	SingleListStringNode* ptr1 = Config->FilePath;
	char* dir1 = dir;
	char* val = (char*)malloc(MAX_PATH);
	for (int i = 0; i < Config->NumOfFiles; i++)
	{
		strcpy(val, Config->FilePath->value);
		strcpy(Config->FilePath->value, dir);
		strcat(Config->FilePath->value, val);
		dir = dir1;
		SingleStrlistAddDownmost(&FileToPaths, Config->FilePath->value);
		Config->FilePath = Config->FilePath->up;
	//	FileToPaths = FileToPaths->up;
	}
	//FileToPaths = ptr;
	Config->FilePath = ptr1;
	mas = InfSect(&size, Config);
	CreateFileArch(FileToPaths, mas, size);

}

