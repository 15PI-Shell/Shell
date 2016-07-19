#include "MathFunction.h"


char* MathFun_sin(char* arg)
{
	double *ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up==NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", sin(*ans));
			return arg;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_cos(char* arg)
{
	double* ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", cos(*ans));
			return arg;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_tan(char* arg)
{
	double* ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", tan(*ans));
			return arg;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_cot(char* arg)
{
	double* ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			*ans = tan(*ans);
			if (*ans)
			{
				arg = malloc(1000);
				sprintf(arg, "%lf", 1 / tan(*ans));
				return arg;
			}
			else
				return -1;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_log(char* arg)
{
	double* ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", log(*ans));
			return arg;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_exp(char* arg)
{
	double* ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", exp(*ans));
			return arg;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_sqrt(char* arg)
{
	double* ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(arg, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &ans))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", sqrt(*ans));
			return arg;
		}
		else
		{
			return -1;
		}
	else
		return -1;
}

char* MathFun_pow(char* arg)
{
	SingleLinklistNode* ArgList=NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double* arg1, *arg2;
	if (k == 2)
	{
		if (MathInterpreter(Basic_VM, (char*)ArgList->up->value, &arg1) && MathInterpreter(Basic_VM, (char*)ArgList->value, &arg2))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", pow(*arg1, *arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}

char* MathFun_max(char* arg)
{
	SingleLinklistNode* ArgList = NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double *arg1, *arg2;
	if (k == 2)
	{
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &arg1) && MathInterpreter(Basic_VM, (char*)ArgList->up->value, &arg2))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", max(*arg1, *arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}

char* MathFun_min(char* arg)
{
	SingleLinklistNode* ArgList = NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double *arg1, *arg2;
	if (k == 2)
	{
		if (MathInterpreter(Basic_VM, (char*)ArgList->value, &arg1) && MathInterpreter(Basic_VM, (char*)ArgList->up->value, &arg2))
		{
			arg = malloc(1000);
			sprintf(arg, "%lf", min(*arg1, *arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}