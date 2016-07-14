#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//делает копию строки
void* MkMemCopy(void* str);

//структура, описывающая объект односвязного списка
typedef struct SingleLinklistNode
{
	struct SingleLinklistNode *up;//выше по списку
	void* value;//значение
} SingleLinklistNode;

//Добавляает в односвязный список копию указанной строки в самый низ (должен быть передан нижний элемент списка)
//Указатель будет перемещён на этот элемент
void SingleLinklistAddDownmost(SingleLinklistNode** downmost, void* value, int length);
//Добавляает в односвязный список копию указанной строки выше указанного элемента
void SingleLinklistInsertAbove(SingleLinklistNode* node, void* value, int length);
//Добавляает в односвязный список копию указанной строки в самый верх списка (передавать можно любой элемент)
//Если это указатель на 0, то вместо добавления, функция перезапишет его созданным элементом
void SingleLinklistAddUpmost(SingleLinklistNode** node, void* value, int length);
//Удаляет из односвязного списка самый нижний элемент (должен быть передан он)
//Указатель будет перемещён на элемент выше данного
void SingleLinklistRemoveDownmost(SingleLinklistNode** downmost);
//Удаляет самый верхний элемент из односвязного списка (передавать можно любой элемент, кроме самого верхнего)
void SingleLinklistRemoveUpmost(SingleLinklistNode* node);
//Удаляет элемент выше данного, не нарушая структуру односвязного списка
void SingleLinklistRemoveNextUp(SingleLinklistNode* node);
//присоединяет к нижнему листу верхний
//up и down должны указывать на самый нижний элемент списка
void SingleLinklistConcat(SingleLinklistNode* up, SingleLinklistNode** down);

//структура, описывающая объект двусвязного списка
typedef struct DoubleLinklistNode
{
	struct DoubleLinklistNode *up, *down;
	void* value;
} DoubleLinklistNode;

//Добавляает в двусвязный список копию указанной строки ниже указанного элемента
void DoubleLinklistInsertUnder(DoubleLinklistNode* node, void* value, int length);
//Добавляает в двусвязный список копию указанной строки выше указанного элемента
void DoubleLinklistInsertAbove(DoubleLinklistNode* node, void* value, int length);
//Добавляает в двусвязный список копию указанной строки в самый низ списка (передавать можно любой элемент)
//Если это указатель на 0, то вместо добавления выше, функция перезапишет его созданным элементом
void DoubleLinklistAddDownmost(DoubleLinklistNode** node, void* value, int length);
//Добавляает в двусвязный список копию указанной строки в самый верх списка (передавать можно любой элемент)
//Если это указатель на 0, то вместо добавления, функция перезапишет его созданным элементом
void DoubleLinklistAddUpmost(DoubleLinklistNode** node, void* value, int length);
//Удаляет самый нижний элемент из двусвязного списка (передавать можно любой элемент)
void DoubleLinklistRemoveDownmost(DoubleLinklistNode** node);
//Удаляет самый верхний элемент из двусвязного списка (передавать можно любой элемент)
void DoubleLinklistRemoveUpmost(DoubleLinklistNode** node);
//Удаляет элемент, не нарушая структуру двусвязного списка
//Указатель переместится на элемент ниже данного, если он существует, иначе на верхний. Если это единственный элемент, то установится на 0
void DoubleLinklistRemove(DoubleLinklistNode** node);
//присоединяет к нижнему листу верхний (наверх)
void DoubleLinklistConcat(DoubleLinklistNode* up, DoubleLinklistNode** down);