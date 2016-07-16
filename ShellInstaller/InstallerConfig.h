#pragma once

#include "StringList.h"

typedef struct InstallerConfig
{
	char* msg; //приветственное сообщение
	SingleListStringNode* FilePath;
	int NumOfFiles;
	char* PathToIcon;
} InstallerConfig;