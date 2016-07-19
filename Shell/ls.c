#include "ls.h"
FindFilesRecursive(char *Directory, char * mask, SingleLinklistNode **last)
{
	SingleLinklistNode *folders = NULL;
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



char* ls(char* args)
{
	SingleLinklistNode *ArgList = NULL;
	int n_args;
	char *Dir = (char*)malloc(MAX_PATH);
	char *mask = (char*)malloc(MAX_PATH);
	n_args = ParsOfArgs(args, &ArgList);
	SingleLinklistNode *last = NULL;
	switch (n_args)
	{
	case 1:	if (!strcmp(ArgList->value, "-R") || (!strcmp(ArgList->value, "-r")))
	{
		printf("list of arguments is wrong\n");
		free(mask); free(Dir);
		return -1;
	}
			if (strlen(ArgList->value) > MAX_PATH)
			{
				printf("list of arguments is wrong)\n");
				free(mask); free(Dir);
				return -1;
			}
			strcpy(Dir, ArgList->value);
			FindFilesAndDirsMask("*", Dir, 3, &last);
			while (last)
			{
				printf("%s\n", last->value);
				SingleLinklistRemoveDownmost(&last);
			}
			break;
	case 2: if (strlen(ArgList->value) > MAX_PATH)
	{
		printf("list of arguments is wrong)\n");
		free(mask); free(Dir);
		return -1;
	}
			if (strlen(ArgList->value) > MAX_PATH)
			{
				printf("list of arguments is wrong)\n");
				free(mask); free(Dir);
				return -1;
			}
			if (strcmp(ArgList->value, "-R") != 0 && (strcmp(ArgList->value, "-r") != 0))
			{
				strcpy(mask, ArgList->value);
				SingleLinklistRemoveDownmost(&ArgList);
				strcpy(Dir, ArgList->value);
				FindFilesAndDirsMask(mask, Dir, 3, &last);
			}
			else
			{
				SingleLinklistRemoveDownmost(&ArgList);
				printf("list of arguments is wrong)\n");
				free(mask); free(Dir);
				return -1;
			}
			while (last)
			{
				printf("%s\n", last->value);
				SingleLinklistRemoveDownmost(&last);
			}
			break;
	case 3:if (strlen(ArgList->value)>MAX_PATH)
	{
		printf("list of arguments is wrong)\n");
		free(mask); free(Dir);
		return -1;
	}
		   strcpy(mask, ArgList->value);
		   SingleLinklistRemoveDownmost(&ArgList);
		   if (strlen(ArgList->value)>MAX_PATH)
		   {
			   printf("list of arguments is wrong)\n");
			   free(mask); free(Dir);
			   return -1;
		   }
		   strcpy(Dir, ArgList->value);
		   SingleLinklistRemoveDownmost(&ArgList);
		   if (strcmp(ArgList->value, "-R") && (strcmp(ArgList->value, "-r")))
		   {
			   SingleLinklistRemoveDownmost(&ArgList);
			   printf("List of arguments is wrong\n");
			   free(mask); free(Dir);
			   return -1;
		   }
		   else
		   {
			   SingleLinklistRemoveDownmost(&ArgList);
			   FindFilesRecursive(Dir, mask, &last);
			   while (last)
			   {
				   printf("%s\n", last->value);
				   SingleLinklistRemoveDownmost(&last);
			   }
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