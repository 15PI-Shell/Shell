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

	if (CheckNextSyms(inter, "if"))
	{
		int bkp = inter->scptr;
		inter->scptr += 2;
		SkipSpaces(inter);
		if (inter->scr[inter->scptr] == '(')
			inter->scptr++;
		else
		{
			inter->scptr = bkp;
			ProcAssignment(inter, VM);
			return;
		}
		int result = ProcCondition(inter, VM);

		SkipSpaces(inter);

		if (!CheckNextSyms(inter, "{"))
		{
			inter->scfailed = 1;
			return;
		}

		inter->scptr++;

		if (result)
		{
			inter->insideLevel++;
			inter->retBackTo[inter->insideLevel] = -1;//никуда не надо возвращаться
			return;
		}

		int delta = 1;
		while (inter->scr[inter->scptr] && delta != 0)
		{
			if (inter->scr[inter->scptr] == '{')
				delta++;
			else if (inter->scr[inter->scptr] == '}')
				delta--;
			inter->scptr++;
		}

		if (!inter->scr[inter->scptr] && inter->scr[inter->scptr - 1] != '}')
		{
			inter->scfailed = 1;
			return;
		}

		SkipSpaces(inter);

		if (CheckNextSyms(inter, "else"))
		{
			inter->scptr+=4;
			SkipSpaces(inter);
			if (!CheckNextSyms(inter, "{"))
			{
				inter->scfailed = 1;
				return;
			}

			inter->scptr++;
			inter->insideLevel++;
			inter->retBackTo[inter->insideLevel] = -1;//никуда не надо возвращаться
		}

		return;
	}

	if (CheckNextSyms(inter, "else"))
	{
		int bkp = inter->scptr;
		inter->scptr += 4;
		SkipSpaces(inter);
		if (!CheckNextSyms(inter, "{"))
		{
			inter->scptr = bkp;
			ProcAssignment(inter, VM);
			return;
		}

		inter->scptr++;

		int delta = 1;
		while (inter->scr[inter->scptr] && delta != 0)
		{
			if (inter->scr[inter->scptr] == '{')
				delta++;
			else if (inter->scr[inter->scptr] == '}')
				delta--;
			inter->scptr++;
		}

		return;
	}

	if (CheckNextSyms(inter, "while"))
	{
		int whilest = inter->scptr;
		inter->scptr += 5;
		SkipSpaces(inter);
		if (inter->scr[inter->scptr] == '(')
			inter->scptr++;
		else
		{
			inter->scptr = whilest;
			ProcAssignment(inter, VM);
			return;
		}
		int result = ProcCondition(inter, VM);

		SkipSpaces(inter);

		if (!CheckNextSyms(inter, "{"))
		{
			inter->scfailed = 1;
			return;
		}

		inter->scptr++;

		if (result)
		{
			inter->insideLevel++;
			inter->retBackTo[inter->insideLevel] = whilest;
			return;
		}

		int delta = 1;
		while (inter->scr[inter->scptr] && delta != 0)
		{
			if (inter->scr[inter->scptr] == '{')
				delta++;
			else if (inter->scr[inter->scptr] == '}')
				delta--;
			inter->scptr++;
		}

		if (!inter->scr[inter->scptr] && inter->scr[inter->scptr - 1] != '}')
		{
			inter->scfailed = 1;
			return;
		}

		return;
	}

	if (CheckNextSyms(inter, "}"))
	{
		if (inter->retBackTo[inter->insideLevel] != -1)
			inter->scptr = inter->retBackTo[inter->insideLevel];
		else
			inter->scptr++;
		inter->insideLevel--;
		if (inter->insideLevel < 0)
			inter->scfailed = 1;
		return;
	}

	ProcAssignment(inter, VM);
}

int EvalScript(char* script, int v)
{
	TrieNode* VM = VM_Create();//TODO: memleak
	InterpData data;

	data.verbose = v;
	data.row = data.scfailed = data.scptr = data.insideLevel = 0;
	data.scr = script;

	while (data.scr[data.scptr] && !data.scfailed)
	{
		Block(&data, VM);
		SkipSpaces(&data);
	}

	if (data.insideLevel > 0)
		data.scfailed = 1;

	if (data.scfailed)
		printf("Failed on %d line, near '%c'\n\n", data.row + 1, data.scr[data.scptr]);

	return !data.scfailed;
}