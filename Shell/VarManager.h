#pragma once

#include "BuiltinProgController.h"
#include "Trie.h"

void* VM_GetVariable(TrieNode* trieRoot, char* name, int* constant, BPC_Returns* type);
void VM_DeclareVariable(TrieNode* trieRoot, char* name, int constant, BPC_Returns type);