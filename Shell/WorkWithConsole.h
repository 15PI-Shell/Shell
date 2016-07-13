#pragma once
#include <Windows.h>
#include "StringList.h"
#include "KeyDefines.h"
#include "FindFile.h"
#include"BuiltinProgController.h"
#include "GetClipboardContent.h"
#include "DirHistory.h"
#include <stdio.h>
#include <string.h>
//#include "Clipboard.h"

#define MAX_CONSOLE_INPUT 300
void ConsoleInitialisation();

void ConsoleCursorMoveLeft();//обработка клавиши <-, стрелка в лево
void ConsoleCursoreMoveRight();//обработка клавиши ->, сдвиг в право
void ConsoleGetNextHistory();//обработка клавиши стрелка вниз, перелистывание истории внмз
void ConsoleGetPrewHistory();//обработка клавиши стрелка вверх, перелистывание истории команд наверх
void ConsoleBackSpace();// обработка клавиши бекспейс, удаление элемента слева от курсора
void ConsoleDeleteCurElem();//обрабока клавиши del, удаление текущего элемента
void ConsoleEnter();//обработка клавиши Enter, передача введеной команды интерпетатору, доавление ее в историю и переход на новую строку
void ConsolePrintChar();//обработка неслужебных клавиш, вывод символа соответств этой клавише
void PrintListOfAutocompletion();//распечатка всезх найденных вариантов автодополнений, возвращает кол-во занимаемых строк
void ConsoleAutocompletion() ;// основная функция автодополнения, в флаге отражего напечатан ли список автодополнений, нет-0, да-кол-во занимаемых строк 

void PastInConsole();