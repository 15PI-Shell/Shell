#include "TryCmd.h"

char* TryCmd(char* cmd)
{
	SingleLinklistNode* sl;
	int k = ParsOfArgs(cmd, &sl);
	if (!k || k > 1)
		return -1;
	return CmdInterpretator(sl->value);
}