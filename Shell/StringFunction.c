#include "StringFunction.h"

char* StringFun_strlen(char* arg)
{
	int ptr = -1;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList))
	{
		while (arg[++ptr] == ' ');
			if (arg[ptr] == '\"')
			{
				sprintf(arg, "%d", strlen((char*)ArgList->value));
				return arg;
			}
			else
			{
				//Здесь должно быть определение переменных строкового типа и работа с ними
				sprintf(arg, "%d", strlen("Test"));
				return arg;
			}
	}
	else
		return -1;
}