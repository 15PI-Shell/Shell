#include "Shell.h"

int main()
{

	BPC_Init();//регистрируем программы в контроллере

	char mas[] = "sum 40 5 | HelloWorld | D:\\printf\\Debug\\printf.exe";
	analisator(mas);

	getch();

	return 0;
}
