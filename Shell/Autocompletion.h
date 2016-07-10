#pragma once
#include "FindFile.h"
#include "KeyDefines.h"
#include "WorkWithComline.h"

//void ComplitionOfCommand(char *entry, int EnLen);//поиск автодополнений в листе команд 
int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions);//распечатка всезх найденных вариантов автодополнений, возвращает кол-во занимаемых строк
void DeleteListOfAutocomlition(int n);//если распечатанные автодополнения не актуальны, убираем с экрана
void ConsoleAutocomplition( int Buflen, int *flag);// основная функция автодополнения, в флаге отражего напечатан ли список автодополнений, нет-0, да-кол-во занимаемых строк 
int  DetermineEntry(char *Buff, int Buflen, char *entry, int *PosEntryStart);// определяет момент, с которого начнется автодополнения, возвращает 1 - если автодополнение комманды, 2-автодополнение имени файла, 0-в случае ошибки(например если буффер пуст)
void ChekFFlagOfAutoComplition(int *flag);
