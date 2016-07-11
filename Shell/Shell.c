#include "Shell.h"
#include <conio.h>
int main()
{
	BPC_Init();
	ConsoleInitialisation();
	int key, flagOfAutocomplitionList = 0; LastFoundList = 0;
	while (1)
	{
		key = _getch();
		switch (key)
		{
		case keys_arrows:
			key = _getch();
			switch (key)
			{
			case key_left: ConsoleCursorMoveLeft(); break;
			case key_right:ConsoleCursoreMoveRight(); break;
			case key_up: ConsoleGetPrewHistory(flagOfAutocomplitionList); break;
			case key_down:ConsoleGetNextHistory(flagOfAutocomplitionList); break;
			case key_del: ConsoleDeleteCurElem(); break;
			}
			break;
		case key_enter:ConsoleEnter(flagOfAutocomplitionList); break;
		case key_BackSpase:ConsoleBackSpace(); break;
		case key_tab:ConsoleAutocomplition(flagOfAutocomplitionList,  &LastFoundList); 
			if ((LastFoundList != NULL)&&(LastFoundList->up != 0))
			{
				key = _getch();
				switch (key)
				{
				case keys_arrows:
					key = _getch();
					switch (key)
					{
					case key_left: ConsoleCursorMoveLeft(); break;
					case key_right:ConsoleCursoreMoveRight(); break;
					case key_up: ConsoleGetPrewHistory(flagOfAutocomplitionList); break;
					case key_down:ConsoleGetNextHistory(flagOfAutocomplitionList); break;
					case key_del: ConsoleDeleteCurElem(); break;
					}
					break;
				case key_enter:ConsoleEnter(flagOfAutocomplitionList); break;
				case key_BackSpase:ConsoleBackSpace(); break;
				case key_tab:flagOfAutocomplitionList = PrintListOfAutocomplition(LastFoundList);
					break;//автодополнение
				default:ConsolePrintChar(key); break;
				}
			}break;
		default:ConsolePrintChar(key); break;
		}
	}
	return 0;
}
