#pragma once
#include <Windows.h>
#define MAX_CONSOLE_INPUT 100
char *Buff;
HANDLE hConsole;
COORD cor, startPrintPoint;
int cur = 0;
void ClearComline();
void ReprintConsoleBuffer();
void IncCursor();
void DecCursor();