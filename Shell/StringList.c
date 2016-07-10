#include "StringList.h"

char* MkStrCopy(char* str)
{
	char* new_str = (char*)malloc(strlen(str) + 1);
	strcpy(new_str, str);
	return new_str;
}

//односвязный список:

SingleListStringNode* SingleCreateAndLink(char* value, SingleListStringNode* down, SingleListStringNode* up)
{
	SingleListStringNode* newnode = (SingleListStringNode*)malloc(sizeof(SingleListStringNode));
	newnode->up = up;
	newnode->value = value;
	if (down)
		down->up = newnode;
	return newnode;
}

void SingleStrlistAddDownmost(SingleListStringNode** downmost, char* value)
{
	value = MkStrCopy(value);

	*downmost = SingleCreateAndLink(value, 0, *downmost);
}

void SingleStrlistInsertAbove(SingleListStringNode* node, char* value)
{
	assert(node);//нельзя ничего вставить над нулём
	value = MkStrCopy(value);
	SingleCreateAndLink(value, node, node->up);
}

void SingleStrlistAddUpmost(SingleListStringNode** node, char* value)
{
	value = MkStrCopy(value);

	if (0 == *node)
	{
		*node = SingleCreateAndLink(value, 0, 0);
		return;
	}

	SingleListStringNode* iter = *node;
	while (iter->up)
		iter = iter->up;
	SingleStrlistInsertAbove(iter, value);
}

void SingleStrlistRemoveDownmost(SingleListStringNode** downmost)
{
	SingleListStringNode* reassign = (*downmost)->up;
	free(*downmost);
	*downmost = reassign;
}

void SingleStrlistRemoveUpmost(SingleListStringNode* node)
{
	assert(node && node->up);//нельзя ничего удалить над нулём и самым верхним элементом

	SingleListStringNode* iter = node;
	while (iter->up->up)
		iter = iter->up;

	SingleStrlistRemoveNextUp(iter);
}

void SingleStrlistRemoveNextUp(SingleListStringNode* node)
{
	assert(node && node->up);//нельзя ничего удалить над нулём и самым верхним элементом

	SingleListStringNode* reassign = 0;
	if (node->up->up)
		reassign = node->up->up;

	free(node->up);
	node->up = reassign;
}

void SingleStrlistConcat(SingleListStringNode* up, SingleListStringNode* down)
{
	if (0 == up)
		return;
	SingleListStringNode* iter1 = down;
	while (iter1->up)
		iter1 = iter1->up;
	iter1->up = up;
}

//двусвязный список:

DoubleListStringNode* DoubleCreateAndLink(char* value, DoubleListStringNode* down, DoubleListStringNode* up)
{
	DoubleListStringNode* newnode = (DoubleListStringNode*)malloc(sizeof(DoubleListStringNode));
	
	newnode->value = value;

	if (down)
	{
		down->up = newnode;
		newnode->down = down;
	}
	else
		newnode->down = 0;
	if (up)
	{
		up->down = newnode;
		newnode->up = up;
	}
	else
		newnode->up = 0;

	return newnode;
}

void DoubleStrlistInsertUnder(DoubleListStringNode* node, char* value)
{
	assert(node);//нельзя ничего вставить над нулём
	value = MkStrCopy(value);
	DoubleCreateAndLink(value, node->down, node);
}

void DoubleStrlistInsertAbove(DoubleListStringNode* node, char* value)
{
	assert(node);//нельзя ничего вставить над нулём
	value = MkStrCopy(value);
	DoubleCreateAndLink(value, node, node->up);
}

void DoubleStrlistAddDownmost(DoubleListStringNode** node, char* value)
{
	value = MkStrCopy(value);

	if (0 == *node)
	{
		*node = DoubleCreateAndLink(value, 0, 0);
		return;
	}

	DoubleListStringNode* iter = *node;
	while (iter->down)
		iter = iter->down;

	DoubleCreateAndLink(value, 0, iter);
}

void DoubleStrlistAddUpmost(DoubleListStringNode** node, char* value)
{
	value = MkStrCopy(value);

	if (0 == *node)
	{
		*node = DoubleCreateAndLink(value, 0, 0);
		return;
	}

	DoubleListStringNode* iter = *node;
	while (iter->up)
		iter = iter->up;

	DoubleCreateAndLink(value, iter, 0);
}

void DoubleStrlistRemoveDownmost(DoubleListStringNode** node)
{
	if (0 == *node)
		return;

	DoubleListStringNode** iter = node;
	while ((*iter)->down)
		(*iter) = (*iter)->down;

	if ((*iter)->up)
		(*iter)->up->down = 0;

	free(*iter);
	*iter = 0;
}

void DoubleStrlistRemoveUpmost(DoubleListStringNode** node)
{
	if (0 == *node)
		return;

	DoubleListStringNode** iter = node;
	while ((*iter)->up)
		(*iter) = (*iter)->up;

	if ((*iter)->down)
		(*iter)->down->up = 0;

	free(*iter);
	*iter = 0;
}

void DoubleStrlistRemove(DoubleListStringNode** node)
{
	if (0 == *node)
		return;

	DoubleListStringNode* assign;

	if ((*node)->down)
	{
		(*node)->down->up = (*node)->up;
		assign = (*node)->down;
	}
	if ((*node)->up)
	{
		(*node)->up->down = (*node)->down;
		if (0 == assign)
			assign = (*node)->down;
	}

	free(*node);
	*node = assign;
}

void DoubleStrlistConcat(DoubleListStringNode* up, DoubleListStringNode* down)
{
	if (0 == up)
		return;
	DoubleListStringNode* iter1 = down;
	while (iter1->up)
		iter1 = iter1->up;
	DoubleListStringNode* iter2 = up;
	while (iter2->down)
		iter2 = iter2->down;
	iter1->up = iter2;
	iter2->down = iter1;
}