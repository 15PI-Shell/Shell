#include "MathInterpreter.h"

int ptr = -1, failed = 0;
char* str;

double Term();
double Summand();
double Multipler();
double Const();
double Function(char* funct);
double Variable(); //им всем не нужна видимость извне, поэтому тут

double Term()//Выражение = Слагаемое [+/- Выражение]
{

	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	while (str[ptr] == ' ')
		ptr++;

	double ans;
	ans = Summand();

	while (str[ptr] == ' ')
		ptr++;

	switch (str[ptr])
	{
	case '-'://два case указывающих на одно место, хорошая штука
	case '+':
		ans += Term();
		break;
	case '\0':
		break;
	case ')':
		ptr++;//пропускаем )
			  //выражение закончилось
		break;
	default:
		failed = 1;
		return 0;
	}
	return ans;
}

double Summand()//Слагаемое = Множитель [* Множитель]
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	while (str[ptr] == ' ')
		ptr++;

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
	ans = Multipler();

	while (str[ptr] == ' ')
		ptr++;

	switch (str[ptr])
	{
	case '*':
		ptr++;//пропускаем *
		ans *= Summand();
		break;
	case '/':
		ptr++;
		double checkAns = Multipler();
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

double Multipler()//Множитель = Константа | Выражение
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	while (str[ptr] == ' ')
		ptr++;

	double ans;
	if (str[ptr] == '(')//выражение в скобках
	{
		ptr++;//пропускаем (
		ans = Term();
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
		if (str[ptr] == '(')
			ans = Function(str2);
		else
			ans = Variable();
	}
	else
		ans = Const();
	return ans;
}

double Function(char* funct)
{
	char* fun = (char*)malloc(strlen(funct) + 1);
	double ans = Term();
	return ans;
}

double Variable()
{
	return 1;
}

double Const()
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	while (str[ptr] == ' ')
		ptr++;
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

int MathInterpreter(char* expression, double* result)
{
	ptr = failed = 0;
	str = expression;

	*result = Term();

	if (str[ptr] != 0)//остались ещё какие-то символы, например лишние закрывающие скобки
		failed = 1;

	if (failed)
		*result = ptr;//сохраним где упали

	return !failed;
}