#include "ScriptInterpreter.h"

char* scr;
int scptr, scfailed;

char* ParseName();
char* ParseBeforeSemi();
char* ParseInsideBrackets();


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
		scptr++;
		changed = 1;
	}

	if (CheckNextSyms("//"))
	{
		scptr += 2;
		while (scr[scptr] && scr[scptr] != '\n')
			scptr++;
		if (scr[scptr])
			scptr++;
		changed = 1;
	}

	if (CheckNextSyms("/*"))
	{
		scptr += 2;
		while (!CheckNextSyms("*/"))
			scptr++;
		if (scr[scptr] && scr[scptr + 1])
			scptr += 2;
		else
			scfailed = 1;
		changed = 1;
	}

	if (changed)
		SkipSpaces();
}

char* StringTermProc(char* term)
{
	if (term[0] == '\"')
	{
		strtok(term, "\"");
		return term + 1;
	}
	else
	{
		//функция?

	}
	return 0;
}

void GetTerm(char* term, void** result, BPC_Returns *type)
{
	if (MathInterpreter(term, result))
		*type = BPC_ReturnsDouble;
	else
	{
		*result = StringTermProc(term);
		*type = BPC_ReturnsString;
	}
}

char* ProcFunction(char* name, BPC_Returns* type)
{
	void* result;
	char* args = (char*)malloc(1000);
	GetTerm(ParseInsideBrackets(), &result, type);
	if (scfailed)
		return;
	switch (*type)
	{
	case BPC_ReturnsInt:
		sprintf(args, "%d", *((int*)result));
		break;
	case BPC_ReturnsDouble:
		sprintf(args, "%lf", *((double*)result));
		break;
	case BPC_ReturnsString:
		strcpy(args, (char*)result);
		break;
	}
	char* res = BPC_Execute(name, args, type);
	if (res >= 0)
	{
		SkipSpaces();
		if (scr[scptr] == ';')
			scptr++;
		return res;
	}
	else
	{
		scfailed = 1;
		return 0;
	}
}

void ProcAssignment()
{
	char* name = ParseName();

	if (scfailed)
		return;

	BPC_Returns outerFuncRet;

	if (scr[scptr] == '(')
	{
		ProcFunction(name, &outerFuncRet);
		return;
	}

	BPC_Returns varType;

	void* currentVal = VM_GetVariable(name, &varType);

	int mode;

	if (CheckNextSyms("="))
	{
		scptr++;
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
	void* result;
	BPC_Returns termType;

	int ptrbkp = scptr;
	char* funcname = ParseName();
	if (scfailed || scr[scptr] != '(')
	{
		scptr = ptrbkp;
		scfailed = 0;
		char* bsemi = ParseBeforeSemi();
		GetTerm(bsemi, &result, &termType);
	}
	else
	{
		result = ProcFunction(funcname, &termType);
	}

	if (scfailed)
		return;

	if (varType == BPC_ReturnsString)
	{
		char* toAssign = (char*)malloc(1000);
		char *val = (char*)currentVal;

		switch (mode)
		{
		case 0:
			strcpy(toAssign, (char*)result);
			break;
		case 1:
			strcpy(toAssign, (char*)val);
			strcat(toAssign, (char*)result);
			break;
		default:
			scfailed = 1;
			return;
		}
		VM_SetVariable(name, toAssign);
	}
	else
	{
		if (termType == BPC_ReturnsString)
		{
			scfailed = 1;
			return 0;
		}

		if (varType == BPC_ReturnsInt)
		{
			int toAssign = *((double*)result);
			int *val;
			if (currentVal)
				val = ((int*)currentVal);
			else if (mode != 0)
			{
				scfailed = 1;
				return;
			}
			else
			{
				val = (int*)malloc(sizeof(int));
			}
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
				*val /= toAssign;
				break;
			}
			VM_SetVariable(name, val);
		}
		else if (varType == BPC_ReturnsDouble)
		{
			double toAssign = *((double*)result);
			double *val;//TODO: Memory leak
			if (currentVal)
				val = ((double*)currentVal);
			else if (mode != 0)
			{
				scfailed = 1;
				return;
			}
			else
			{
				val = (int*)malloc(sizeof(double));
			}
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
				*val /= toAssign;
				break;
			}
			VM_SetVariable(name, val);
		}
	}
}

void Block()
{
	SkipSpaces();
	if (0 == scr[scptr])
		return;

	if (CheckNextSyms("if"))
	{
		scptr += 2;
		SkipSpaces();


	}
	else
		if (CheckNextSyms("while"))
		{
			scptr += 5;
			SkipSpaces();


		}
		else
			if (CheckNextSyms("for"))
			{
				scptr += 3;
				SkipSpaces();


			}
			else
			{
				ProcAssignment();
			}
}

char* ParseBeforeSemi()
{
	SkipSpaces();

	int start = scptr;

	while (!CheckNextSyms(";"))
		scptr++;

	char* data = (char*)malloc(scptr - start + 1);
	strncpy(data, scr + start, scptr - start);
	data[scptr - start] = 0;
	scptr++;//за ;
	SkipSpaces();

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
		scptr++;
	char* varname = (char*)malloc(scptr - start + 1);
	strncpy(varname, scr + start, scptr - start);
	varname[scptr - start] = 0;

	SkipSpaces();

	return varname;
}

char* ParseInsideBrackets()
{
	scptr++;//пропускаем (
	int delta = 1, start = scptr;
	while (delta != 0 && scr[scptr])
	{
		scptr++;
		if (scr[scptr] == '(')
			delta++;
		else if (scr[scptr] == ')')
			delta--;
	}

	if (delta != 0)
	{
		scfailed = 1;
		return 0;
	}

	char* data = (char*)malloc(scptr - start + 1);
	strncpy(data, scr + start, scptr - start);
	data[scptr - start] = 0;
	scptr++;
	SkipSpaces();

	return data;
}

int EvalScript(char* script)
{
	scptr = scfailed = 0;
	scr = script;

	while (!CheckNextSyms("begin") && !scfailed)
	{
		SkipSpaces();
		if (CheckNextSyms("int "))
		{
			scptr += 4;
			while (!CheckNextSyms(";") && !scfailed)
			{
				VM_DeclareVariable(ParseName(), BPC_ReturnsInt);
				if (scr[scptr] != ',' && scr[scptr] != ';')
					scfailed = 1;
				if (scr[scptr] == ',')
					scptr++;
			}
			scptr++;
		}
		if (CheckNextSyms("double "))
		{
			scptr += 7;
			while (!CheckNextSyms(";") && !scfailed)
			{
				VM_DeclareVariable(ParseName(), BPC_ReturnsDouble);
				if (scr[scptr] != ',' && scr[scptr] != ';')
					scfailed = 1;
				if (scr[scptr] == ',')
					scptr++;
			}
			scptr++;
		}
		if (CheckNextSyms("string "))
		{
			scptr += 7;
			while (!CheckNextSyms(";") && !scfailed)
			{
				VM_DeclareVariable(ParseName(), BPC_ReturnsString);
				if (scr[scptr] != ',' && scr[scptr] != ';')
					scfailed = 1;
				if (scr[scptr] == ',')
					scptr++;
			}
			scptr++;
		}
	}

	if (!CheckNextSyms("begin"))
		scfailed = 1;

	scptr += 5;

	while (!CheckNextSyms("end") && !scfailed)
	{
		Block();
		SkipSpaces();
	}

	if (scfailed)
		printf("scfailed on %d\n\n", scptr);

	getch();

	return !scfailed;
}