#include "KeyEvents.h"
int ConsoleCursorMoveLeft(int CurPos)
{
	if (CurPos > 0)
	{
		CurPos = DecCursor(CurPos);
		SetConsoleCursorPosition(hConsole, cor);
		return CurPos;
	}
}
int ConsoleCursoreMoveRight(int CurPos)
{
	if (Buff[CurPos])
	{
		CurPos = IncCursor(CurPos);
		SetConsoleCursorPosition(hConsole, cor);
	}
	return CurPos;
}
void ConsoleGetNextHistory(int *CurPos, DoubleListStringNode *CurrHist, int *flagOfAutocomplition)
{
	if (CurrHist->down != NULL)
	{
		CurrHist = CurrHist->down;
		Buff = CurrHist->value;
		cor.X = 0;
		ResetCur(CurPos);
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
}
void ConsoleGetPrewHistory(int *CurPos, DoubleListStringNode*CurrHist, int *flagOfAutocomplition)
{
	if (CurrHist->up != NULL)
	{
		CurrHist = CurrHist->up;
		Buff = CurrHist->value;
		cor.X = 0;
		ResetCur(CurPos);
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
}
void ConsoleDeleteCurElem(int CurPos)
{
	if (Buff[CurPos])
	{
		int len = strlen(Buff);
		for (int i = CurPos; i <= len; i++)
			Buff[i] = Buff[i + 1];
		ReprintConsoleBuffer();
	}
}
void ConsoleBackSpace(int CurPos)
{
	if (CurPos > 0)
	{
		int len = strlen(Buff);
		for (int i = CurPos - 1; i < len; i++)
			Buff[i] = Buff[i + 1];
		CurPos = DecCursor(CurPos);
		ReprintConsoleBuffer();
	}
}
char* ConsoleEnter(DoubleListStringNode *CurHist, int *flagOfAutocomplition, int CurPos)
{
	char *command;
	command= (char*)malloc(MAX_CONSOLE_INPUT + 2);memset(command, 0, MAX_CONSOLE_INPUT + 2);
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
	strcpy(command, Buff);
	OnNextLine(&CurPos);
	return command;
}