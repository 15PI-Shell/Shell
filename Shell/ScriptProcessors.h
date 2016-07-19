#pragma once
#include "ScriptParsers.h"
#include "VarManager.h"
#include <math.h>
#include "MathInterpreter.h"

void ProcDeclaresAs(InterpData* inter, TrieNode* VM);// , BPC_Returns type);

void GetTerm(InterpData* inter, TrieNode* VM, char* term, void** result);// , BPC_Returns *type);

int StringTermProc(InterpData* inter, TrieNode* VM, void** result);

char* ProcFunction(InterpData* inter, TrieNode* VM, char* name);// , BPC_Returns* type);

void ProcAssignment(InterpData* inter, TrieNode* VM);

char* ProcSingleCondition(InterpData* inter);

char* ProcArgumets(InterpData* inter, TrieNode* VM);

//уже без (
int ProcCondition(InterpData* inter, TrieNode* VM);