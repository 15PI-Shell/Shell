#include "Shell.h"
int main()
{
	
	BPC_Init();
	ConsoleInitialisation();
	int key, buffLen, cur = 0, flagOfAutocomplitionList=0;
	while (1)
	{
		buffLen = strlen(Buff);
		key = getch();
		switch (key)
		{
		case keys_arrows:
			key = getch();
			switch (key)
			{
			case key_left: ConsoleCursorMoveLeft(); break;
			case key_right:ConsoleCursoreMoveRight(); break;
			case key_up: ConsoleGetPrewHistory(&flagOfAutocomplitionList); break;
			case key_down:ConsoleGetNextHistory(&flagOfAutocomplitionList); break;
			case key_del: ConsoleDeleteCurElem(); break;
			}
			break;
		case key_enter:ConsoleEnter(flagOfAutocomplitionList); break;	
		case key_BackSpase:ConsoleBackSpace();break;
		case key_tab:ConsoleAutocomplition( buffLen, &flagOfAutocomplitionList);break;//автодополнение
		default:ConsolePrintChar(key, buffLen);break;
		}
	}
	return 0;
}
