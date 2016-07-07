#ifndef BuiltinProgController
#define BuiltinProgController

#include <stdlib.h>
#include <string.h>
#include <cctype>
#include "StringList.h"

#include "HelloWorld.h"

//enum, описывающий какое значение возвращает встроенная программа
enum BPC_Returns
{
	BPC_ReturnsInt,
	BPC_ReturnsFloat,
	BPC_ReturnsString,
	BPC_ReturnsNothing
};

//фукнция, регистрирующая встроенные программы в префиксном дереве
void BPC_Init();

//фукнция, запускающая встроенную программу по имени.
//принимает список аргументов и указатель на enum, в который будет сохранен тип возвращаемого значения
void* BPC_Execute(char* program, STRLIST_NODE* args, BPC_Returns* returns);

//функция, возвращающая список всех встроенных программ, начинающихся с заданного префикса
//при префиксе равном имени программы функция не вернёт её саму, тк дополнять больше нечего
STRLIST_NODE* BPC_GetHints(char* prefix);

#endif