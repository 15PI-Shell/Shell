#include "Trie.h"

//инициализирует и возвращает корень префиксного дерева
TrieNode* Trie_Create()
{
	TrieNode* trieRoot = (TrieNode*)malloc(sizeof(TrieNode));
	for (int i = 0; i < 26; ++i)
		trieRoot->children[i] = 0;
	trieRoot->hasAValue = 0;
	trieRoot->key = 0;//инициализируем корень, он не содержит никаких значений. Используется чтобы не создавать по дереву на каждую букву
	return trieRoot;
}

//функция, возвращающая узел дерева, которому соотвествует указанная строка
TrieNode* FindInTrieRec(TrieNode* node, char* name)
{
	if (!(tolower(name[0]) >= 'a' && tolower(name[0]) <= 'z'))
		return 0;

	if (0 == node)
		return 0;//узел не найден

	if (0 == *(name + 1))
		return node;//если следующий символ строки - 0, то возвращаем этот узел - работа закончена.

					//name + 1 каждую итерацию как бы откусывает первый символ от строки
	return FindInTrieRec(node->children[tolower(name[1]) - 'a'], name + 1);//дерево регистронезависимо, поэтому переводим любой символ в нижний регистр
}

//оболочка для запуска рекурсивной функции
TrieNode* Trie_Find(TrieNode* trieRoot, char* name)
{
	if (!(tolower(name[0]) >= 'a' && tolower(name[0]) <= 'z'))
		return 0;
	return FindInTrieRec(trieRoot->children[tolower(name[0]) - 'a'], name);
}

//функция, регистрирующая программу в контроллере
void Trie_RegisterRec(TrieNode** node, char* name, void* data)
{
	if (0 == *node)
	{//создаЄм новый узел в префиксном дереве, если он ещЄ не существует
		*node = (TrieNode*)malloc(sizeof(TrieNode));
		for (int i = 0; i < 26; ++i)
			(*node)->children[i] = 0;//зануляем всех потомков - их ещЄ нет
		(*node)->hasAValue = 0;//по умолчанию этот узел не содержит описание программы
		(*node)->key = *name;//подписываем текущий узел
	}

	if (0 == *(name + 1))
	{//если следующий символ строки 0 - мы на месте
		(*node)->hasAValue = 1;
		(*node)->data = data;//заполняем поля и отмечаем узел, как содержащий программу
		return;
	}

	Trie_RegisterRec(&(*node)->children[tolower(name[1]) - 'a'], name + 1, data);
}

//оболочка для запуска рекурсивной функции
void Trie_Register(TrieNode* trieRoot, char* name, void* data)
{
	Trie_RegisterRec(&trieRoot->children[tolower(name[0]) - 'a'], name, data);
}

//функция, реализующая обход в глубину и сохраняющая все найденные названия программ в список
void Trie_FindAllValuesInSubtree(TrieNode* node, char* prefix, int prefixLen, SingleLinklistNode** values)
{
	if (0 == node)
		return;//тупик

	prefix[prefixLen] = node->key;//проходя по дереву продолжаем наращивать строку-название программы

	if (node->hasAValue)//если в этом узле содержится описание программы, значит путь который мы прошли - еЄ название
	{
		prefix[prefixLen + 1] = 0;//устанавливаем финальный 0
		SingleLinklistAddDownmost(values, prefix, strlen(prefix));//помещаем в конец списка
	}

	for (int i = 0; i < 26; ++i)
		Trie_FindAllValuesInSubtree(node->children[i], prefix, prefixLen + 1, values);//продолжаем обход по всем возможным потомкам
}
