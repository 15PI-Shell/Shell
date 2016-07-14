#include "VarManager.h"

typedef struct
{
	BPC_Returns type;
	void* value;
} VM_Variable;

TrieNode* trieRoot;

void* VM_GetVariable(char* name, BPC_Returns* type)
{
	TrieNode* node = Trie_Find(trieRoot, name);
	if (node)
	{
		*type = ((VM_Variable*)node->data)->type;
		return ((VM_Variable*)node->data)->value;
	}
	else
	{
		printf("Trying to get undeclared variable\n");
		return 0;
	}
}

void VM_SetVariable(char* name, void* value)
{
	//SHOW
	BPC_Returns br = BPC_ReturnsNothing;
	VM_GetVariable(name, &br);
	switch (br)
	{
	case BPC_ReturnsInt:
		printf("Setting %s to %d\n", name, (*(int*)value));
		break;
	case BPC_ReturnsDouble:
		printf("Setting %s to %lf\n", name, (*(double*)value));
		break;
	case BPC_ReturnsString:
		printf("Setting %s to \"%s\"\n", name, (char*)value);
		break;
	}
	//SHOW

	TrieNode* node = Trie_Find(trieRoot, name);
	if (node)
		((VM_Variable*)node->data)->value = value;
	else
		printf("Trying to set undeclared variable\n");//TODO error
}

void VM_DeclareVariable(char* name, BPC_Returns type)
{
	printf("Declared: %s as ", name);
	switch (type)
	{
	case BPC_ReturnsInt:
		printf("int\n");
		break;
	case BPC_ReturnsDouble:
		printf("double\n");
		break;
	case BPC_ReturnsString:
		printf("string\n");
		break;
	}

	VM_Variable* v = (VM_Variable*)malloc(sizeof(VM_Variable));

	v->type = type;
	v->value = 0;

	Trie_Register(trieRoot, name, v);
}

void VM_Init()
{
	trieRoot = Trie_Create();
}