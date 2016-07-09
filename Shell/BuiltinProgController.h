#ifndef BuiltinProgController
#define BuiltinProgController

#include <stdlib.h>
#include <string.h>
#include "StringList.h"

#include "HelloWorld.h"
#include "sum.h"

//enum, описывающий какое значение возвращает встроенная программа
typedef enum BPC_Returns
{
	BPC_ReturnsInt,
	BPC_ReturnsDouble,
	BPC_ReturnsString,
	BPC_ReturnsNothing
} BPC_RETURNS;

//фукнция, регистрирующая встроенные программы в префиксном дереве
void BPC_Init();

//фукнция, запускающая встроенную программу по имени.
//принимает список аргументов и указатель на enum, в который будет сохранен тип возвращаемого значения
char* BPC_Execute(char* program, char* args, BPC_RETURNS* returns);

//функция, возвращающая список всех встроенных программ, начинающихся с заданного префикса
//при префиксе равном имени программы функция не вернёт её саму, тк дополнять больше нечего
STRLIST_NODE* BPC_GetHints(char* prefix);

#endif
