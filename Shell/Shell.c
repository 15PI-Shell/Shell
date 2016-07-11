#include "Shell.h"

int main()
{

	BPC_Init();//регистрируем программы в контроллере

	char mas[] = "sum 156 100 | Binary ";
	char *p = analisator(mas);
	if (p != NULL)
		while (*p != '\0')
		{
			printf("%c", *p);
			p++;
		}
	getch();

	return 0;
}
