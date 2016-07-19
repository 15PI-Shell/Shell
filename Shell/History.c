#include "History.h"
char* PrintHistory(char *args)
{
	int i = 0, n = 0;
	SingleLinklistNode *ARList = NULL;
	int n_ar;
	n_ar = ParsOfArgs(args, &ARList);
	if (n_ar != 1)
	{
		printf("The number of arguments is wrong\n");
		return -1;
	}
	sscanf(ARList->value, "%d", &n);//сканируем аргумент
	if (n <=0) {
		printf("Argument is wrong\n");
		return -1;
	}
	while ((CurrHist->up) && (i < n))//перематываем историю на n позиций верх или насколько это возможно, если в истории меньше строк
	{
		CurrHist=CurrHist->up;
		i++;
	}

	while ((CurrHist->down) && (i >= 0))
	{
		printf("%s\n", (char*)CurrHist->value);
		i--;
		CurrHist = CurrHist->down;
	}
	return 0;
}