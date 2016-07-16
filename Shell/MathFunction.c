#include "MathFunction.h"


char* MathFun_sin(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		sprintf(arg, "%lf", sin(ans));
		return arg;
	}
	else
	{
		//free(copy_arg);
		return -1;
	}
}

char* MathFun_cos(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		sprintf(arg, "%lf", cos(ans));
		return arg;
	}
	else
		return -1;
}

char* MathFun_tan(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		sprintf(arg, "%lf", tan(ans));
		return arg;
	}
	else
		return -1;
}

char* MathFun_cot(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		if ((ans = tan(ans)))
			sprintf(arg, "%lf", 1 / ans);
		else
			return -1;
		return arg;
	}
	else
		return -1;
}

char* MathFun_log(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		sprintf(arg, "%lf", log(ans));
		return arg;
	}
	else
		return -1;
}

char* MathFun_exp(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		sprintf(arg, "%lf", exp(ans));
		return arg;
	}
	else
		return -1;
}

char* MathFun_sqrt(char* arg)
{
	double ans;
	if (MathInterpreter(arg, &ans))
	{
		sprintf(arg, "%lf", sqrt(ans));
		return arg;
	}
	else
		return -1;
}

char* MathFun_pow(char* arg)
{
	SingleLinklistNode* ArgList=NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double arg1, arg2;
	if (k)
	{
		if (MathInterpreter((char*)ArgList->value, &arg1))
			if (MathInterpreter((char*)ArgList->up->value, &arg2))
				if (ArgList->up->up == NULL)
		{
			sprintf(arg, "%lf", pow(arg1, arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}