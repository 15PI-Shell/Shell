#include "LinkedList.h"

void* MkMemCopy(void* str, int length)
{
	void* new_str = malloc(length);
	memcpy(new_str, str, length);
	return new_str;
}

//односвязный список:

SingleLinklistNode* SingleCreateAndLink(void* value, SingleLinklistNode* down, SingleLinklistNode* up)
{
	SingleLinklistNode* newnode = (SingleLinklistNode*)malloc(sizeof(SingleLinklistNode));
	newnode->up = up;
	newnode->value = value;
	if (down)
		down->up = newnode;
	return newnode;
}

void SingleLinklistAddDownmost(SingleLinklistNode** downmost, void* value, int length)
{
	value = MkMemCopy(value, length);

	*downmost = SingleCreateAndLink(value, 0, *downmost);
}

void SingleLinklistInsertAbove(SingleLinklistNode* node, void* value, int length)
{
	assert(node);//нельзя ничего вставить над нулём
	value = MkMemCopy(value, length);
	SingleCreateAndLink(value, node, node->up);
}

void SingleLinklistAddUpmost(SingleLinklistNode** node, void* value, int length)
{
	value = MkMemCopy(value, length);

	if (0 == *node)
	{
		*node = SingleCreateAndLink(value, 0, 0);
		return;
	}

	SingleLinklistNode* iter = *node;
	while (iter->up)
		iter = iter->up;
	SingleLinklistInsertAbove(iter, value, length);
}

void SingleLinklistRemoveDownmost(SingleLinklistNode** downmost)
{
	SingleLinklistNode* reassign = (*downmost)->up;
	free(*downmost);
	*downmost = reassign;
}

void SingleLinklistRemoveUpmost(SingleLinklistNode* node)
{
	assert(node && node->up);//нельзя ничего удалить над нулём и самым верхним элементом

	SingleLinklistNode* iter = node;
	while (iter->up->up)
		iter = iter->up;

	SingleLinklistRemoveNextUp(iter);
}

void SingleLinklistRemoveNextUp(SingleLinklistNode* node)
{
	assert(node && node->up);//нельзя ничего удалить над нулём и самым верхним элементом

	SingleLinklistNode* reassign = 0;
	if (node->up->up)
		reassign = node->up->up;

	free(node->up);
	node->up = reassign;
}

void SingleLinklistConcat(SingleLinklistNode* up, SingleLinklistNode** down)
{
	if (0 == up)
		return;
	if (0 == *down)
	{
		*down = up;
		return;
	}
	SingleLinklistNode* iter1 = *down;
	while (iter1->up)
		iter1 = iter1->up;
	iter1->up = up;
}

//двусвязный список:

DoubleLinklistNode* DoubleCreateAndLink(void* value, DoubleLinklistNode* down, DoubleLinklistNode* up)
{
	DoubleLinklistNode* newnode = (DoubleLinklistNode*)malloc(sizeof(DoubleLinklistNode));
	
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

void DoubleLinklistInsertUnder(DoubleLinklistNode* node, void* value, int length)
{
	assert(node);//нельзя ничего вставить над нулём
	value = MkMemCopy(value, length);
	DoubleCreateAndLink(value, node->down, node);
}

void DoubleLinklistInsertAbove(DoubleLinklistNode* node, void* value, int length)
{
	assert(node);//нельзя ничего вставить над нулём
	value = MkMemCopy(value, length);
	DoubleCreateAndLink(value, node, node->up);
}

void DoubleLinklistAddDownmost(DoubleLinklistNode** node, void* value, int length)
{
	value = MkMemCopy(value, length);

	if (0 == *node)
	{
		*node = DoubleCreateAndLink(value, 0, 0);
		return;
	}

	DoubleLinklistNode* iter = *node;
	while (iter->down)
		iter = iter->down;

	DoubleCreateAndLink(value, 0, iter);
}

void DoubleLinklistAddUpmost(DoubleLinklistNode** node, void* value, int length)
{
	value = MkMemCopy(value, length);

	if (0 == *node)
	{
		*node = DoubleCreateAndLink(value, 0, 0);
		return;
	}

	DoubleLinklistNode* iter = *node;
	while (iter->up)
		iter = iter->up;

	DoubleCreateAndLink(value, iter, 0);
}

void DoubleLinklistRemoveDownmost(DoubleLinklistNode** node)
{
	if (0 == *node)
		return;

	DoubleLinklistNode** iter = node;
	while ((*iter)->down)
		(*iter) = (*iter)->down;

	if ((*iter)->up)
		(*iter)->up->down = 0;

	free(*iter);
	*iter = 0;
}

void DoubleLinklistRemoveUpmost(DoubleLinklistNode** node)
{
	if (0 == *node)
		return;

	DoubleLinklistNode** iter = node;
	while ((*iter)->up)
		(*iter) = (*iter)->up;

	if ((*iter)->down)
		(*iter)->down->up = 0;

	free(*iter);
	*iter = 0;
}

void DoubleLinklistRemove(DoubleLinklistNode** node)
{
	if (0 == *node)
		return;

	DoubleLinklistNode* assign;

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

void DoubleLinklistConcat(DoubleLinklistNode* up, DoubleLinklistNode** down)
{
	if (0 == up)
		return;
	if (0 == *down)
	{
		*down = up;
		return;
	}
	DoubleLinklistNode* iter1 = *down;
	while (iter1->up)
		iter1 = iter1->up;
	DoubleLinklistNode* iter2 = up;
	while (iter2->down)
		iter2 = iter2->down;
	iter1->up = iter2;
	iter2->down = iter1;
}