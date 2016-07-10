#include "WorkWithComline.h"
void ConsoleInitialisation()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = cor.Y = 0;
	startPrintPoint.X = startPrintPoint.Y = 0;
	DoubleStrlistAddUpmost(&CurrHist, "");//добавляем "ничто" в историю
	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//поправка на перетаскивание символов backspace'ом
}
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
void ResetCur()
{
	cur = 0;
}
void AddNewCommandInHistory(char *str, DoubleListStringNode *CurHist)
{
	
}
void OnNextLine()
{
	cor.X = 0;
cor.Y++;
startPrintPoint.Y = cor.Y;
SetConsoleCursorPosition(hConsole, cor);
ResetCur();
memset(Buff, 0, MAX_CONSOLE_INPUT);
}
void ConsolePrintChar(int key, int buffLen)
{
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
}

/*-----------Keys------------------------------*/

void ConsoleCursorMoveLeft()
{
	if (cur > 0)
	{
		 DecCursor();
		SetConsoleCursorPosition(hConsole, cor);
		return cur;
	}
}
void ConsoleCursoreMoveRight()
{
	if (Buff[cur])
	{
		 IncCursor();
		SetConsoleCursorPosition(hConsole, cor);
	}
}
void ConsoleGetNextHistory( int *flagOfAutocomplition)
{
	if (CurrHist->down != NULL)
	{
		CurrHist = CurrHist->down;
		Buff = CurrHist->value;
		cor.X = 0;
		ResetCur();
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
}
void ConsoleGetPrewHistory( int *flagOfAutocomplition)
{
	if (CurrHist->up != NULL)
	{
		CurrHist = CurrHist->up;
		Buff = CurrHist->value;
		cor.X = 0;
		ResetCur(cur);
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
}
void ConsoleDeleteCurElem()
{
	if (Buff[cur])
	{
		int len = strlen(Buff);
		for (int i = cur; i <= len; i++)
			Buff[i] = Buff[i + 1];
		ReprintConsoleBuffer();
	}
}
void ConsoleBackSpace()
{
	if ( cur> 0)
	{
		int len = strlen(Buff);
		for (int i = cur - 1; i < len; i++)
			Buff[i] = Buff[i + 1];
		DecCursor();
		ReprintConsoleBuffer();
	}
}
void ConsoleEnter( int *flagOfAutocomplition)
{
	char *str;
	str = (char*)malloc(MAX_CONSOLE_INPUT + 2);memset(str, 0, MAX_CONSOLE_INPUT + 2);
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
	DoubleStrlistAddDownmost(&CurrHist, str);
	while (CurrHist->down)//сбрасываем указатель истории, всегда сидим в самом низу
		CurrHist = CurrHist->down;
	strcmp(CurrHist->value, Buff);
	OnNextLine();
	
}