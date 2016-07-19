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
	char *fileName = (char*)malloc(strlen(arg));
	strcpy(fileName, ARList->value);
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