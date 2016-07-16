#include "MathFunction.h"


char* MathFun_sin(char* arg)
{
	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up==NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			sprintf(arg, "%lf", sin(ans));
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
	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			sprintf(arg, "%lf", cos(ans));
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

	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			sprintf(arg, "%lf", tan(ans));
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

	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			ans = tan(ans);
			if (ans)
			{
				sprintf(arg, "%lf", 1 / tan(ans));
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

	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			sprintf(arg, "%lf", log(ans));
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

	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			sprintf(arg, "%lf", exp(ans));
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
	char* RightArgs = RetRightArg(arg);
	double ans;
	SingleLinklistNode* ArgList = NULL;
	if (ParsOfArgs(RightArgs, &ArgList) && ArgList->up == NULL)
		if (MathInterpreter((char*)ArgList->value, &ans))
		{
			sprintf(arg, "%lf", sqrt(ans));
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
	char* RightArgs = RetRightArg(arg);
	SingleLinklistNode* ArgList=NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double arg1, arg2;
	if (k)
	{
		if (MathInterpreter((char*)ArgList->value, &arg1) && MathInterpreter((char*)ArgList->up->value, &arg2) && ArgList->up->up == NULL)
		{
			sprintf(arg, "%lf", pow(arg1, arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}

char* MathFun_max(char* arg)
{
	char* RightArgs = RetRightArg(arg);
	SingleLinklistNode* ArgList = NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double arg1, arg2;
	if (k)
	{
		if (MathInterpreter((char*)ArgList->value, &arg1) && MathInterpreter((char*)ArgList->up->value, &arg2) && ArgList->up->up == NULL)
		{
			sprintf(arg, "%lf", max(arg1, arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}

char* MathFun_min(char* arg)
{
	char* RightArgs = RetRightArg(arg);
	SingleLinklistNode* ArgList = NULL;
	int k = ParsOfArgs(arg, &ArgList);
	double arg1, arg2;
	if (k)
	{
		if (MathInterpreter((char*)ArgList->value, &arg1) && MathInterpreter((char*)ArgList->up->value, &arg2) && ArgList->up->up == NULL)
		{
			sprintf(arg, "%lf", min(arg1, arg2));
			return arg;
		}
		else
			return -1;
	}
	else return -1;
}