#pragma once
#include <Windows.h>
#include "StringList.h"
#define MAX_CONSOLE_INPUT 100
char *Buff;
HANDLE hConsole;
COORD cor, startPrintPoint;
DoubleListStringNode *CurrHist;
int cur;
void ClearComline();
void ReprintConsoleBuffer();
void IncCursor();
void DecCursor();
void ResetCur();
void OnNextLine();
void ConsoleInitialisation();
void ConsolePrintChar(int, int);
void ConsoleCursorMoveLeft();
void ConsoleCursoreMoveRight();
void ConsoleGetNextHistory( int * );
void ConsoleGetPrewHistory(  int *);
void ConsoleBackSpace();
void ConsoleDeleteCurElem();
void ConsoleEnter( int *flagOfAutocomplition);
