#include "Shell.h"

int main()
{
	BPC_Init();//регистрируем программы в контроллере

	char mas[] = "Sum 12 8";
	char *p = analisator(mas);

	getch();
	return 0;
}
