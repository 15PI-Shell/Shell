#include "Shell.h"

int main()
{
	BPC_Init();

	char* arg = " sum 15 100 >sum 25";
	CmdInterpretator(arg);
	getch();
	/*
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
	}*/

	return 0;
}