#include "ScriptInterpreter.h"

void Block(InterpData* inter, TrieNode* VM)
{
	SkipSpaces(inter);
	if (0 == inter->scr[inter->scptr])
		return;

	if (CheckNextSyms(inter, "int "))
	{
		inter->scptr += 3;
		ProcDeclaresAs(inter, VM, BPC_ReturnsInt);
		if (!CheckNextSyms(inter, ";"))
			inter->scfailed = 1;//TODO: комменты к failed
		inter->scptr++;
		return;
	}

	if (CheckNextSyms(inter, "double "))
	{
		inter->scptr += 6;
		ProcDeclaresAs(inter, VM, BPC_ReturnsDouble);
		if (!CheckNextSyms(inter, ";"))
			inter->scfailed = 1;
		inter->scptr++;
		return;
	}

	if (CheckNextSyms(inter, "string "))
	{
		inter->scptr += 6;
		ProcDeclaresAs(inter, VM, BPC_ReturnsString);
		if (!CheckNextSyms(inter, ";"))
			inter->scfailed = 1;
		inter->scptr++;
		return;
	}

	if (CheckNextSyms(inter, "if "))
	{
		inter->scptr += 2;
		SkipSpaces(inter);
		if (inter->scr[inter->scptr] == '(')
			inter->scptr++;
		else
		{
			inter->scfailed = 1;
			return;
		}
		int result = ProcCondition(inter);
		return;
	}

	if (CheckNextSyms(inter, "while "))
	{
		inter->scptr += 5;
		SkipSpaces(inter);
		if (inter->scr[inter->scptr] == '(')
			inter->scptr++;
		else
		{
			inter->scfailed = 1;
			return;
		}
		int result = ProcCondition(inter);
		return;
	}
	if (CheckNextSyms(inter, "for "))
	{
		inter->scptr += 3;
		SkipSpaces(inter);

		return;
	}

	ProcAssignment(inter, VM);
}

int EvalScript(char* script)
{
	TrieNode* VM = Trie_Create();//TODO: memleak
	InterpData data;

	data.row = data.scfailed = data.scptr = 0;
	data.scr = script;

	while (data.scr[data.scptr] && !data.scfailed)
	{
		Block(&data, VM);
		SkipSpaces(&data);
	}

	if (data.scfailed)
		printf("Failed on %d line, near '%c'\n\n", data.row + 1, data.scr[data.scptr]);

	getch();

	return !data.scfailed;
}