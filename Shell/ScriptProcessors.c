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
		if (inter->verbose)
			printf("Setting %s to \"%s\"\n", name, (char*)val);
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
			if (inter->verbose)
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
			if (inter->verbose)
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
		VM_DeclareVariable(VM, name, 0, inter->verbose, type);
		if (inter->scr[inter->scptr] == '=')
		{
			inter->scptr++;
			int constant;
			BPC_Returns termType, varType;
			void* result, *currentVal;
			char* send1 = ParseBeforeComa(inter);
			char* send2 = ParseBeforeSemi(inter);
			GetTerm(inter, VM, strlen(send1) < strlen(send2) ? send1 : send2, &result, &termType);
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
	if (!strcmp(term, ""))
	{
		*result = "";
		*type = BPC_ReturnsString;
		return;
	}
	term = MkMemCopy(term, strlen(term));
	TrieNode* bk = Basic_VM;
	Basic_VM = VM;
	if (MathInterpreter(VM, term, result))
	{
		Basic_VM = bk;
		*type = BPC_ReturnsDouble;
		inter->scptr += strlen(term);
	}
	else
	{
		Basic_VM = bk;
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
			if (retType != BPC_ReturnsString)
			{
				inter->scfailed = 1;
				return 0;
			}
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
			if (retType != BPC_ReturnsString && retType != BPC_ReturnsNothing)
			{
				inter->scfailed = 1;
				return 0;
			}
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
	int delta = 1;
	while (inter->scr[inter->scptr] && !CheckNextSyms(inter, "&&") && !CheckNextSyms(inter, "||"))
	{
		if (CheckNextSyms(inter, ")"))
			delta--;
		else if (CheckNextSyms(inter, "("))
			delta++;
		if (delta == 0)
			break;
		inter->scptr++;
	}

	if (delta != 0)
	{
		inter->scfailed = 1;
		return 0;
	}

	char* data = (char*)malloc(inter->scptr - start + 1);
	strncpy(data, inter->scr + start, inter->scptr - start);
	data[inter->scptr - start] = 0;
	SkipSpaces(inter);

	return data;
}

int ProcCondition(InterpData* inter, TrieNode* VM)
{
	int result;
	SkipSpaces(inter);
	if (CheckNextSyms(inter, "("))
	{
		inter->scptr++;
		result = ProcCondition(inter, VM);
		SkipSpaces(inter);
		if (!CheckNextSyms(inter, ")"))
		{
			inter->scfailed = 1;
			return 0;
		}
		inter->scptr++;
		return result;
	}
	SkipSpaces(inter);

	char* parsed = ProcSingleCondition(inter);

	BPC_Returns leftType, rightType;
	void* resL, *resR;

	int innerPtr = 0;
	int startLeft = 0, startRight;

	while (parsed[innerPtr] && !CheckNextSymsFree(innerPtr, parsed, "<") && !CheckNextSymsFree(innerPtr, parsed, "!") && !CheckNextSymsFree(innerPtr, parsed, "=") && !CheckNextSymsFree(innerPtr, parsed, ">"))
		innerPtr++;

	char* left = (char*)malloc(innerPtr - startLeft + 1);
	strncpy(left, parsed + startLeft, innerPtr - startLeft);
	left[innerPtr - startLeft] = 0;

	InterpData id;
	id.row = id.scfailed = id.scptr = 0;
	id.scr = left;

	GetTerm(&id, VM, left, &resL, &leftType);

	inter->scfailed = id.scfailed;

	if (inter->scfailed)
		return 0;

	if (parsed[innerPtr])
	{
		char mode[2];
		if (parsed[innerPtr + 1] == '=')
		{
			mode[0] = parsed[innerPtr];
			mode[1] = parsed[innerPtr + 1];
			innerPtr += 2;
		}
		else
		{
			mode[0] = parsed[innerPtr];
			mode[1] = 0;
			innerPtr++;
		}

		startRight = innerPtr;
		int delta = 0;

		while (parsed[innerPtr])
		{
			if (parsed[innerPtr] == '(')
				delta++;
			else if (parsed[innerPtr] == ')')
				delta--;
			innerPtr++;
			if (delta < 0)
				break;
		}

		char* right = (char*)malloc(innerPtr - startRight + 1);
		strncpy(right, parsed + startRight, innerPtr - startRight);
		right[innerPtr - startRight] = 0;

		id.row = id.scfailed = id.scptr = 0;
		id.scr = right;

		GetTerm(&id, VM, right, &resR, &rightType);

		inter->scfailed = id.scfailed;

		if (inter->scfailed)
			return 0;

		if (leftType == BPC_ReturnsInt && rightType == BPC_ReturnsDouble)
		{
			rightType = BPC_ReturnsDouble;
			int trint = *(double*)resR;
			resR = &trint;
		}

		if (rightType == BPC_ReturnsInt && leftType == BPC_ReturnsDouble)
		{
			leftType = BPC_ReturnsDouble;
			int trint = *(double*)resL;
			resL = &trint;
		}

		if (leftType != rightType)
			result = 0;
		else
		{
			switch (mode[0] * 2 + mode[1])//hack
			{
			case '=' * 2 + '=':
				switch (leftType)
				{
				case BPC_ReturnsInt:
					result = (*(int*)resL) == (*(int*)resR);
					break;
				case BPC_ReturnsDouble:
					result = fabs((*(double*)resL) - (*(double*)resR)) < 0.00001;
					break;
				case BPC_ReturnsString:
					result = !strcmp((char*)resL, (char*)resR);
					break;
				}
				break;
			case '!' * 2 + '=':
				switch (leftType)
				{
				case BPC_ReturnsInt:
					result = (*(int*)resL) != (*(int*)resR);
					break;
				case BPC_ReturnsDouble:
					result = fabs((*(double*)resL) - (*(double*)resR)) >= 0.00001;
					break;
				case BPC_ReturnsString:
					result = !!strcmp((char*)resL, (char*)resR);
					break;
				}
				break;
			case '<' * 2:
				switch (leftType)
				{
				case BPC_ReturnsInt:
					result = (*(int*)resL) < (*(int*)resR);
					break;
				case BPC_ReturnsDouble:
					result = (*(double*)resL) < (*(double*)resR);
					break;
				case BPC_ReturnsString:
					result = strcmp((char*)resL, (char*)resR) < 0;
					break;
				}
				break;
			case '>' * 2:
				switch (leftType)
				{
				case BPC_ReturnsInt:
					result = (*(int*)resL) > (*(int*)resR);
					break;
				case BPC_ReturnsDouble:
					result = (*(double*)resL) > (*(double*)resR);
					break;
				case BPC_ReturnsString:
					result = strcmp((char*)resL, (char*)resR) > 0;
					break;
				}
				break;
			case '>' * 2 + '=':
				switch (leftType)
				{
				case BPC_ReturnsInt:
					result = (*(int*)resL) >= (*(int*)resR);
					break;
				case BPC_ReturnsDouble:
					result = (*(double*)resL) >= (*(double*)resR);
					break;
				case BPC_ReturnsString:
					result = strcmp((char*)resL, (char*)resR) >= 0;
					break;
				}
				break;
			case '<' * 2 + '=':
				switch (leftType)
				{
				case BPC_ReturnsInt:
					result = (*(int*)resL) <= (*(int*)resR);
					break;
				case BPC_ReturnsDouble:
					result = (*(double*)resL) <= (*(double*)resR);
					break;
				case BPC_ReturnsString:
					result = strcmp((char*)resL, (char*)resR) <= 0;
					break;
				}
				break;
			}
		}
	}
	else
	{
		switch (leftType)
		{
		case BPC_ReturnsString:
			result = !!*(char*)resL;
			break;
		case BPC_ReturnsInt:
			result = !!*(int*)resL;
			break;
		case BPC_ReturnsDouble:
			result = !!*(double*)resL;
			break;
		}
	}
	if (inter->verbose)
		printf("Parsed condition element: %s (%s)\n", parsed, result ? "TRUE" : "FALSE");

	if (CheckNextSyms(inter, "&&"))
	{
		inter->scptr += 2;
		int rightPart = ProcCondition(inter, VM);
		return result && rightPart;
	}
	else
		if (CheckNextSyms(inter, "||"))
		{
			inter->scptr += 2;
			int rightPart = ProcCondition(inter, VM);
			return result || rightPart;
		}
		else
			if (CheckNextSyms(inter, ")"))
			{
				inter->scptr++;
				//последнее
				return result;
			}
	inter->scfailed = 1;
	return 0;
}