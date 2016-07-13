#ifndef ModifyingRegistry
#define ModifyingRegistry

#include <Windows.h>
#include <string.h>

LSTATUS RegFunStatus;
HKEY shellRegKey, pathRegKey, userRegKey;
char BufPath[10000];
char InstallationPath[MAX_PATH + 1];

//Функция добавления пути установки в PATH
int InstallModifyPath();
//Функция удаления пути установки из PATH
int UninstallModifyPath();

//Функция добавления данных о программе в регистр
int InstallKeyRegistry();
//Функция удаления данных о программе их регистра
int UninstallKeyRegistry();

#endif
