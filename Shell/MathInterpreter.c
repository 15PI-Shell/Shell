#include "MathInterpreter.h"
#include "BuiltinProgController.h"

int ptr = -1, failed = 0, bracets = 0;
char* str;

double Term();
double Summand();
double Multipler();
double Const();
double Function(char* funct);
double Variable(); //им всем не нужна видимость извне, поэтому тут

void Pass()
{
	while (str[ptr] == ' ')
		ptr++;
}

double Term()//Выражение = Слагаемое [+/- Выражение]
{

	if (failed)
		return 0;//если где-то ошибка, то завершаем рекурсию
	Pass();

	double ans;
	ans = Summand();

	Pass();

	switch (str[ptr])
	{
	case '-'://два case указывающих на одно место, хорошая штука
	case '+':
		ans += Term();
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

double Summand()//Слагаемое = Множитель [* Множитель]
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
	ans = Multipler();

	Pass();

	switch (str[ptr])
	{
	case '*':
		ptr++;//пропускаем *
		ans *= Summand();
		break;
	case '/':
		ptr++;
		double checkAns = Summand();
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
	Pass();

	double ans;
	if (str[ptr] == '(')//выражение в скобках
	{
		bracets++;
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
		Pass();
		if (str[ptr] == '(')
		{
			ans = Function(str2);
		}
		else
			ans = Variable();
	}
	else
		ans = Const();
	return ans;
}

double Function(char* funct)
{
	//char* fun = (char*)malloc(strlen(funct) + 1);
	//strcpy(fun, funct);
	//funct = fun;
	char args[1000] = "";
	int i = 0;
	int bracets2 = 1;
	//int ptr2 = ptr;
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
	args[i] = 0;
	ptr++;
	BPC_Returns returns;
	int copy_ptr = ptr;
	char* str2 = (char*)malloc(strlen(str));
	strcpy(str2, str);
	char* ReturnPtr;
	ReturnPtr = BPC_Execute(funct, args, &returns);
	if (ReturnPtr == -1)
	{
		//ptr += copy_ptr;
		failed = 1;
		return 0;
	}
	if (returns == BPC_ReturnsDouble)
	{
		double ans = 0;
		//copy_ptr = copy_ptr + ptr + 1;
		MathInterpreter(ReturnPtr, &ans);
		ptr = copy_ptr;
		strcpy(str, str2);
		return ans;
		//free(funct);
		free(str2);
	}

	else
	{

		failed = 1;
		return 0;
	}
}

double Variable()
{
	return 1;
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

int MathInterpreter(char* expression, double* result)
{
	ptr = failed = 0;
	str = (char*)malloc(strlen(expression));
	strcpy(str, expression);
	expression = str;

	*result = Term();

	if (str[ptr] != 0)//остались ещё какие-то символы, например лишние закрывающие скобки
		failed = 1;

	if (failed)
		*result = ptr;//сохраним где упали

	return !failed;
}