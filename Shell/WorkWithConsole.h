﻿#pragma once
#include <Windows.h>
#include "LinkedList.h"
#include "KeyDefines.h"
#include "FindFile.h"
#include "BuiltinProgController.h"
#include "BuiltinProgInterpretator.h"
#include "GetClipboardContent.h"
#include <stdio.h>
#include <string.h>

#define MAX_CONSOLE_INPUT 300

DoubleLinklistNode *CurrHist;

void ConsoleInitialisation();

void ConsoleCursorMoveLeft();//обработка клавиши <-, стрелка в лево
void ConsoleCursoreMoveRight();//обработка клавиши ->, сдвиг в право
void ConsoleGetNextHistory();//обработка клавиши стрелка вниз, перелистывание истории внмз
void ConsoleGetPrewHistory();//обработка клавиши стрелка вверх, перелистывание истории команд наверх
void ConsoleBackSpace();// обработка клавиши бекспейс, удаление элемента слева от курсора
void ConsoleDeleteCurElem();//обрабока клавиши del, удаление текущего элемента
void ConsoleEnter();//обработка клавиши Enter, передача введеной команды интерпетатору, доавление ее в историю и переход на новую строку
void ConsolePrintChar(int key);//обработка неслужебных клавиш, вывод символа соответств этой клавише
void PrintListOfAutocompletion();//распечатка всезх найденных вариантов автодополнений, возвращает кол-во занимаемых строк
void ConsoleAutocompletion() ;// основная функция автодополнения, в флаге отражего напечатан ли список автодополнений, нет-0, да-кол-во занимаемых строк 

void PastInConsole();
