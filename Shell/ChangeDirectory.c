#include "ChangeDirectory.h"

char* ChangeDirectory(char* str)
{
	if (DirectoryExists(str) && (strlen(str) <= MAX_PATH))
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