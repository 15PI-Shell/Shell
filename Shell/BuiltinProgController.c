#include "BuiltinProgController.h"

//структура, описывающая встроенную программу
typedef struct BPC_Program
{
	void* (*procFuncPtr)(STRLIST_NODE* args);//указатель на обработчик программы, принимает список аргументов
	BPC_RETURNS returns;//какой тип данных возвращает функция
} BPC_PROGRAM;

//структура, описывающая узел префиксного дерева
typedef struct TrieNode
{
	char key;//буква, соотвествующая этому узлу
	int hasAValue;//содержит ли описание программы или лишь является промежуточным узлом
	BPC_PROGRAM functionDesc;//описание программы
	struct TrieNode* children[26];//a..z - возможные потомки этого узла
} TRIE_NODE;

TRIE_NODE* trieRoot = 0;//корень префиксного дерева

//функция, возвращающая узел дерева, которому соотвествует указанная строка
TRIE_NODE* FindInTrie(TRIE_NODE* node, char* name)
{
	if (0 == node)
		return 0;//узел не найден

	if (0 == *(name + 1))
		return node;//если следующий символ строки - 0, то возвращаем этот узел - работа закончена.

	//name + 1 каждую итерацию как бы откусывает первый символ от строки
	return FindInTrie(node->children[tolower(name[1]) - 'a'], name + 1);//дерево регистронезависимо, поэтому переводим любой символ в нижний регистр
}

//функция, регистрирующая программу в контроллере
void RegisterProgram(TRIE_NODE** node, char* name, void* (*procFuncPtr)(STRLIST_NODE* args), BPC_RETURNS returns)
{
	if (0 == *node)
	{//создаём новый узел в префиксном дереве, если он ещё не существует
		*node = (TRIE_NODE*)malloc(sizeof(TRIE_NODE));
		for (int i = 0; i < 26; ++i)
			(*node)->children[i] = 0;//зануляем всех потомков - их ещё нет
		(*node)->hasAValue = 0;//по умолчанию этот узел не содержит описание программы
		(*node)->key = *name;//подписываем текущий узел
	}

	if (0 == *(name + 1))
	{//если следующий символ строки 0 - мы на месте
		(*node)->hasAValue = 1;
		(*node)->functionDesc.procFuncPtr = procFuncPtr;
		(*node)->functionDesc.returns = returns;//заполняем поля и отмечаем узел, как содержащий программу
		return;
	}

	RegisterProgram(&(*node)->children[tolower(name[1]) - 'a'], name + 1, procFuncPtr, returns);
}

//функция, реализующая обход в глубину и сохраняющая все найденные названия программ в список
void FindAllValuesInSubtree(TRIE_NODE* node, char* prefix, int prefixLen, STRLIST_NODE** values)
{
	if (0 == node)
		return;//тупик
	
	prefix[prefixLen] = node->key;//проходя по дереву продолжаем наращивать строку-название программы

	if (node->hasAValue)//если в этом узле содержится описание программы, значит путь который мы прошли - её название
	{
		prefix[prefixLen + 1] = 0;//устанавливаем финальный 0
		char* toInsert = (char*)malloc(sizeof(char) * (prefixLen + 1));
		strcpy(toInsert, prefix);//копируем ответ в новую строку для помещения в список
		StrlistAdd(values, toInsert);//помещаем в конец списка
	}

	for (int i = 0; i < 26; ++i)
		FindAllValuesInSubtree(node->children[i], prefix, prefixLen + 1, values);//продолжаем обход по всем возможным потомкам
}

//видимый извне блок:

void BPC_Init()
{
	//здесь будет список всех реализованных встроенных программ
	RegisterProgram(&trieRoot, "HelloWorld", HelloWorldProc, BPC_ReturnsNothing);
	RegisterProgram(&trieRoot, "HelloAcuion", HelloWorldProc, BPC_ReturnsNothing);
}

void* BPC_Execute(char* program, STRLIST_NODE* args, BPC_RETURNS* returns)
{
	TRIE_NODE* tn = FindInTrie(trieRoot, program);//ищем строку-название в дереве
	if (0 == tn || 0 == tn->hasAValue)//если строка содержится в дереве и её конец приходится на узел с описанием программы, то продолжаем
		return (void*)-2;//программа не существует
	*returns = tn->functionDesc.returns;//заполняем возвращаемое значение
	return tn->functionDesc.procFuncPtr(args);//вызываем обработчик программы
}

STRLIST_NODE* BPC_GetHints(char* prefix)
{
	TRIE_NODE* tn = FindInTrie(trieRoot, prefix);//находим узел, на котором заканчивается заданный префикс
	if (0 == tn)
		return 0;//такого префикса нет в дереве
	STRLIST_NODE* hints = 0;//инициализируем список строк нулём
	char* extPrefix = (char*)malloc(sizeof(char) * 100);//предполагаемая максимальная длина названия программы - 100 символов
	strcpy(extPrefix, prefix);//создаём за префиксом свободное место для записи туда результата передвижения по дереву
	int prefixLen = strlen(prefix);
	for (int i = 0; i < 26; ++i)
		FindAllValuesInSubtree(tn->children[i], extPrefix, prefixLen, &hints);//обходим всех потомков найденного узла
	free(extPrefix);
	return hints;
}
