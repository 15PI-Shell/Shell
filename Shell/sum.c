#include "sum.h"

char* sumProc(char* arg)
{
	int a=0, b=0;
	sscanf(arg, "%d%d", &a, &b);//опасно, но для теста хватит
	sprintf(arg, "%d", a + b);
	return arg;
}
