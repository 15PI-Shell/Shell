#pragma once

#include "BuiltinProgController.h"
#include "Trie.h"

void* VM_GetVariable(char* name, BPC_Returns* type);
void VM_SetVariable(char* name, void* value);
void VM_DeclareVariable(char* name, BPC_Returns type);
void VM_Init();