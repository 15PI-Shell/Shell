#pragma once

#include <string.h>

typedef struct
{
	char* scr;
	int scptr, scfailed, row;
} InterpData;

int CheckNextSyms(InterpData* inter, char* syms);

void SkipSpaces(InterpData* inter);

char* ParseBeforeComa(InterpData* inter);

char* ParseBeforeSemi(InterpData* inter);

char* ParseName(InterpData* inter);

char* ParseInsideBrackets(InterpData* inter);

char* ParseInsideQuotes(InterpData* inter);