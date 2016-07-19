#include "ScriptParsers.h"

int CheckNextSymsFree(int ptr, char* where, char* syms)
{
	int len = strlen(syms);
	for (int i = 0; i < len; ++i)
		if (0 == where[ptr + i] || where[ptr + i] != syms[i])
			return 0;
	return 1;
}

int CheckNextSyms(InterpData* inter, char* syms)
{
	int len = strlen(syms);
	for (int i = 0; i < len; ++i)
		if (0 == inter->scr[inter->scptr + i] || inter->scr[inter->scptr + i] != syms[i])
			return 0;
	return 1;
}

void SkipSpaces(InterpData* inter)
{
	int changed = 0;
	while ((inter->scr[inter->scptr] == ' ' || inter->scr[inter->scptr] == '\t' || inter->scr[inter->scptr] == '\n' || inter->scr[inter->scptr] == '\r'))
	{
		if (inter->scr[inter->scptr] == '\n')
			inter->row++;
		inter->scptr++;
		changed = 1;
	}

	if (CheckNextSyms(inter, "//"))
	{
		inter->scptr += 2;
		while (inter->scr[inter->scptr] && inter->scr[inter->scptr] != '\n')
			inter->scptr++;
		inter->row++;
		if (inter->scr[inter->scptr])
			inter->scptr++;
		changed = 1;
	}

	if (CheckNextSyms(inter, "/*"))
	{
		inter->scptr += 2;
		while (!CheckNextSyms(inter, "*/"))
		{
			if (inter->scr[inter->scptr] == '\n')
				inter->row++;
			inter->scptr++;
		}
		if (inter->scr[inter->scptr] && inter->scr[inter->scptr + 1])
			inter->scptr += 2;
		else
			inter->scfailed = 1;
		changed = 1;
	}

	if (changed)
		SkipSpaces(inter);
}

char* ParseBeforeComa(InterpData* inter, int delta)
{
	SkipSpaces(inter);

	int start = inter->scptr;
	int insidePtr = inter->scptr;

	while (inter->scr[insidePtr])
	{
		if (inter->scr[insidePtr] == '(')
			delta++;
		else if (inter->scr[insidePtr] == ')')
		{
			delta--;
			if (delta < 0)
				break;
		}
		if (inter->scr[insidePtr] == ',' && delta == 0)
			break;
		insidePtr++;
	}

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, inter->scr + start, insidePtr - start);
	data[insidePtr - start] = 0;

	return data;
}

char* ParseBeforeSemi(InterpData* inter)
{
	SkipSpaces(inter);

	int start = inter->scptr;
	int insidePtr = inter->scptr;

	while (inter->scr[insidePtr] && (inter->scr[insidePtr] != ';'))
		insidePtr++;

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, inter->scr + start, insidePtr - start);
	data[insidePtr - start] = 0;

	return data;
}

char* ParseName(InterpData* inter)
{
	SkipSpaces(inter);

	if (!((inter->scr[inter->scptr] >= 'A' && inter->scr[inter->scptr] <= 'Z') || (inter->scr[inter->scptr] >= 'a' && inter->scr[inter->scptr] <= 'z')))
	{
		inter->scfailed = 1;
		return 0;
	}

	int start = inter->scptr;
	while ((inter->scr[inter->scptr] >= 'A' && inter->scr[inter->scptr] <= 'Z') || (inter->scr[inter->scptr] >= 'a' && inter->scr[inter->scptr] <= 'z') || (inter->scr[inter->scptr] >= '0' && inter->scr[inter->scptr] <= '9'))
		inter->scptr++;
	char* varname = (char*)malloc(inter->scptr - start + 1);
	strncpy(varname, inter->scr + start, inter->scptr - start);
	varname[inter->scptr - start] = 0;

	SkipSpaces(inter);

	return varname;
}

char* ParseInsideBrackets(InterpData* inter)
{
	inter->scptr++;//пропускаем (
	int insidePtr = inter->scptr;
	int delta = 1, start = insidePtr;
	while (delta != 0 && inter->scr[insidePtr])
	{
		insidePtr++;
		if (inter->scr[insidePtr] == '(')
			delta++;
		else if (inter->scr[insidePtr] == ')')
			delta--;
	}

	if (delta != 0)
	{
		inter->scfailed = 1;
		return 0;
	}

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, inter->scr + start, insidePtr - start);
	data[insidePtr - start] = 0;
	insidePtr++;

	return data;
}

char* ParseInsideQuotes(InterpData* inter)
{
	inter->scptr++;//пропускаем (
	int insidePtr = inter->scptr;
	int start = insidePtr;
	while (inter->scr[insidePtr])
	{
		if (inter->scr[insidePtr] == '"')
				break;
		insidePtr++;
	}

	if (!inter->scr[insidePtr])
	{
		inter->scfailed = 1;
		return 0;
	}

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, inter->scr + start, insidePtr - start);
	data[insidePtr - start] = 0;

	return data;
}