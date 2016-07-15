#include "cat.h"

char* cat(char* arg)
{
	FILE *fp = NULL;
	fp = fopen(arg, "r");
	if (fp == NULL)
	{
		printf("FILe Openning Error\n");
			return -1;
	}
	fseek(fp, 0, SEEK_END);
	long pos = ftell(fp);
	if (pos > 0)
	{
		char c;
		while (!feof(fp))
		{
			fscanf(fp, "%c", &c);
			printf("%c", c);
		}
	}
	else return 0;
	fclose(fp);
}