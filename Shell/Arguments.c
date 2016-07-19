#include "Arguments.h"

char* RetRightArg(TrieNode* VM, char* args, int* failed)
{
	InterpData data;
	data.row = data.scfailed = data.scptr = 0;
	data.scr = args;
	char* ans = ProcArgumets(&data, VM);
	*failed = data.scfailed;
	return ans;
}