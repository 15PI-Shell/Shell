#include "Shell.h"
int main()
{
	
	BPC_Init();
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = cor.Y = 0;
	DoubleListStringNode *CurrHist = 0;
	startPrintPoint.X = startPrintPoint.Y = 0;
	char*str;
	DoubleStrlistAddUpmost(&CurrHist, "");//добавляем "ничто" в историю

	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2); 
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//поправка на перетаскивание символов backspace'ом
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
			case key_left: cur = ConsoleCursorMoveLeft(cur);
				break;
			case key_right:cur = ConsoleCursoreMoveRight(cur);
				break;
			case key_up: ConsoleGetPrewHistory(&cur, CurrHist, &flagOfAutocomplitionList);
				break;
			case key_down:ConsoleGetNextHistory(&cur, CurrHist, &flagOfAutocomplitionList);
				break;
			case key_del: ConsoleDeleteCurElem(cur); break;
			}
			break;
		case key_enter:str = (char*)malloc(MAX_CONSOLE_INPUT + 2);
			str = ConsoleEnter(CurrHist, &flagOfAutocomplitionList, cur); 
			AddNewCommandInHistory(str, CurrHist);
			break;
			
		case key_BackSpase:ConsoleBackSpace(cur);
			break;
		case key_tab:ConsoleAutocomplition( buffLen, &flagOfAutocomplitionList);//автодополнение
			break;
		default:
			if (buffLen < MAX_CONSOLE_INPUT && isprint(key) && !(key >= 'а' && key <= 'я') && !(key >= 'А' && key <= 'Я'))
			{
				//если символ печатаем и не кириллица (ну её)
				buffLen = strlen(Buff);
				for (int i = buffLen; i > cur; --i)
					Buff[i] = Buff[i - 1];
				Buff[cur] = (char)key;
				cur=IncCursor(cur);
				ReprintConsoleBuffer();
			}
			break;
		}
	}
	return 0;
}
