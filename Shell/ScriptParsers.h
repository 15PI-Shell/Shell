#pragma once

#include <string.h>

typedef struct
{
	char* scr;
	int scptr, scfailed, row;
	int insideLevel;
	int retBackTo[100];
	int verbose;
} InterpData;

int CheckNextSymsFree(int ptr, char* where, char* syms);

int CheckNextSyms(InterpData* inter, char* syms);

void SkipSpaces(InterpData* inter);

char* ParseBeforeComa(InterpData* inter, int delta);

char* ParseBeforeSemi(InterpData* inter);

char* ParseName(InterpData* inter);

char* ParseInsideBrackets(InterpData* inter);

char* ParseInsideQuotes(InterpData* inter);