#include "Shell.h"

char *Buff;
HANDLE hConsole;
COORD cor, startPrintPoint;
int cur = 0;

void ClearComline()
{
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	for (int i = 0; i < MAX_CONSOLE_INPUT; ++i)
		printf(" ");
}

void ReprintConsoleBuffer()
{
	ClearComline();
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	printf("%s", Buff);
	SetConsoleCursorPosition(hConsole, cor);
}

void IncCursor()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cur++;
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X) % inf.dwSize.X;
}

void DecCursor()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cur--;
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X) % inf.dwSize.X;
}

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

	int key, buffLen;
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
		case key_tab: //автодополнение
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
