#include "ChangeDirectory.h"

char* ChangeDirectory(char* str)
{
	SingleLinklistNode* dir = NULL;
	if (!ParsOfArgs(str, &dir))
		return -1;
	if (0 == dir)
		return -1;
	if (dir->up == NULL)
		str = dir->value;
	else
		return -1;
	if (!strcmp(str, ".."))
	{
		int iter = 0, slc = 0, lsl = 0;
		while (CurrentDirectory[iter])
		{
			if (CurrentDirectory[iter] == '\\')
			{
				slc++;
				lsl = iter;
			}
			iter++;
		}
		if (slc > 1)
			CurrentDirectory[lsl] = 0;
		else
			CurrentDirectory[lsl + 1] = 0;
		return 0;
	}
	if ((tolower(str[0]) >= 'a') && (tolower(str[0]) <= 'z') && (str[1] == ':'))
		if (DirectoryExists(str))
			strcpy(CurrentDirectory, str);
		else
		{
			printf("Directory doesn't exist\n");
			return -1;
		}
	else
	{
		int i = -1;
		while (str[++i])
		{
			if (str[i] != '.' && str[i] != '\\' && str[i] != '/')
			{
				i = -1;
				break;
			}
		}
		if (str[0] != '\\' && str[0] != '/' && i == -1)
		{
			char CurDir[MAX_PATH + 1];
			strcpy(CurDir, CurrentDirectory);
			if (CurDir[strlen(CurDir) - 1] != '\\' || CurDir[strlen(CurDir) - 1] != '/')
				strcat(CurDir, "\\");
			strcat(CurDir, str);
			if (DirectoryExists(CurDir))
				strcpy(CurrentDirectory, CurDir);
			else
			{
				printf("Directory doesn't exist\n");
				return -1;
			}
		}
		else
		{
			printf("Directory doesn't exist\n");
			return -1;
		}
	}
}