#pragma once

#include "StringList.h"

typedef struct InstallerConfig
{
	char* msg; //�������������� ���������
	SingleListStringNode* FilePath;
	int NumOfFiles;
	char* PathToIcon;
} InstallerConfig;