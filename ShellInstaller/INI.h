#pragma once

#include <Windows.h>
#include "StringList.h"
#include "InstallerConfig.h"

//На вход получает путь к папке с файлами, которые нужно активировать(в том числе с .ini) и структуру. Возвращает 0 в случае удачи.
int ReadInstConfig(char* PathToFolder, InstallerConfig* Config);