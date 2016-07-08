#include "StringList.h"

void StrlistAdd(STRLIST_NODE** last, char* value)
{
	char* cpy_value = (char*)malloc(strlen(value));
	strcpy(cpy_value, value);
	STRLIST_NODE* node = (STRLIST_NODE*)malloc(sizeof(STRLIST_NODE));
	node->value = cpy_value;
	node->next = *last;//создаём объект списка, записываем в него значение, указываем следующий элемент как последний
	*last = node;//устанавливаем созданный элемент как последний
}
