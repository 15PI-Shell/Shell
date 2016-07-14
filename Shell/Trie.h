#pragma once

#include <ctype.h>
#include <stdlib.h>
#include "LinkedList.h"

//структура, описывающая узел префиксного дерева
typedef struct TrieNode
{
	char key;//буква, соотвествующая этому узлу
	int hasAValue;//содержит ли описание программы или лишь является промежуточным узлом
	void* data;//описание программы
	struct TrieNode* children[26];//a..z - возможные потомки этого узла
} TrieNode;

TrieNode* Trie_Find(TrieNode* trieRoot, char* name);
void Trie_Register(TrieNode* trieRoot, char* name, void* data);
void Trie_FindAllValuesInSubtree(TrieNode* node, char* prefix, int prefixLen, SingleLinklistNode** values);