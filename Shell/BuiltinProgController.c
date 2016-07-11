#include "BuiltinProgController.h"

//структура, описывающая встроенную программу
typedef struct
{
	char* (*procFuncPtr)(char* args);//указатель на обработчик программы, принимает строку аргументов
	BPC_Returns returns;//какой тип данных возвращает функция
} BPC_Program;

//структура, описывающая узел префиксного дерева
typedef struct
{
	char key;//буква, соотвествующая этому узлу
	int hasAValue;//содержит ли описание программы или лишь является промежуточным узлом
	BPC_Program functionDesc;//описание программы
	struct TrieNode* children[26];//a..z - возможные потомки этого узла
} TrieNode;

TrieNode* trieRoot = 0;//корень префиксного дерева

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
TrieNode* FindInTrie(char* name)
{
	if (!(tolower(name[0]) >= 'a' && tolower(name[0]) <= 'z'))
		return 0;
	return FindInTrieRec(trieRoot->children[tolower(name[0]) - 'a'], name);
}

//функция, регистрирующая программу в контроллере
void RegisterProgramRec(TrieNode** node, char* name, char* (*procFuncPtr)(char* args), BPC_Returns returns)
{
	if (0 == *node)
	{//создаём новый узел в префиксном дереве, если он ещё не существует
		*node = (TrieNode*)malloc(sizeof(TrieNode));
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

	RegisterProgramRec(&(*node)->children[tolower(name[1]) - 'a'], name + 1, procFuncPtr, returns);
}

//оболочка для запуска рекурсивной функции
void RegisterProgram(char* name, char* (*procFuncPtr)(char* args), BPC_Returns returns)
{
	RegisterProgramRec(&trieRoot->children[tolower(name[0]) - 'a'], name, procFuncPtr, returns);
}

//функция, реализующая обход в глубину и сохраняющая все найденные названия программ в список
void FindAllValuesInSubtree(TrieNode* node, char* prefix, int prefixLen, SingleListStringNode** values)
{
	if (0 == node)
		return;//тупик
	
	prefix[prefixLen] = node->key;//проходя по дереву продолжаем наращивать строку-название программы

	if (node->hasAValue)//если в этом узле содержится описание программы, значит путь который мы прошли - её название
	{
		prefix[prefixLen + 1] = 0;//устанавливаем финальный 0
		SingleStrlistAddDownmost(values, prefix);//помещаем в конец списка
	}

	for (int i = 0; i < 26; ++i)
		FindAllValuesInSubtree(node->children[i], prefix, prefixLen + 1, values);//продолжаем обход по всем возможным потомкам
}

//видимый извне блок:

void BPC_Init()
{
	trieRoot = (TrieNode*)malloc(sizeof(TrieNode));
	for (int i = 0; i < 26; ++i)
		trieRoot->children[i] = 0;
	trieRoot->hasAValue = 0;
	trieRoot->key = 0;//инициализируем корень, он не содержит никаких значений. Используется чтобы не создавать по дереву на каждую букву

	//здесь будет список всех реализованных встроенных программ
	RegisterProgram("HelloWorld", HelloWorldProc, BPC_ReturnsNothing);
	RegisterProgram("HelloAcuion", HelloWorldProc, BPC_ReturnsNothing);
	RegisterProgram("Sum", sumProc, BPC_ReturnsInt);
	RegisterProgram("Binary", BinaryProc, BPC_ReturnsInt);
}

char* BPC_Execute(char* program, char* args, BPC_Returns* returns)
{
	TrieNode* tn = FindInTrie(program);//ищем строку-название в дереве
	if (0 == tn || 0 == tn->hasAValue)//если строка содержится в дереве и её конец приходится на узел с описанием программы, то продолжаем
		return (char*)-2;//программа не существует
	*returns = tn->functionDesc.returns;//заполняем возвращаемое значение
	return tn->functionDesc.procFuncPtr(args);//вызываем обработчик программы
}

SingleListStringNode* BPC_GetHints(char* prefix)
{
	TrieNode* tn = FindInTrie(prefix);//находим узел, на котором заканчивается заданный префикс
	if (0 == tn)
		return 0;//такого префикса нет в дереве
	SingleListStringNode* hints = 0;//инициализируем список строк нулём
	char* extPrefix = (char*)malloc(sizeof(char) * 100);//предполагаемая максимальная длина названия программы - 100 символов
	strcpy(extPrefix, prefix);//создаём за префиксом свободное место для записи туда результата передвижения по дереву
	int prefixLen = strlen(prefix);
	for (int i = 0; i < 26; ++i)
		FindAllValuesInSubtree(tn->children[i], extPrefix, prefixLen, &hints);//обходим всех потомков найденного узла
	free(extPrefix);
	return hints;
}
