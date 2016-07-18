#pragma once

#include "Trie.h"
#include "BuiltinProgController.h"
//?!!
void* VM_GetVariable(TrieNode* trieRoot, char* name, int* constant);// , BPC_Returns* type);
void VM_DeclareVariable(TrieNode* trieRoot, char* name, int constant, int verb);// , BPC_Returns type);
TrieNode* VM_Create();
