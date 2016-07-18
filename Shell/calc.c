#include "calc.h"

char* calc(char* expr)
{
	while (*expr == ' ')
		expr++;
	int q = 0;
	if (*expr == '"')
	{
		*expr++;
		while (expr[q] && expr[q] != '"')
			q++;
		expr[q] = ' ';
	}
	double* res = malloc(sizeof(double));
	if (!MathInterpreter(Basic_VM, expr, &res))
	{
		free(res);
		return -1;
	}
	sprintf(expr, "%lf", *res);
	free(res);
	return expr;
}