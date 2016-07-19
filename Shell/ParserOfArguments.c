#include "ParserOfArguments.h"

int ParsOfArgs(char* str, SingleLinklistNode** args)
{
	if (0 == str)
		return 0;
	char* sp;
	int len = strlen(str);
	char*str1;
	str1 = str;
	int cnt_args = 0;  //подсчет аргументов
	int cnt_quo = 0; //подсчет кавычек
	int cnt_spc = 0; //переменная для подсчета пробелов

	for (int i = 0; i < len; i++, str1++) //проверка на количество кавычек
	{
		if (*str1 == '\"')
		{
			cnt_quo++;
		}
	}
	if (cnt_quo % 2 != 0) //нечетно - ошибка
	{
		printf("The string is incorrect");
		return 0;
	}

	cnt_quo = 0;

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
				cnt_quo += 2;
			}
			if (sp != 0)//wtf?
			{
				SingleLinklistAddDownmost(args, sp, strlen(sp)); //кладем в список аргументы
				cnt_args++;
				
				len -= strlen(sp) + cnt_spc + cnt_quo;	//длина строки высчитывается заново, вычитая предыдущее содержимое строки
				str += strlen(sp);					//прыгаем через слово, которое уже внесено в список
				cnt_spc = 0;
				cnt_quo = 0;
			}
		}
		else cnt_spc++; //иначе считаем количество пробелов
	}
	return cnt_args;
}
