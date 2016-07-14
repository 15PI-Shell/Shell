#include "ScriptInterpreter.h"

char* scr;
int scptr, failed;

char* ParseName();
char* ParseBeforeSemi();

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
	while ((scr[scptr] == ' ' || scr[scptr] == '\t' || scr[scptr] == '\n' || scr[scptr] == '\r') && scptr++ && (changed = 1));

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
			failed = 1;
		changed = 1;
	}

	if (changed)
		SkipSpaces();
}

void GetTerm(char* term, void** result, BPC_Returns *type)
{
	if (MathInterpreter(term, result))
		*type = BPC_ReturnsDouble;
	else
	{
		failed = 1;
		*type = BPC_ReturnsString;
	}
}

void ProcAssignment()
{
	char* name = ParseName();

	if (failed)
		return;

	if (scr[scptr] == '(')
	{
		//func
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

	void* result;
	BPC_Returns termType;
	char* cc = ParseBeforeSemi();
	GetTerm(cc, &result, &termType);
	if (failed)
		return;

	if (varType == BPC_ReturnsString)
	{
		//convert to string
	}
	else
	{
		if (termType == BPC_ReturnsString)
		{
			failed = 1;
			return 0;
		}

		if (varType == BPC_ReturnsInt)
		{
			int toAssign = *((double*)result);
			int *val = (int*)malloc(sizeof(int));
			if (currentVal)
				val = ((int*)currentVal);
			else if (mode != 0)
			{
				failed = 1;
				return;
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
			double *val = (int*)malloc(sizeof(double));//TODO: Memory leak
			if (currentVal)
				val = ((double*)currentVal);
			else if (mode != 0)
			{
				failed = 1;
				return;
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
		failed = 1;
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

int EvalScript(char* script)
{
	scptr = failed = 0;
	scr = script;

	while (!CheckNextSyms("begin") && !failed)
	{
		SkipSpaces();
		if (CheckNextSyms("int "))
		{
			scptr += 4;
			while (!CheckNextSyms(";") && !failed)
			{
				VM_DeclareVariable(ParseName(), BPC_ReturnsInt);
				if (scr[scptr] != ',' && scr[scptr] != ';')
					failed = 1;
				if (scr[scptr] == ',')
					scptr++;
			}
			scptr++;
		}
		if (CheckNextSyms("double "))
		{
			scptr += 7;
			while (!CheckNextSyms(";") && !failed)
			{
				VM_DeclareVariable(ParseName(), BPC_ReturnsDouble);
				if (scr[scptr] != ',' && scr[scptr] != ';')
					failed = 1;
				if (scr[scptr] == ',')
					scptr++;
			}
			scptr++;
		}
		if (CheckNextSyms("string "))
		{
			scptr += 7;
			while (!CheckNextSyms(";") && !failed)
			{
				VM_DeclareVariable(ParseName(), BPC_ReturnsString);
				if (scr[scptr] != ',' && scr[scptr] != ';')
					failed = 1;
				if (scr[scptr] == ',')
					scptr++;
			}
			scptr++;
		}
	}

	if (!CheckNextSyms("begin"))
		failed = 1;

	scptr += 5;

	while (!CheckNextSyms("end") && !failed)
	{
		Block();
	}

	if (failed)
		printf("Failed on %d\n\n", scptr);

	getch();

	return !failed;
}