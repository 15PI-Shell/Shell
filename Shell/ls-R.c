#include "ls-R.h"

FindFilesRecursive(char *Directory, char * mask, SingleLinklistNode **last)
{
	SingleLinklistNode *folders=NULL;
	FindFilesAndDirsMask(mask, Directory, 3, last);
	FindFilesAndDirsMask("*", Directory, 2, &folders);
	char *dircopy = (char*)malloc(MAX_PATH);
	strcpy(dircopy, Directory);
	while (folders)
	{
		strcat(Directory, "\\");
		strcat(Directory, folders->value);
		FindFilesRecursive(Directory, mask, last);
		SingleLinklistRemoveDownmost(&folders);
		strcpy(Directory, dircopy);
	}
	free(dircopy);
}

char* ls_R(char* args)
{
	SingleLinklistNode *ArgList = NULL;
	int n_args;
	char *Dir = (char*)malloc(MAX_PATH);
	char *mask = (char*)malloc(MAX_PATH);
	n_args = ParsOfArgs(args, &ArgList);
	SingleLinklistNode *last=NULL;
	switch (n_args)
	{
	case 1: strcpy(Dir, ArgList->value);
		FindFilesRecursive(Dir,"*",&last);
		while (last)
		{
			printf("%s\n",( char*)last->value);
			SingleLinklistRemoveDownmost(&last);
			
		}
		break;
	case 2: strcpy(mask, ArgList->value);
			SingleLinklistRemoveDownmost(&ArgList);
		strcpy(Dir, ArgList->value);
		SingleLinklistRemoveDownmost(&ArgList);
		FindFilesRecursive(Dir, mask, &last);
		while (last)
		{
			printf("%s\n", last->value);
			SingleLinklistRemoveDownmost(&last);
		}
		break;
	default:printf("List of arguments is wrong\n");
		free(mask); free(Dir);
		while(ArgList)
			SingleLinklistRemoveDownmost(&ArgList);
		return -1;
	}
	free(mask);
	free(Dir);
	return 0;
}