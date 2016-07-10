#pragma once
#include <Windows.h>
#include "StringList.h"
#include "KeyDefines.h"
#include "FindFile.h"
#include <stdio.h>

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
void ConsolePrintChar(int);
void ConsoleCursorMoveLeft();
void ConsoleCursoreMoveRight();
void ConsoleGetNextHistory(int *);
void ConsoleGetPrewHistory(int *);
void ConsoleBackSpace();
void ConsoleDeleteCurElem();
void ConsoleEnter(int *flagOfAutocomplition);

int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions);//распечатка всезх найденных вариантов автодополнений, возвращает кол-во занимаемых строк
void DeleteListOfAutocomlition(int n);//если распечатанные автодополнения не актуальны, убираем с экрана
void ConsoleAutocomplition(int *, SingleListStringNode*) ;// основная функция автодополнения, в флаге отражего напечатан ли список автодополнений, нет-0, да-кол-во занимаемых строк 
int  DetermineEntry(char *Buff, int Buflen, char *entry, int *PosEntryStart);// определяет момент, с которого начнется автодополнения, возвращает 1 - если автодополнение комманды, 2-автодополнение имени файла, 0-в случае ошибки(например если буффер пуст)
void ChekFFlagOfAutoComplition(int *flag);
