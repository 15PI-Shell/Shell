#include "ParserOfArguments.h"

int ParsOfArgs(char* str, SingleLinklistNode** args)
{
	int flag = 0;
	char* sp;
	int len = strlen(str);
	char*str1;
	str1 = str;
	int cnt = 0;
	int cnt_spc = 0; //переменная для подсчета пробелов

	for (int i = 0; i < len; i++, str1++) //проверка на количество кавычек
	{
		if (*str1 == '\"')
		{
			cnt++;
		}
	}
	if (cnt % 2 != 0) //нечетно - ошибка
	{
		printf("The string is incorrect");
		return 0;
	}

	for (int i = 0; i<len; str++, i++)
	{
		if (*str != ' ')	      //если не пробел, то проверяем, является ли след. символ кавычкой
		{
			if (*str != '\"')
			{
				sp = strtok(str, " \""); //если первый символ не кавычки
			}							//разбивает также слитное написание аргументов
			else
			{
				sp = strtok(str, "\"");	//забирает все содержимое под кавычками
			}
			if (sp != 0)//wtf?
			{
				SingleLinklistAddDownmost(args, sp); //кладем в список аргументы
				len -= strlen(sp) + cnt_spc;		//длина строки высчитывается заново, вычитая предыдущее содержимое строки
				str += strlen(sp);					//прыгаем через слово, которое уже внесено в список
				cnt_spc = 0;
			}
		}
		else cnt_spc++; //иначе считаем количество пробелов
	}
	return 1;
}