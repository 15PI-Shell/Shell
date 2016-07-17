#include "Binary.h"

char* BinaryProc(char* arg)
{
	int a = 0, i = 0, b=0;
	int mas[100];

	sscanf(arg,"%d",&a);

	while (a != 0)
	{
		mas[i] = a % 2;
		a = a / 2;
		i++;
	}

	arg = (char*)malloc(sizeof(char)*(i+1));

	for (i; i > 0; i--)
	{
		b = b * 10 + mas[i-1];
	}
	
	sprintf(arg, "%d", b);
	return arg;
}
