#include "Shell.h"
int main()
{
	BPC_Init();

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = cor.Y = 0;
	startPrintPoint.X = startPrintPoint.Y = 0;
	DoubleListStringNode *CurrHist = 0;

	DoubleStrlistAddUpmost(&CurrHist, "");//добавляем "ничто" в историю

	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//поправка на перетаскивание символов backspace'ом

	int key, buffLen, flagOfAutocomplitionList=0;
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
			case key_left:
				if (cur > 0)
				{
					DecCursor();
					SetConsoleCursorPosition(hConsole, cor);
				}
				break;
			case key_right:
				if (Buff[cur])
				{
					IncCursor();
					SetConsoleCursorPosition(hConsole, cor);
				}
				break;
			case key_up:
				if (CurrHist->up != NULL)
				{
					CurrHist = CurrHist->up;
					Buff = CurrHist->value;
					cor.X = 0;
					cur = 0;
					ReprintConsoleBuffer();
				}
				if (flagOfAutocomplitionList != 0)
				{
					DeleteListOfAutocomlition(flagOfAutocomplitionList);
					flagOfAutocomplitionList = 0;
				}
				break;
			case key_down:
				if (CurrHist->down != NULL)
				{
					CurrHist = CurrHist->down;
					Buff = CurrHist->value;
					cor.X = 0;
					cur = 0;
					ReprintConsoleBuffer();
				}
				if (flagOfAutocomplitionList != 0)
				{
					DeleteListOfAutocomlition(flagOfAutocomplitionList);
					flagOfAutocomplitionList = 0;
				}
				break;
			case key_del:
				if (Buff[cur])
				{
					for (int i = cur; i <= buffLen; i++)
						Buff[i] = Buff[i + 1];
					ReprintConsoleBuffer();
				}
				break;
			}
			break;
		case key_enter:
			if (flagOfAutocomplitionList != 0)
			{
				DeleteListOfAutocomlition(flagOfAutocomplitionList);
				flagOfAutocomplitionList = 0;
			}
			cor.X = 0;
			cor.Y++;
			startPrintPoint.Y = cor.Y;
			cur = 0;
			SetConsoleCursorPosition(hConsole, cor);
			DoubleStrlistAddUpmost(&CurrHist, Buff);
			while (CurrHist->down)//сбрасываем указатель истории, всегда сидим в самом низу
				CurrHist = CurrHist->down;
			memset(Buff, 0, MAX_CONSOLE_INPUT);
			break;
		case key_BackSpase:
			if (cur > 0)
			{
				for (int i = cur - 1; i <= buffLen; i++)
					Buff[i] = Buff[i + 1];
				DecCursor();
				ReprintConsoleBuffer();
			}
			break;
		case key_tab:if (flagOfAutocomplitionList != 0)
		{
			DeleteListOfAutocomlition(flagOfAutocomplitionList);
			flagOfAutocomplitionList = 0;
		}
			Autocomplition(Buff, buffLen, &flagOfAutocomplitionList);//автодополнение
			break;
		default:
			if (buffLen < MAX_CONSOLE_INPUT && isprint(key) && !(key >= 'а' && key <= 'я') && !(key >= 'А' && key <= 'Я'))
			{
				//если символ печатаем и не кириллица (ну её)
				buffLen = strlen(Buff);
				for (int i = buffLen; i > cur; --i)
					Buff[i] = Buff[i - 1];
				Buff[cur] = (char)key;
				IncCursor();
				ReprintConsoleBuffer();
			}
			break;
		}
	}
	return 0;
}
