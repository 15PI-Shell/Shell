#include "StringList.h"

void StrlistAdd(STRLIST_NODE** last, char* value)
{
	STRLIST_NODE* node = (STRLIST_NODE*)malloc(sizeof(STRLIST_NODE));
	node->value = value;
	node->next = *last;//создаём объект списка, записываем в него значение, указываем следующий элемент как последний
	*last = node;//устанавливаем созданный элемент как последний
}
