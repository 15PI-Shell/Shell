#include "ReadLine.h"

char* ReadLine(char* arg)
{
	char* ret = malloc(1000);
	fgets(ret, 1000, stdin);
	ret[strlen(ret) - 1] = 0;
	return ret;
}