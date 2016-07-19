#include "VarManager.h"

typedef struct
{
	BPC_Returns type;
	void* value;
	int constant;
} VM_Variable;

void* VM_GetVariable(TrieNode* trieRoot, char* name, int* constant, BPC_Returns* type)
{
	TrieNode* node = Trie_Find(trieRoot, name);
	if (node)
	{
		*type = ((VM_Variable*)node->data)->type;
		*constant = ((VM_Variable*)node->data)->constant;
		return ((VM_Variable*)node->data)->value;
	}
	else
	{
		printf("Trying to get undeclared variable\n");
		return 0;
	}
}

void VM_DeclareVariable(TrieNode* trieRoot, char* name, int constant, int verb, BPC_Returns type)
{
	VM_Variable* v = (VM_Variable*)malloc(sizeof(VM_Variable));

	v->type = type;
	v->constant = constant;
	if (verb)
		printf("Declared: %s as ", name);
	switch (type)
	{
	case BPC_ReturnsInt:
		if (verb)
			printf("int\n");
		v->value = malloc(sizeof(int));
		*((int*)v->value) = 0;
		break;
	case BPC_ReturnsDouble:
		if (verb)
			printf("double\n");
		v->value = malloc(sizeof(double));
		*((double*)v->value) = 0;
		break;
	case BPC_ReturnsString:
		if (verb)
			printf("string\n");
		v->value = malloc(sizeof(char) * 1000);
		*((char*)v->value) = 0;
		break;
	}

	Trie_Register(trieRoot, name, v);
}

void MakeConst(TrieNode* root, char* name, double val)
{
	int constant;
	BPC_Returns type;
	VM_DeclareVariable(root, name, 1, 0, BPC_ReturnsDouble);
	double* d = VM_GetVariable(root, name, &constant, &type);
	*d = val;
}

TrieNode* VM_Create()
{
	TrieNode* root = Trie_Create();
	MakeConst(root, "PI", 3.1415926);
	return root;
}