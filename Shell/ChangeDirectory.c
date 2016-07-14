#include "ChangeDirectory.h"

char* ChangeDirectory(char* str)
{
	SingleListStringNode* dir;
	if (!ParsOfArgs(str, &dir))
		return -1;
	if (0 == dir)
		return -1;
	str = dir->value;
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
	if (DirectoryExists(str))
	{	
		strcpy(CurrentDirectory, str);
		return 0;		
	}
	else
	{
		printf("Directory doen't exist\n");
		return -1;
	}

}