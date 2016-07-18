#include "ls.h"



char* ls(char* args)
{
	SingleLinklistNode *ArgList=NULL;
	int n_args;
	char *Dir = (char*)malloc(MAX_PATH);
	char *mask = (char*)malloc(MAX_PATH);
	n_args = ParsOfArgs(args, &ArgList);
	SingleLinklistNode *last=NULL;
	switch(n_args)
	{
		case 1: strcpy(Dir, ArgList->value);
			FindFilesAndDirsMask("*", Dir, 3, &last);
			while (last)
			{
				printf("%s\n", last->value);
				SingleLinklistRemoveDownmost(&last);
			}
			break;
		case 2: strcpy(mask, ArgList->value);
			ArgList = ArgList->up;
			strcpy(Dir, ArgList->value);
				FindFilesAndDirsMask(mask, Dir, 3, &last);
				while (last)
				{
					printf("%s\n", last->value);
					SingleLinklistRemoveDownmost(&last);
				}
			 break;
		default:printf("List of arguments is wrong\n");
			free(mask); free(Dir);
			return -1;
	}
	free(mask);
	free(Dir);
	return 0;
}