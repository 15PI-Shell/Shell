#include "prFile.h"

char* printFile(char* arg)
{
	FILE *fp = NULL;
	SingleLinklistNode *ARList=NULL;
	int n_ar;
	n_ar = ParsOfArgs(arg, &ARList);
	if (n_ar!= 1)
	{
		printf("The number of arguments is wrong\n");
		return -1;
	}
	if (strlen(ARList->value) > MAX_PATH)
	{
		printf("list of arguments is wrong\n");
		SingleLinklistRemoveDownmost(&ARList);
		return -1;
	}
	char *fileName = (char*)malloc(MAX_PATH + 1);
	strcpy(fileName, ARList->value);
	char *slesh=NULL;
	slesh= strstr(fileName, "\\");
	if (slesh == NULL)
	{
		int len = strlen(fileName);
		int lenP = strlen(CurrentDirectory);
		if (lenP + len < MAX_PATH + 1)
		{
			char *tmp= (char*)malloc(MAX_PATH + 1);
			strcpy(tmp, CurrentDirectory);
			strcat(tmp, "\\");
			strcat(tmp, fileName);
			strcpy(fileName, tmp);
			free(tmp);
		}
		else
		{
			printf("File's name is wrong\n");
			free(fileName);
			return -1;

		}
	}

	SingleLinklistRemoveDownmost(&ARList);
	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("FILe Openning Error\n");
			return -1;
	}
	fseek(fp, 0, SEEK_END);
	long pos = ftell(fp);
	if (pos > 0)
	{
		rewind(fp);
		char *str=(char*)malloc(300);
		while (fgets(str, 300,fp))
		{
			printf("%s", str);
		}
		free(str);
	}
	else
	{
		printf("File is empty\n");
	}
	free(fileName);
	fclose(fp);
	return 0;
}