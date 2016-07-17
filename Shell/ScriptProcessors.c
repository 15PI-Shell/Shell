#include "ScriptProcessors.h"

void ProcDeclaresAs(InterpData* inter, TrieNode* VM, BPC_Returns type)
{
	while (!CheckNextSyms(inter, ";") && !inter->scfailed)
	{
		VM_DeclareVariable(VM, ParseName(inter), 0, type);
		if (inter->scr[inter->scptr] != ',' && inter->scr[inter->scptr] != ';')
			inter->scfailed = 1;
		if (inter->scr[inter->scptr] == ',')
			inter->scptr++;
	}
}

void GetTerm(InterpData* inter, TrieNode* VM, char* term, void** result, BPC_Returns *type)
{
	SkipSpaces(inter);
	term = MkMemCopy(term, strlen(term));
	if (MathInterpreter(term, result))
	{
		*type = BPC_ReturnsDouble;
		inter->scptr += strlen(term);
	}
	else
	{
		if (!StringTermProc(inter, VM, term, result))
		{
			inter->scfailed = 1;
			return;
		}
		*type = BPC_ReturnsString;
	}
	//free(term) TODO: memleak
}

int StringTermProc(InterpData* inter, TrieNode* VM, char* term, void** result)
{
	if (term[0] == '\"')
	{
		strtok(term, "\"");
		inter->scptr += strlen(term + 1) + 2;//+"Э
		*result = term + 1;
	}
	else
	{
		BPC_Returns ret;
		//функция?
		char* funcname = ParseName(inter);
		if (inter->scr[inter->scptr] != '(')//TODO: переменная ещё!
		{
			int constant;
			*result = VM_GetVariable(VM, funcname, &constant, &ret);
			if (inter->scfailed)
				return 0;
			return 1;
		}
		else
		{
			*result = ProcFunction(inter, VM, funcname, &ret);
		}
	}
	return 1;
}

char* ProcFunction(InterpData* inter, TrieNode* VM, char* name, BPC_Returns* type)
{
	void* result;
	char* args = (char*)malloc(1000);
	char* tmp = (char*)malloc(1000);
	*args = 0;
	inter->scptr++;
	char* part = ParseBeforeComa(inter);
	int first = 1;
	do
	{
		GetTerm(inter, VM, part, &result, type);
		if (inter->scfailed)
			return 0;
		switch (*type)
		{
		case BPC_ReturnsInt:
			sprintf(tmp, "%d", *((int*)result));
			break;
		case BPC_ReturnsDouble:
			sprintf(tmp, "%lf", *((double*)result));
			break;
		case BPC_ReturnsString:
			strcpy(tmp, "\"");
			strcat(tmp, (char*)result);
			strcat(tmp, "\"");
			break;
		}
		if (!first)
			strcat(args, " ");
		else
			first = 0;
		strcat(args, tmp);

		if (inter->scr[inter->scptr] == ',')
			inter->scptr++;
		part = ParseBeforeComa(inter);
	} while (*part);
	free(tmp);
	char* res = BPC_Execute(name, args, type);
	//TODO: mem leak args
	if (res >= 0)
	{
		SkipSpaces(inter);
		if (inter->scr[inter->scptr] == ')')
			inter->scptr++;
		else
		{
			inter->scfailed = 1;
			return 0;
		}
		return res;
	}
	else
	{
		inter->scfailed = 1;
		return 0;
	}
}

void ProcAssignment(InterpData* inter, TrieNode* VM)
{
	char* name = ParseName(inter);

	void* result;

	if (inter->scfailed)
		return;

	BPC_Returns termType;

	if (inter->scr[inter->scptr] == '(')
	{
		inter->scptr -= strlen(name);
		GetTerm(inter, VM, ParseBeforeSemi(inter), &result, &termType);
		if (inter->scr[inter->scptr] == ';')
			inter->scptr++;
		else
			inter->scfailed = 1;
		return;
	}

	BPC_Returns varType;
	int constant;

	void* currentVal = VM_GetVariable(VM, name, &constant, &varType);

	if (0 == currentVal)
		inter->scfailed = 1;//undeclared

	if (constant)
		inter->scfailed = 1;

	if (inter->scfailed)
		return;

	int mode;

	if (CheckNextSyms(inter, "="))
	{
		inter->scptr++;
		mode = 0;
	}
	else
		if (CheckNextSyms(inter, "+="))
		{
			inter->scptr += 2;
			mode = 1;
		}
		else
			if (CheckNextSyms(inter, "-="))
			{
				inter->scptr += 2;
				mode = 2;
			}
			else
				if (CheckNextSyms(inter, "*="))
				{
					inter->scptr += 2;
					mode = 3;
				}
				else
					if (CheckNextSyms(inter, "/="))
					{
						inter->scptr += 2;
						mode = 4;
					}

	SkipSpaces(inter);

	GetTerm(inter, VM, ParseBeforeSemi(inter), &result, &termType);
	if (inter->scr[inter->scptr] == ';')
		inter->scptr++;
	else
	{
		inter->scfailed = 1;
		return;
	}

	if (inter->scfailed)
		return;

	if (varType == BPC_ReturnsString)
	{
		char *val = (char*)currentVal;

		switch (mode)
		{
		case 0:
			strcpy(val, (char*)result);
			break;
		case 1:
			strcat(val, (char*)result);
			break;
		default:
			inter->scfailed = 1;
			return;
		}
		printf("Setting %s to %s\n", name, (char*)val);
		//VM_SetVariable(name, val);//не нужно?
	}
	else
	{
		if (termType == BPC_ReturnsString)
		{
			inter->scfailed = 1;
			return;
		}

		if (varType == BPC_ReturnsInt)
		{
			int toAssign = *((double*)result);
			int *val = (int*)currentVal;

			switch (mode)
			{
			case 0:
				*val = toAssign;
				break;
			case 1:
				*val += toAssign;
				break;
			case 2:
				*val -= toAssign;
				break;
			case 3:
				*val *= toAssign;
				break;
			case 4:
				if (!toAssign)
				{
					inter->scfailed = 1;
					return;
				}
				*val /= toAssign;
				break;
			}
			printf("Setting %s to %d\n", name, *((int*)val));
			//VM_SetVariable(name, val);//не нужно?
		}
		else if (varType == BPC_ReturnsDouble)
		{
			double toAssign = *((double*)result);
			double *val = (double*)currentVal;

			switch (mode)
			{
			case 0:
				*val = toAssign;
				break;
			case 1:
				*val += toAssign;
				break;
			case 2:
				*val -= toAssign;
				break;
			case 3:
				*val *= toAssign;
				break;
			case 4:
				if (!toAssign)
				{
					inter->scfailed = 1;
					return;
				}
				*val /= toAssign;
				break;
			}
			printf("Setting %s to %lf\n", name, *((double*)val));
			//VM_SetVariable(name, val);//не нужно?
		}
	}
}

char* ProcSingleCondition(InterpData* inter)
{
	int start = inter->scptr;
	while (!CheckNextSyms(inter, "&&") && !CheckNextSyms(inter, "||") && !CheckNextSyms(inter, ")"))
		inter->scptr++;

	char* data = (char*)malloc(inter->scptr - start + 1);
	strncpy(data, inter->scr + start, inter->scptr - start);
	data[inter->scptr - start] = 0;
	SkipSpaces(inter);

	return data;
}

//уже без (
int ProcCondition(InterpData* inter)
{
	SkipSpaces(inter);
	//void* Lresult, *Rresult;
	//BPC_Returns* Ltype, Rtype;
	//int from = scptr;
	char* parsed = ProcSingleCondition(inter);

	printf("Parsed condition element: %s\n", parsed);

	/*char* left = parsed;
	char* right = parsed;


	GetTerm(from, left, &Lresult, &Ltype);//TODO: избавится от from
	GetTerm(from, right, &Rresult, &Rtype);*/

	if (CheckNextSyms(inter, "&&"))
	{
		inter->scptr += 2;
		int rightPart = ProcCondition(inter);
		return 1;
	}
	else
		if (CheckNextSyms(inter, "||"))
		{
			inter->scptr += 2;
			int rightPart = ProcCondition(inter);
			return 1;
		}
		else
			if (CheckNextSyms(inter, ")"))
			{
				inter->scptr++;
				//последнее
				return 1;
			}
	inter->scfailed = 1;
	return 0;
}