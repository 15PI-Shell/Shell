#include "MathInterpreter.h"

int ptr = 0, failed = 0;
char* str;

lld Term();
lld Summand();
lld Multipler();
lld Const();//им всем не нужна видимость извне, поэтому тут

lld Term()//Выражение = Слагаемое [+/- Выражение]
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	lld ans;
	ans = Summand();

	switch (str[ptr])
	{
	case '-'://два case указывающих на одно место, хорошая штука
	case '+':
		ans += Term();
		break;
	case '\0':
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

lld Summand()//Слагаемое = Множитель [* Множитель]
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

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

	lld ans;
	ans = Multipler();
	if (str[ptr] == '*')
	{
		ptr++;//пропускаем *
		ans *= Multipler();
	}
	return ans * modify;
}

lld Multipler()//Множитель = Константа | Выражение
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	lld ans;
	if (str[ptr] == '(')//выражение в скобках
	{
		ptr++;//пропускаем (
		ans = Term();
	}
	else
		ans = Const();
	return ans;
}

lld Const()
{
	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию

	if (str[ptr] < '0' || str[ptr] > '9')
	{
		failed = 1;
		return 0;
	}
	lld ans = 0;
	while (str[ptr] >= '0' && str[ptr] <= '9')
	{
		ans *= 10;
		ans += str[ptr] - '0';
		ptr++;
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