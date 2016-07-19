#pragma once

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "LinkedList.h"
#include "Trie.h"

//встроенные программы:
#include "HelloWorld.h"
#include "Binary.h"
#include "ChangeDirectory.h"
#include "History.h"
#include "tail.h"
#include "cat.h"
#include "diff.h"
#include "ls.h"
#include "MathFunction.h"
#include "StringFunction.h"
#include "echo.h"
#include "ScriptExec.h"
#include "TryCmd.h"
#include "toStr.h"
#include "toDouble.h"
#include "ReadLine.h"
#include "calc.h"

//enum, описывающий какое значение возвращает встроенная программа
typedef enum
{
	BPC_ReturnsInt,
	BPC_ReturnsDouble,
	BPC_ReturnsString,
	BPC_ReturnsNothing
} BPC_Returns;

//фукнция, регистрирующая встроенные программы в префиксном дереве
void BPC_Init();

//фукнция, запускающая встроенную программу по имени.
//принимает список аргументов и указатель на enum, в который будет сохранен тип возвращаемого значения
char* BPC_Execute(char* program, char* args, BPC_Returns* returns);

//функция, возвращающая список всех встроенных программ, начинающихся с заданного префикса
//при префиксе равном имени программы функция не вернёт её саму, тк дополнять больше нечего
SingleLinklistNode* BPC_GetHints(char* prefix);

BPC_Returns BPC_WhatRets(char* name);