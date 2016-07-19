#include "MathInterpreter.h"
#include "BuiltinProgController.h"

int failed;

typedef struct
{
	int ptr, bracets;
	char* str;
} Mathi;

double Term(Mathi* mi, TrieNode* VM);
double Summand(Mathi* mi, TrieNode* VM);
double Multipler(Mathi* mi, TrieNode* VM);
double Const(Mathi* mi);
double Function(Mathi* mi, TrieNode* VM, char* funct);
double Variable(Mathi* mi, TrieNode* VM, char* Var);
void shift(char* out, int key);
char* GetRightExpression(char* expression); //им всем не нужна видимость извне, поэтому тут

void Pass(Mathi* mi)
{
	while (mi->str[mi->ptr] == ' ' || mi->str[mi->ptr] == '\t' || mi->str[mi->ptr] == '\n' || mi->str[mi->ptr] == '\r')
		mi->ptr++;
}

void shift(char* out, int key)
{
	for (int i = strlen(out); i >= key; i--)
	{
		out[i + 1] = out[i];
	}
}

char* GetRightExpression(char* expression)
{
	int brac = 0;
	char* out = (char*)malloc(2 * strlen(expression));
	strcpy(out, expression);
	int i = -1, open = 0, close = 0;
	while (out[++i])
	{
		switch (out[i])
		{
		case '/':
			if (close == -1)
			{
				shift(out, i);
				out[i] = ')';
				close = i;
			}
			else
			{
				shift(out, open);
				out[open] = '(';
				open = 0;
				close = -1;
				i++;
			}
			break;
		case '*':
			if (close == -1)
			{
				shift(out, i);
				out[i] = ')';
				close = i;
			}
			break;
		case '+':
		case '-':
			if (close == -1)
			{
				shift(out, i);
				out[i] = ')';
				close = i;
			}
			else if (brac == 0)
				open = i + 1;
			break;
		case '(':
			if (close == -1)
				close = -2;
			else
				open = i;
			brac++;
			break;
		case ')':
			if (close == -2)
			{
				shift(out, i);
				out[i] = ')';
				close = i;
			}
			brac--;
			break;
		}
	}
	if (close == -1)
		strcat(out, ")");
	return out;
}

double Term(Mathi* mi, TrieNode* VM)//Выражение = Слагаемое [+/- Выражение]
{

	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию
	Pass(mi);

	double ans;
	ans = Summand(mi, VM);

	Pass(mi);

	switch (mi->str[mi->ptr])
	{
	case '-'://два case указывающих на одно место, хорошая штука
	case '+':
		ans += Term(mi, VM);
		break;
	case '\0':
		break;
	case ')':
		if (--(mi->bracets) < 0)
		{
			failed = 1;
			return 0;
		}
		mi->ptr++;//пропускаем )
			  //выражение закончилось
		break;
	default:
		failed = 1;
		return 0;
	}
	return ans;
}

double Summand(Mathi* mi, TrieNode* VM)//Слагаемое = Множитель [* Множитель]
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	Pass(mi);

	int modify = 1;
	switch (mi->str[mi->ptr])
	{
	case '+':
		mi->ptr++;
		break;
	case '-':
		modify = -1;
		mi->ptr++;
		break;
	}//иначе это константа без знака

	double ans;
	ans = Multipler(mi, VM);

	Pass(mi);

	switch (mi->str[mi->ptr])
	{
	case '*':
		mi->ptr++;//пропускаем *
		ans *= Summand(mi, VM);
		break;
	case '/':
		mi->ptr++;
		double checkAns = Summand(mi, VM);
		if (checkAns)
			ans /= checkAns;
		else
		{
			failed = 1;
			return 0;
		}
	}
	return ans * modify;
}

double Multipler(Mathi* mi, TrieNode* VM)//Множитель = Константа | Выражение
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию
	Pass(mi);

	double ans;
	if (mi->str[mi->ptr] == '(')//выражение в скобках
	{
		mi->bracets++;
		mi->ptr++;//пропускаем (
		ans = Term(mi, VM);
	}
	else if (tolower(mi->str[mi->ptr]) >= 'a' && tolower(mi->str[mi->ptr] <= 'z'))
	{
		char str2[20];
		int i = 0;
		while (tolower(mi->str[mi->ptr]) >= 'a' && tolower(mi->str[mi->ptr] <= 'z'))
		{
			str2[i] = mi->str[mi->ptr];
			mi->ptr++;
			i++;
		}
		str2[i] = 0;
		if (mi->str[mi->ptr] >= '0'&&mi->str[mi->ptr] <= '9')
		{
			failed = 1;
			return 0;
		}
		Pass(mi);
		if (mi->str[mi->ptr] == '(')
		{
			ans = Function(mi, VM, str2);
		}
		else
			ans = Variable(mi, VM, str2);
	}
	else
		ans = Const(mi);
	return ans;
}

double Function(Mathi* mi, TrieNode* VM, char* funct)
{
	BPC_Returns ret = BPC_WhatRets(funct);
	if (ret == BPC_ReturnsNothing || ret == BPC_ReturnsString)
	{
		failed = 1;
		return 0;
	}
	char args[1000] = "(";
	int i = 1;
	int bracets2 = 1;
	while ((mi->str[++(mi->ptr)] != ')') || (bracets2 != 1))
	{
		switch (mi->str[mi->ptr])
		{
		case '(':
			bracets2++;
			break;
		case ')':
			bracets2--;
			break;
		case 0:
			failed = 1;
			return 0;
		}
		args[i++] = mi->str[mi->ptr];
	}
	args[i] = ')';
	args[i + 1] = 0;
	mi->ptr++;
	strcpy(args, RetRightArg(VM, args, &failed));
	BPC_Returns returns;
	int copy_ptr = mi->ptr;
	char* str2 = (char*)malloc(strlen(mi->str) + 1);
	strcpy(str2, mi->str);
	char* ReturnPtr;
	ReturnPtr = BPC_Execute(funct, args, &returns);
	if (ReturnPtr == -1)
	{
		failed = 1;
		return 0;
	}

	double *ans = 0;
	MathInterpreter(VM, ReturnPtr, &ans);
	if (failed)
		return 0;
	mi->ptr = copy_ptr;
	strcpy(mi->str, str2);
	free(str2);
	return *ans;
}

double Variable(Mathi* mi, TrieNode* VM, char* Var)
{
	if (!strcmp(Var, "inf"))
	{
		failed = 1;
		return 1;
	}
	int constant;
	BPC_Returns type;
	void* result = VM_GetVariable(VM, Var, &constant, &type);
	if (!result)
	{
		failed = 1;
		return 0;
	}
	double toRet;
	if (type == BPC_ReturnsString)
	{
		failed = 1;
		return 1;
	}
	if (type == BPC_ReturnsInt)
		toRet = *(int*)result;
	if (type == BPC_ReturnsDouble)
		toRet = *(double*)result;
	return toRet;
}

double Const(Mathi* mi)
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	Pass(mi);

	if (mi->str[mi->ptr] < '0' || mi->str[mi->ptr] > '9')
	{
		failed = 1;
		return 0;
	}
	double ans = 0;
	while (mi->str[mi->ptr] >= '0' && mi->str[mi->ptr] <= '9')
	{
		ans *= 10;
		ans += mi->str[mi->ptr] - '0';
		mi->ptr++;
	}
	if (mi->str[mi->ptr] == '.')
	{
		double fract = 0;
		int k = 0;
		mi->ptr++;
		while (mi->str[mi->ptr] >= '0' && mi->str[mi->ptr] <= '9')
		{
			fract *= 10;
			fract += mi->str[mi->ptr] - '0';
			mi->ptr++;
			k++;
		}
		while (k--)
			fract *= 0.1;
		ans += fract;
	}
	return ans;
}

int MathInterpreter(TrieNode* VM, char* expression, double** result)
{
	Mathi mi;
	mi.ptr = failed = mi.bracets = 0;
	mi.str = GetRightExpression(expression);
	double* ans = (double*)malloc(sizeof(double));
	*ans = Term(&mi, VM);

	if (mi.str[mi.ptr] != 0)//остались ещё какие-то символы, например лишние закрывающие скобки
		failed = 1;

	if (!failed)
		*result = ans;

	return !failed;
}