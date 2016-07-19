#include "StringFunction.h"

char* StringFun_strlen(char* arg)
{
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList))
	{
		if (ArgList->up == NULL)
		{
			arg = malloc(1000);
			sprintf(arg, "%d", strlen((char*)ArgList->value));
			return arg;
		}
		else
			return -1;
	}
	else
		return -1;
}