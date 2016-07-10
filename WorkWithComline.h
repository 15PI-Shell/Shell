#pragma once
#include <Windows.h>
#include "StringList.h"
#define MAX_CONSOLE_INPUT 100
char *Buff;
HANDLE hConsole;
COORD cor, startPrintPoint;
void ClearComline();
void ReprintConsoleBuffer();
int IncCursor(int currentPos);
int DecCursor(int currentPos);
void ResetCur(int currentPos);
void AddNewCommandInHistory(char *str, DoubleListStringNode *CurHist);
void OnNextLine(int CurrPos);
