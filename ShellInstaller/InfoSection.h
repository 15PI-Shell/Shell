#pragma once

#include <Windows.h>
#include <stdio.h>

#include "InstallerConfig.h"

#define RESERVED_32_kb 32768
#define RESERVED_40_kb 40960
#define RESERVED_2_b 2
#define RESERVED_512_b 512
#define RESERVED_4_b 4


char* InfSect(int* Size, InstallerConfig* struc);
