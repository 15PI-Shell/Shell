#include "Shell.h"
#include "MathInterpreter.h"//временно


//((2+7)*2-65)+((4523-542)*8-34)-256*(5+(234-56)*(345+34)) = -17239785
//(123-12)*1024-34*42+(144*12-6*31)*17+28*(56-91) = 137470
//-(-1)*(-3) = -3
//-5*(50+7*0) = -250
//((((5+2)*3)*4)*9)+0 = 756
//((((5+2)*3)*4)*9 - ошибка, не совпадает колво скобок
int main()
{
	BPC_Init();
	double result;
	char* expression = (char*)malloc(1000);
	while (1)
	{
		fgets(expression, 1000, stdin);
		expression[strlen(expression) - 1] = 0;
		if (MathInterpreter(expression, &result))
			printf("Answer: %lf\n", result);
		else
			printf("Error on %d\n", (int)result);
	}
	return 0;//до остального дело не дойдёт


	
	ConsoleInitialisation();
	int key;
	while (1)
	{
		key = getch();
		switch (key)
		{
		case keys_arrows:
			key = getch();
			switch (key)
			{
			case key_left: ConsoleCursorMoveLeft(); break;
			case key_right:ConsoleCursoreMoveRight(); break;
			case key_up: ConsoleGetPrewHistory(); break;
			case key_down:ConsoleGetNextHistory(); break;
			case key_del: ConsoleDeleteCurElem(); break;
			} break;
		case key_enter:ConsoleEnter(); break;
		case key_BackSpase:ConsoleBackSpace(); break;
		case key_tab: ConsoleAutocompletion(); break;
		case keys_ctrl_v: PastInConsole(); break;
		default:ConsolePrintChar(key); break;
		}
	}

	return 0;
}
