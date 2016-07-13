// ParsArgs.cpp: определяет точку входа для консольного приложения.
//
#include <StringList.h>
#include <malloc.h>

void ParsOfArgs(char* str)
{
	SingleListStringNode* args = NULL;
	int flag = 0;
	char* sp;
	int len = strlen(str);
	char*str1;
	for (int i = 0; i<len; str++, i++)
	{
		if (*str != ' ')	      //если не пробел, то проверяем, является ли след. символ кавычкой
		{
			if (*str == '\"')	//если является, то выполняем проверку на закрывающуюся кавычку
			{
				str1 = str;
				str1++;
				for (int j = i; j < len; j++, str1++)
				{
					if (*str1 == '\"')	//если есть закрывающаяся, то поднимаем флаг
					{
						flag = 1;
						break;
					}
				}
				if (flag != 1)	//иначе выходим из функции
				{
					printf("The string is incorrect");
					break;
				}

			}
			else                //если не кавычка
			{
				if (flag != 1)	//если флаг опущен, забираем токен  
				{
					sp = strtok(str, " ");
				}
				else            //если флаг поднят, то забираем содержимое кавычек и опускаем флаг
				{
					sp = strtok(str, "\"");
					flag = 0;
				}
				SingleStrlistAddDownmost(&args, sp);	//кладём в список наши аргументы
				len -= strlen(str);				//уменьшаем длину строки
				str += strlen(sp);				//перепрыгиваем через слово, которое уже в списке
			}
		}
	}
}
