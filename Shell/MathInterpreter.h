#pragma once

#include <stdio.h>//не нужен, но для дебага мб
#include <stdlib.h>
#include "Arguments.h"

typedef long long lld;

//Выполняет вычисление математического выражения
//Возвращает 0 если выражение некорректно, в этом случае в result будет позиция, где встречен некорректный символ
//второй аргумент - указатель, в который будет положен результат
int MathInterpreter(char* expression, double* result);