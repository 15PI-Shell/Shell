#ifndef FindFile
#define FindFile

#include <windows.h>
#include "StringList.h"

//функция поиска файла по префиксу
STRLIST_NODE* FindFile(char* FileMask);

#endif