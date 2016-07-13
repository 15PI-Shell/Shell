#ifndef StringList
#define StringList

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//делает копию строки
char* MkStrCopy(char* str);

//структура, описывающая объект односвязного списка
typedef struct SingleListStringNode
{
	struct SingleListStringNode *up;//выше по списку
	char* value;//значение
} SingleListStringNode;

//Добавляает в односвязный список копию указанной строки в самый низ (должен быть передан нижний элемент списка)
//Указатель будет перемещён на этот элемент
void SingleStrlistAddDownmost(SingleListStringNode** downmost, char* value);
//Добавляает в односвязный список копию указанной строки выше указанного элемента
void SingleStrlistInsertAbove(SingleListStringNode* node, char* value);
//Добавляает в односвязный список копию указанной строки в самый верх списка (передавать можно любой элемент)
//Если это указатель на 0, то вместо добавления, функция перезапишет его созданным элементом
void SingleStrlistAddUpmost(SingleListStringNode** node, char* value);
//Удаляет из односвязного списка самый нижний элемент (должен быть передан он)
//Указатель будет перемещён на элемент выше данного
void SingleStrlistRemoveDownmost(SingleListStringNode** downmost);
//Удаляет самый верхний элемент из односвязного списка (передавать можно любой элемент, кроме самого верхнего)
void SingleStrlistRemoveUpmost(SingleListStringNode* node);
//Удаляет элемент выше данного, не нарушая структуру односвязного списка
void SingleStrlistRemoveNextUp(SingleListStringNode* node);
//присоединяет к нижнему листу верхний
//up и down должны указывать на самый нижний элемент списка
void SingleStrlistConcat(SingleListStringNode* up, SingleListStringNode* down);

//структура, описывающая объект двусвязного списка
typedef struct DoubleListStringNode
{
	struct DoubleListStringNode *up, *down;
	char* value;
} DoubleListStringNode;

//Добавляает в двусвязный список копию указанной строки ниже указанного элемента
void DoubleStrlistInsertUnder(DoubleListStringNode* node, char* value);
//Добавляает в двусвязный список копию указанной строки выше указанного элемента
void DoubleStrlistInsertAbove(DoubleListStringNode* node, char* value);
//Добавляает в двусвязный список копию указанной строки в самый низ списка (передавать можно любой элемент)
//Если это указатель на 0, то вместо добавления выше, функция перезапишет его созданным элементом
void DoubleStrlistAddDownmost(DoubleListStringNode** node, char* value);
//Добавляает в двусвязный список копию указанной строки в самый верх списка (передавать можно любой элемент)
//Если это указатель на 0, то вместо добавления, функция перезапишет его созданным элементом
void DoubleStrlistAddUpmost(DoubleListStringNode** node, char* value);
//Удаляет самый нижний элемент из двусвязного списка (передавать можно любой элемент)
void DoubleStrlistRemoveDownmost(DoubleListStringNode** node);
//Удаляет самый верхний элемент из двусвязного списка (передавать можно любой элемент)
void DoubleStrlistRemoveUpmost(DoubleListStringNode** node);
//Удаляет элемент, не нарушая структуру двусвязного списка
//Указатель переместится на элемент ниже данного, если он существует, иначе на верхний. Если это единственный элемент, то установится на 0
void DoubleStrlistRemove(DoubleListStringNode** node);
//присоединяет к нижнему листу верхний (наверх)
void DoubleStrlistConcat(DoubleListStringNode* up, DoubleListStringNode* down);
#endif
