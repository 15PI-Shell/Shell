#include "MathInterpreter.h"
#include "BuiltinProgController.h"

int ptr = -1, failed = 0, bracets = 0;
char* str;

double Term(TrieNode* VM);
double Summand(TrieNode* VM);
double Multipler(TrieNode* VM);
double Const();
double Function(TrieNode* VM, char* funct);
double Variable(TrieNode* VM, char* Var);
void shift(char* out, int key);
char* GetRightExpression(char* expression); //им всем не нужна видимость извне, поэтому тут

void Pass()
{
	while (str[ptr] == ' ')
		ptr++;
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

double Term(TrieNode* VM)//Выражение = Слагаемое [+/- Выражение]
{

	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию
	Pass();

	double ans;
	ans = Summand(VM);

	Pass();

	switch (str[ptr])
	{
	case '-'://два case указывающих на одно место, хорошая штука
	case '+':
		ans += Term(VM);
		break;
	case '\0':
		break;
	case ')':
		if (--bracets < 0)
		{
			failed = 1;
			return 0;
		}
		ptr++;//пропускаем )
			  //выражение закончилось
		break;
	default:
		failed = 1;
		return 0;
	}
	return ans;
}

double Summand(TrieNode* VM)//Слагаемое = Множитель [* Множитель]
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	Pass();

	int modify = 1;
	switch (str[ptr])
	{
	case '+':
		ptr++;
		break;
	case '-':
		modify = -1;
		ptr++;
		break;
	}//иначе это константа без знака

	double ans;
	ans = Multipler(VM);

	Pass();

	switch (str[ptr])
	{
	case '*':
		ptr++;//пропускаем *
		ans *= Summand(VM);
		break;
	case '/':
		ptr++;
		double checkAns = Summand(VM);
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

double Multipler(TrieNode* VM)//Множитель = Константа | Выражение
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию
	Pass();

	double ans;
	if (str[ptr] == '(')//выражение в скобках
	{
		bracets++;
		ptr++;//пропускаем (
		ans = Term(VM);
	}
	else if (tolower(str[ptr]) >= 'a' && tolower(str[ptr] <= 'z'))
	{
		char str2[20];
		int i = 0;
		while (tolower(str[ptr]) >= 'a' && tolower(str[ptr] <= 'z'))
		{
			str2[i] = str[ptr];
			ptr++;
			i++;
		}
		str2[i] = 0;
		if (str[ptr] >= '0'&&str[ptr] <= '9')
		{
			failed = 1;
			return 0;
		}
		Pass();
		if (str[ptr] == '(')
		{
			ans = Function(VM, str2);
		}
		else
			ans = Variable(VM, str2);
	}
	else
		ans = Const();
	return ans;
}

double Function(TrieNode* VM, char* funct)
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
	while ((str[++ptr] != ')') || (bracets2 != 1))
	{
		switch (str[ptr])
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
		args[i++] = str[ptr];
	}
	args[i] = ')';
	args[i + 1] = 0;
	ptr++;
	strcpy(args, RetRightArg(VM, args, &failed));
	BPC_Returns returns;
	int copy_ptr = ptr;
	char* str2 = (char*)malloc(strlen(str) + 1);
	strcpy(str2, str);
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
	ptr = copy_ptr;
	strcpy(str, str2);
	free(str2);
	return *ans;
}

double Variable(TrieNode* VM, char* Var)
{
	if (!strcmp(Var, "inf"))
	{
		failed = 1;
		return 1;
	}
	int constant;
	BPC_Returns type;
	void* result = VM_GetVariable(VM, Var, &constant, &type);
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

double Const()
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	Pass();

	if (str[ptr] < '0' || str[ptr] > '9')
	{
		failed = 1;
		return 0;
	}
	double ans = 0;
	while (str[ptr] >= '0' && str[ptr] <= '9')
	{
		ans *= 10;
		ans += str[ptr] - '0';
		ptr++;
	}
	if (str[ptr] == '.')
	{
		double fract = 0;
		int k = 0;
		ptr++;
		while (str[ptr] >= '0' && str[ptr] <= '9')
		{
			fract *= 10;
			fract += str[ptr] - '0';
			ptr++;
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
	ptr = failed = 0;
	str = GetRightExpression(expression);
	double* ans = (double*)malloc(sizeof(double));
	*ans = Term(VM);

	if (str[ptr] != 0)//остались ещё какие-то символы, например лишние закрывающие скобки
		failed = 1;

	*result = ans;

	if (failed)
		*result = ptr;//сохраним где упали

	return !failed;
}