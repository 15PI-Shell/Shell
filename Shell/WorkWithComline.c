#include "WorkWithComline.h"
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

int IncCursor(int currentPos)
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	currentPos++;
	cor.Y = (currentPos + startPrintPoint.Y * inf.dwSize.X) / inf.dwSize.X;
	cor.X = (currentPos + startPrintPoint.Y * inf.dwSize.X) % inf.dwSize.X;
	return currentPos;
}


int DecCursor(int currentPos)
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	currentPos--;
	cor.Y = (currentPos + startPrintPoint.Y * inf.dwSize.X) / inf.dwSize.X;
	cor.X = (currentPos + startPrintPoint.Y * inf.dwSize.X) % inf.dwSize.X;
	return currentPos;
}
void ResetCur(int *currentPos)
{
	currentPos = 0;
}
void AddNewCommandInHistory(char *str, DoubleListStringNode *CurHist)
{
	DoubleStrlistAddDownmost(&CurHist, str);
	while (CurHist->down)//сбрасываем указатель истории, всегда сидим в самом низу
		CurHist = CurHist->down;
}
void OnNextLine(int *cur)
{
	cor.X = 0;
cor.Y++;
startPrintPoint.Y = cor.Y;
SetConsoleCursorPosition(hConsole, cor);
ResetCur(cur);
memset(Buff, 0, MAX_CONSOLE_INPUT);
}
void ConsoleInitialisation()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = cor.Y = 0;
	startPrintPoint.X = startPrintPoint.Y = 0;
	DoubleStrlistAddUpmost(&CurrHist, "");//добавляем "ничто" в историю
	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//поправка на перетаскивание символов backspace'ом
}