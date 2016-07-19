#include "ChangeDirectory.h"

char* ChangeDirectory(char* str)
{
	SingleLinklistNode* dir=NULL;
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
	int i = -1;
	while (str[++i])
	{
		if (str[i] != '.')
		{
			i = -1;
			break;
		}
	}
	if (DirectoryExists(str) && i == -1 && str[0] != '\\')
	{
		strcpy(CurrentDirectory, str);
		return 0;
	}
	else
	{
		printf("Directory doesn't exist\n");
		return -1;
	}

}
