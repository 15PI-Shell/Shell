#pragma once

#include <stdlib.h>
#include <string.h>

//структура, описывающая объект списка
typedef struct strlistNode
{
	struct strlistNode *next;//следующий в списке
	char* value;//значение
} STRLIST_NODE;

//функция, добавляющая в список указанную строку
void StrlistAdd(STRLIST_NODE** last, char* value);
