#include "toDouble.h"

char* toDouble(char* arg)
{
	SingleLinklistNode* sl;
	int k = ParsOfArgs(arg, &sl);
	if (!k || k > 1)
		return -1;
	return sl->value;
}