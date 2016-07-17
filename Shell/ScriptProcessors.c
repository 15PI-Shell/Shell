#include "ScriptProcessors.h"

void SetVar(InterpData* inter, char* name, void* currentVal, void* result, BPC_Returns varType, BPC_Returns termType, int mode)
{
	if (varType == BPC_ReturnsString)
	{
		if (termType != BPC_ReturnsString)
		{
			inter->scfailed = 1;
			return;
		}
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
		printf("Setting %s to \"%s\"\n", name, (char*)val);
		//VM_SetVariable(name, val);//не нужно?
	}
	else
	{
		if (termType == BPC_ReturnsString)
		{
			inter->scfailed = 1;//TODO: tostr conv
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

void ProcDeclaresAs(InterpData* inter, TrieNode* VM, BPC_Returns type)
{
	while (!CheckNextSyms(inter, ";") && !inter->scfailed)
	{
		char* name = ParseName(inter);
		VM_DeclareVariable(VM, name, 0, type);
		if (inter->scr[inter->scptr] == '=')
		{
			inter->scptr++;
			int constant;
			BPC_Returns termType, varType;
			void* result, *currentVal;
			GetTerm(inter, VM, ParseBeforeComa(inter), &result, &termType);
			currentVal = VM_GetVariable(VM, name, &constant, &varType);
			if (inter->scfailed)
				return;
			SetVar(inter, name, currentVal, result, varType, termType, 0);
		}
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
		if (!StringTermProc(inter, VM, result))
		{
			inter->scfailed = 1;
			return;
		}
		*type = BPC_ReturnsString;
	}
	//free(term) TODO: memleak
}

int StringTermProc(InterpData* inter, TrieNode* VM, void** result)
{
	SkipSpaces(inter);
	char* res = malloc(1000), *ret;
	*res = 0;
	if (inter->scr[inter->scptr] == '\"')
	{
		ret = ParseInsideQuotes(inter);
		strcpy(res, ret);
		inter->scptr += strlen(ret) + 1;
		free(ret);
		if (inter->scfailed)
			return 0;
	}
	else
	{
		BPC_Returns retType;
		//функция?
		char* funcname = ParseName(inter);
		if (inter->scfailed)
			return 0;
		if (inter->scr[inter->scptr] != '(')
		{
			int constant;
			ret = VM_GetVariable(VM, funcname, &constant, &retType);
			//TODO: tostr conv
			if (ret == 0)
			{
				inter->scfailed = 1;
				return 0;
			}
			strcpy(res, ret);
			if (inter->scfailed)
				return 0;
		}
		else
		{
			ret = ProcFunction(inter, VM, funcname, &retType);
			if (ret)
				strcpy(res, ret);
			free(ret);
			if (inter->scfailed)
				return 0;
		}
	}

	SkipSpaces(inter);
	char* cnc = 0;
	if (inter->scr[inter->scptr] == '+')
	{
		inter->scptr++;
		StringTermProc(inter, VM, &cnc);
	}
	if (cnc)
		strcat(res, cnc);
	free(cnc);
	*result = (void*)res;
	SkipSpaces(inter);
	return 1;
}

char* ProcArgumets(InterpData* inter, TrieNode* VM)
{
	BPC_Returns type;
	void* result;
	char* args = (char*)malloc(1000);
	char* tmp = (char*)malloc(1000);
	*args = 0;
	inter->scptr++;
	char* part = ParseBeforeComa(inter);
	int first = 1;
	do
	{
		GetTerm(inter, VM, part, &result, &type);
		if (inter->scfailed)
			return 0;
		switch (type)
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
	return args;
}

char* ProcFunction(InterpData* inter, TrieNode* VM, char* name, BPC_Returns* type)
{
	char* args = ProcArgumets(inter, VM);
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

	SetVar(inter, name, currentVal, result, varType, termType, mode);
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