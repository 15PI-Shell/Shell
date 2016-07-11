#include "Binary.h"

char* BinaryProc(char* arg)
{
	int a, i = 0, j = 0;
	char mas[100];

	sscanf(arg,"%d",&a);

	while (a / 2 != 1)
	{
		mas[i] = a % 2;
		a = a / 2;
		i++;
	}
	mas[i] = a % 2;
	i++;
	mas[i] = 1;
	j = i;

	while (i != -1)
	{
		*(arg + j-i) = mas[i];
		i--;
	}
	*(arg + j+1) = '\0';
	return arg;
}