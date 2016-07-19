#include "History.h"
char* PrintHistory(char *args)
{
	int i = 0, n = 0;
	sscanf(args, "%d", &n);//��������� ��������
	if (n <= 0) {
		printf("Argument is wrong\n");
		return -1;
	}
	while ((CurrHist->up) && (i < n))//������������ ������� �� n ������� ���� ��� ��������� ��� ��������, ���� � ������� ������ �����
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