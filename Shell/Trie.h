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
	struct TrieNode* children[36];//a..z - возможные потомки этого узла
} TrieNode;

//инициализирует и возвращает корень префиксного дерева
TrieNode* Trie_Create();

//возвращающает узел дерева, которому соотвествует указанная строка
TrieNode* Trie_Find(TrieNode* trieRoot, char* name);

//регистрирация данных в префиксном дереве
void Trie_Register(TrieNode* trieRoot, char* name, void* data);

//реализация обхода в глубину и сохранения всех найденных данных в поддереве в список
void Trie_FindAllValuesInSubtree(TrieNode* node, char* prefix, int prefixLen, SingleLinklistNode** values);