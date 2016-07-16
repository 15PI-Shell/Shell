#include "ScriptInterpreter.h"

char* scr;
int scptr, scfailed, row, coloumn;

char* ParseName();
char* ParseBeforeSemi();
char* ParseInsideBrackets();
char* ProcFunction(TrieNode* VM, char* name, BPC_Returns* type);
int ProcCondition();

void IncPtr()
{
	scptr++;
	coloumn++;
}

int CheckNextSyms(char* syms)
{
	int len = strlen(syms);
	for (int i = 0; i < len; ++i)
		if (0 == scr[scptr] || scr[scptr + i] != syms[i])
			return 0;
	return 1;
}

void SkipSpaces()
{
	int changed = 0;
	while ((scr[scptr] == ' ' || scr[scptr] == '\t' || scr[scptr] == '\n' || scr[scptr] == '\r'))
	{
		if (scr[scptr] == '\n')
			row++, coloumn = 0;
		IncPtr();
		changed = 1;
	}

	if (CheckNextSyms("//"))
	{
		scptr += 2;
		while (scr[scptr] && scr[scptr] != '\n')
			IncPtr();
		row++, coloumn = 0;
		if (scr[scptr])
			IncPtr();
		changed = 1;
	}

	if (CheckNextSyms("/*"))
	{
		scptr += 2;
		while (!CheckNextSyms("*/"))
		{
			if (scr[scptr] == '\n')
				row++, coloumn = 0;
			IncPtr();
		}
		if (scr[scptr] && scr[scptr + 1])
			scptr += 2;
		else
			scfailed = 1;
		changed = 1;
	}

	if (changed)
		SkipSpaces();
}

int StringTermProc(TrieNode* VM, char* term, void** result)
{
	if (term[0] == '\"')
	{
		strtok(term, "\"");
		scptr += strlen(term + 1) + 2;//+"Э
		*result = term + 1;
	}
	else
	{
		BPC_Returns ret;
		//функция?
		char* funcname = ParseName();
		if (scr[scptr] != '(')//TODO: переменная ещё!
		{
			int constant;
			VM_GetVariable(VM, funcname, &constant, &ret);
			scfailed = 1;
			return 0;
		}
		else
		{
			*result = ProcFunction(VM, funcname, &ret);
		}
	}
	return 1;
}

void GetTerm(TrieNode* VM, char* term, void** result, BPC_Returns *type)
{
	SkipSpaces();
	term = MkMemCopy(term, strlen(term));
	if (MathInterpreter(term, result))
	{
		*type = BPC_ReturnsDouble;
		scptr += strlen(term);
	}
	else
	{
		if (!StringTermProc(VM, term, result))
		{
			scfailed = 1;
			return;
		}
		*type = BPC_ReturnsString;
	}
	//free(term) TODO: memleak
}

char* ParseBeforeComa()
{
	SkipSpaces();

	int start = scptr;
	int insidePtr = scptr;

	int delta = 0;

	while (scr[insidePtr])
	{
		if (scr[insidePtr] == '(')
			delta++;
		else if (scr[insidePtr] == ')')
			delta--;
		if ((scr[insidePtr] == ',' && delta == 0) || delta < 0)
			break;
		insidePtr++;
	}

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, scr + start, insidePtr - start);
	data[insidePtr - start] = 0;

	return data;
}

char* ProcFunction(TrieNode* VM, char* name, BPC_Returns* type)
{
	void* result;
	char* args = (char*)malloc(1000);
	char* tmp = (char*)malloc(1000);
	*args = 0;
	char* inside = ParseInsideBrackets();
	char* part = ParseBeforeComa(inside);
	int first = 1;
	do
	{
		GetTerm(VM, part, &result, type);
		if (scfailed)
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

		if (scr[scptr] == ',')
			scptr++;
		part = ParseBeforeComa();
	} while (*part);
	free(tmp);
	char* res = BPC_Execute(name, args, type);
	//TODO: mem leak args
	if (res >= 0)
	{
		SkipSpaces();
		if (scr[scptr] == ')')
			IncPtr();
		else
		{
			scfailed = 1;
			return 0;
		}
		return res;
	}
	else
	{
		scfailed = 1;
		return 0;
	}
}

void ProcAssignment(TrieNode* VM)
{
	char* name = ParseName();

	void* result;

	if (scfailed)
		return;

	BPC_Returns termType;

	if (scr[scptr] == '(')
	{
		scptr -= strlen(name);
		GetTerm(VM, ParseBeforeSemi(), &result, &termType);
		if (scr[scptr] == ';')
			IncPtr();
		else
			scfailed = 1;
		return;
	}

	BPC_Returns varType;
	int constant;

	void* currentVal = VM_GetVariable(VM, name, &constant, &varType);

	if (constant)
		scfailed = 1;

	if (scfailed)
		return;

	int mode;

	if (CheckNextSyms("="))
	{
		IncPtr();
		mode = 0;
	}
	else
		if (CheckNextSyms("+="))
		{
			scptr += 2;
			mode = 1;
		}
		else
			if (CheckNextSyms("-="))
			{
				scptr += 2;
				mode = 2;
			}
			else
				if (CheckNextSyms("*="))
				{
					scptr += 2;
					mode = 3;
				}
				else
					if (CheckNextSyms("/="))
					{
						scptr += 2;
						mode = 4;
					}

	SkipSpaces();

	GetTerm(ParseBeforeSemi(), &result, &termType);
	if (scr[scptr] == ';')
		IncPtr();
	else
	{
		scfailed = 1;
		return;
	}

	if (scfailed)
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
			scfailed = 1;
			return;
		}
		printf("Setting %s to %s\n", name, (char*)val);
		//VM_SetVariable(name, val);//не нужно?
	}
	else
	{
		if (termType == BPC_ReturnsString)
		{
			scfailed = 1;
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
					scfailed = 1;
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
					scfailed = 1;
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

char* ParseBeforeSemi()
{
	SkipSpaces();

	int start = scptr;
	int insidePtr = scptr;

	while (scr[insidePtr] && (scr[insidePtr] != ';'))
		insidePtr++;

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, scr + start, insidePtr - start);
	data[insidePtr - start] = 0;

	return data;
}

char* ParseName()
{
	SkipSpaces();

	if (!((scr[scptr] >= 'A' && scr[scptr] <= 'Z') || (scr[scptr] >= 'a' && scr[scptr] <= 'z')))
	{
		scfailed = 1;
		return 0;
	}

	int start = scptr;
	while ((scr[scptr] >= 'A' && scr[scptr] <= 'Z') || (scr[scptr] >= 'a' && scr[scptr] <= 'z') || (scr[scptr] >= '0' && scr[scptr] <= '9'))
		IncPtr();
	char* varname = (char*)malloc(scptr - start + 1);
	strncpy(varname, scr + start, scptr - start);
	varname[scptr - start] = 0;

	SkipSpaces();

	return varname;
}

char* ParseInsideBrackets()
{
	IncPtr();//пропускаем (
	int insidePtr = scptr;
	int delta = 1, start = insidePtr;
	while (delta != 0 && scr[insidePtr])
	{
		insidePtr++;
		if (scr[insidePtr] == '(')
			delta++;
		else if (scr[insidePtr] == ')')
			delta--;
	}

	if (delta != 0)
	{
		scfailed = 1;
		return 0;
	}

	char* data = (char*)malloc(insidePtr - start + 1);
	strncpy(data, scr + start, insidePtr - start);
	data[insidePtr - start] = 0;
	insidePtr++;

	return data;
}

char* ParseSingleCondition()
{
	int start = scptr;
	while (!CheckNextSyms("&&") && !CheckNextSyms("||") && !CheckNextSyms(")"))
		IncPtr();

	char* data = (char*)malloc(scptr - start + 1);
	strncpy(data, scr + start, scptr - start);
	data[scptr - start] = 0;
	SkipSpaces();

	return data;
}

//уже без (
int ProcCondition()
{
	SkipSpaces();
	//void* Lresult, *Rresult;
	//BPC_Returns* Ltype, Rtype;
	//int from = scptr;
	char* parsed = ParseSingleCondition();

	printf("Parsed condition element: %s\n", parsed);

	/*char* left = parsed;
	char* right = parsed;


	GetTerm(from, left, &Lresult, &Ltype);//TODO: избавится от from
	GetTerm(from, right, &Rresult, &Rtype);*/

	if (CheckNextSyms("&&"))
	{
		scptr += 2;
		int rightPart = ProcCondition();
		return 1;
	}
	else
		if (CheckNextSyms("||"))
		{
			scptr += 2;
			int rightPart = ProcCondition();
			return 1;
		}
		else
			if (CheckNextSyms(")"))
			{
				IncPtr();
				//последнее
				return 1;
			}
	scfailed = 1;
	return 0;
}

void Block(TrieNode* VM)
{
	SkipSpaces();
	if (0 == scr[scptr])
		return;

	if (CheckNextSyms("if"))
	{
		scptr += 2;
		SkipSpaces();
		if (scr[scptr] == '(')
			IncPtr();
		else
		{
			scfailed = 1;
			return;
		}
		int result = ProcCondition();
	}
	else
		if (CheckNextSyms("while"))
		{
			scptr += 5;
			SkipSpaces();
			if (scr[scptr] == '(')
				IncPtr();
			else
			{
				scfailed = 1;
				return;
			}
			int result = ProcCondition();
		}
		else
			if (CheckNextSyms("for"))
			{
				scptr += 3;
				SkipSpaces();


			}
			else
			{
				ProcAssignment(VM);
			}
}

void ParseDeclaresAs(TrieNode* VM, BPC_Returns type)
{
	while (!CheckNextSyms(";") && !scfailed)
	{
		VM_DeclareVariable(VM, ParseName(), 0, type);
		if (scr[scptr] != ',' && scr[scptr] != ';')
			scfailed = 1;
		if (scr[scptr] == ',')
			IncPtr();
	}
}

int EvalScript(char* script)
{
	TrieNode* VM = Trie_Create();//TODO: memleak

	scptr = scfailed = row = coloumn = 0;
	scr = script;

	while (!CheckNextSyms("begin") && !scfailed)
	{
		SkipSpaces();
		if (CheckNextSyms("int "))
		{
			scptr += 4;
			ParseDeclaresAs(VM, BPC_ReturnsInt);
			IncPtr();
		}
		if (CheckNextSyms("double "))
		{
			scptr += 7;
			ParseDeclaresAs(VM, BPC_ReturnsDouble);
			IncPtr();
		}
		if (CheckNextSyms("string "))
		{
			scptr += 7;
			ParseDeclaresAs(VM, BPC_ReturnsString);
			IncPtr();
		}
	}

	if (!CheckNextSyms("begin"))
		scfailed = 1;

	scptr += 5;

	while (!CheckNextSyms("end") && !scfailed)
	{
		Block(VM);
		SkipSpaces();
	}

	if (scfailed)
		printf("Failed on %d sym \'%c\' (%d row %d col)\n\n", scptr, scr[scptr], row + 1, coloumn + 1);

	getch();

	return !scfailed;
}