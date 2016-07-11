#ifndef ShellInstaller
#define ShellInstaller

#include <Windows.h>
#include <string.h>
#include <stdio.h>

HKEY shellRegKey, pathRegKey;
char InstallationPath[MAX_PATH];
char BufPath[10000];
DWORD SizeBufPath = sizeof(BufPath);
LSTATUS RegFunStatus;

#endif