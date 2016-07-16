#include "cat.h"

char* cat(char* arg)
{
	FILE *fp = NULL;
	SingleLinklistNode *ARList=NULL;
	int n_ar;
	n_ar = ParsOfArgs(arg, &ARList);
	if (n_ar < 1)
	{
		printf("The number of arguments is not enough\n");
		return -1;
	}
	char *fileName = (char*)malloc(strlen(arg));
	strcpy(fileName, ARList->value);
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
		char c;
		while (!eof(fp))
		{
			fscanf(fp, "%c", &c);
			printf("%c", c);
		}
	}
	free(fileName);
	fclose(fp);
	return 0;
}