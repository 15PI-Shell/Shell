#include "MathInterpreter.h"
#include "BuiltinProgController.h"

int ptr = -1, failed = 0, bracets = 0;
char* str;

double Term();
double Summand();
double Multipler();
double Const();
double Function(char* funct);
double Variable(char* Var); 
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
			ans = Variable(str2);
	}
	else
		ans = Const();
	return ans;
}

double Function(char* funct)
{
	char args[1000] = "";
	int i = 0;
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
	args[i] = 0;
	ptr++;
	strcpy(args, RetRightArg(args));
	BPC_Returns returns;
	int copy_ptr = ptr;
	char* str2 = (char*)malloc(strlen(str));
	strcpy(str2, str);
	char* ReturnPtr;
	ReturnPtr = BPC_Execute(funct, args, &returns);
	if (ReturnPtr == -1)
	{
		failed = 1;
		return 0;
	}
	if (returns == BPC_ReturnsDouble)
	{
		double ans = 0;
		MathInterpreter(ReturnPtr, &ans);
		ptr = copy_ptr;
		strcpy(str, str2);
		return ans;
		free(str2);
	}

	else
	{

		failed = 1;
		return 0;
	}
}

double Variable(char* Var)
{
	//Здесь нужна функция, которая будет возвращать число
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
	str = GetRightExpression(expression);
	*result = Term();

	if (str[ptr] != 0)//остались ещё какие-то символы, например лишние закрывающие скобки
		failed = 1;

	if (failed)
		*result = ptr;//сохраним где упали

	return !failed;
}