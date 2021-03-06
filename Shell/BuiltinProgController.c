﻿#include "BuiltinProgController.h"

//структура, описывающая встроенную программу
typedef struct
{
	char* (*procFuncPtr)(char* args);//указатель на обработчик программы, принимает строку аргументов
	BPC_Returns returns;//какой тип данных возвращает функция
} BPC_Program;

TrieNode* trieRoot;

void RegisterProgram(char* name, char* (*procFuncPtr)(char* args), BPC_Returns returns)
{
	BPC_Program* prg = (BPC_Program*)malloc(sizeof(BPC_Program));
	prg->procFuncPtr = procFuncPtr;
	prg->returns = returns;
	Trie_Register(trieRoot, name, prg);
}

//видимо извне:

void BPC_Init()
{
	trieRoot = Trie_Create();
	//здесь будет список всех реализованных встроенных программ
	RegisterProgram("HelloWorld", HelloWorldProc, BPC_ReturnsNothing);
	RegisterProgram("Binary", BinaryProc, BPC_ReturnsString);
	RegisterProgram("cd", ChangeDirectory, BPC_ReturnsNothing);
	RegisterProgram("History", PrintHistory, BPC_ReturnsString);
	RegisterProgram("tail", TailOfFile, BPC_ReturnsString);
	RegisterProgram("prFile", printFile, BPC_ReturnsNothing);
	RegisterProgram("diff", diff, BPC_ReturnsNothing);
	RegisterProgram("ls", ls, BPC_ReturnsNothing);

	RegisterProgram("calc", calc, BPC_ReturnsDouble);
	RegisterProgram("sin", MathFun_sin, BPC_ReturnsDouble);
	RegisterProgram("cos", MathFun_cos, BPC_ReturnsDouble);
	RegisterProgram("tan", MathFun_tan, BPC_ReturnsDouble);
	RegisterProgram("cot", MathFun_cot, BPC_ReturnsDouble);
	RegisterProgram("log", MathFun_log, BPC_ReturnsDouble);
	RegisterProgram("exp", MathFun_exp, BPC_ReturnsDouble);
	RegisterProgram("sqrt", MathFun_sqrt, BPC_ReturnsDouble);
	RegisterProgram("pow", MathFun_pow, BPC_ReturnsDouble);
	RegisterProgram("max", MathFun_max, BPC_ReturnsDouble);
	RegisterProgram("min", MathFun_min, BPC_ReturnsDouble);
	RegisterProgram("strlen", StringFun_strlen, BPC_ReturnsInt);
	RegisterProgram("echo", Echo, BPC_ReturnsNothing);

	RegisterProgram("scexec", ScriptExec, BPC_ReturnsNothing);
	RegisterProgram("trycmd", TryCmd, BPC_ReturnsNothing);
	RegisterProgram("toStr", toStr, BPC_ReturnsString);
	RegisterProgram("toDouble", toDouble, BPC_ReturnsDouble);
	RegisterProgram("ReadLine", ReadLine, BPC_ReturnsString);
}

BPC_Returns BPC_WhatRets(char* name)
{
	TrieNode* t = Trie_Find(trieRoot, name);
	if (!t)
		return BPC_ReturnsNothing;
	return ((BPC_Program*)t->data)->returns;
}

char* BPC_Execute(char* program, char* args, BPC_Returns* returns)
{
	TrieNode* tn = Trie_Find(trieRoot, program);//ищем строку-название в дереве
	if (0 == tn || 0 == tn->hasAValue)//если строка содержится в дереве и её конец приходится на узел с описанием программы, то продолжаем
		return (char*)-2;//программа не существует
	BPC_Program *prog = tn->data;
	*returns = prog->returns;//заполняем возвращаемое значение
	return prog->procFuncPtr(args);//вызываем обработчик программы
}

SingleLinklistNode* BPC_GetHints(char* prefix)
{
	TrieNode* tn = Trie_Find(trieRoot, prefix);//находим узел, на котором заканчивается заданный префикс
	if (0 == tn)
		return 0;//такого префикса нет в дереве
	SingleLinklistNode* hints = 0;//инициализируем список строк нулём
	char* extPrefix = (char*)malloc(sizeof(char) * 100);//предполагаемая максимальная длина названия программы - 100 символов
	strcpy(extPrefix, prefix);//создаём за префиксом свободное место для записи туда результата передвижения по дереву
	int prefixLen = strlen(prefix);
	for (int i = 0; i < 26; ++i)
		Trie_FindAllValuesInSubtree(tn->children[i], extPrefix, prefixLen, &hints);//обходим всех потомков найденного узла
	free(extPrefix);
	return hints;
}
