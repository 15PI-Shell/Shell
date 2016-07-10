#pragma once
#include "Autocompletion.h"
#include "WorkWithComline.h"
int ConsoleCursorMoveLeft(int);
int ConsoleCursoreMoveRight(int);
void ConsoleGetNextHistory(int *, DoubleListStringNode*, int *);
void ConsoleGetPrewHistory(int *, DoubleListStringNode*, int *);
void ConsoleBackSpace(int);
void ConsoleDeleteCurElem(int);
char* ConsoleEnter(DoubleListStringNode *CurHist, int *flagOfAutocomplition, int CurPos);
