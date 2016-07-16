#include "StringFunction.h"

char* StringFun_strlen(char* arg)
{
	char* RightArg = RetRightArg(arg);
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList))
	{
		if (ArgList->up == NULL)
		{
			sprintf(arg, "%d", strlen((char*)ArgList->value));
			return arg;
		}
		else
			return -1;
	}
	else
		return -1;
}