#pragma once

#include <stdlib.h>
#include <math.h>
#include "BuiltinProgController.h"
#include "MathInterpreter.h"
#include "ParserOfArguments.h"
#include "LinkedList.h"
#include "Arguments.h"

char* MathFun_sin (char* arg);
char* MathFun_cos (char* arg);
char* MathFun_tan (char* arg);
char* MathFun_cot (char* arg);
char* MathFun_log (char* arg);
char* MathFun_exp (char* arg);
char* MathFun_sqrt(char* arg);
char* MathFun_pow (char* arg);
char* MathFun_max (char* arg);
char* MathFun_min(char* arg);