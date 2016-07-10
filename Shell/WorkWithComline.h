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

int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions);//���������� ����� ��������� ��������� ��������������, ���������� ���-�� ���������� �����
void DeleteListOfAutocomlition(int n);//���� ������������� �������������� �� ���������, ������� � ������
void ConsoleAutocomplition(int *flag);// �������� ������� ��������������, � ����� �������� ��������� �� ������ ��������������, ���-0, ��-���-�� ���������� ����� 
int  DetermineEntry(char *Buff, int Buflen, char *entry, int *PosEntryStart);// ���������� ������, � �������� �������� ��������������, ���������� 1 - ���� �������������� ��������, 2-�������������� ����� �����, 0-� ������ ������(�������� ���� ������ ����)
void ChekFFlagOfAutoComplition(int *flag);
