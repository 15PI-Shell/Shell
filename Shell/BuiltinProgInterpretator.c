#include "BuiltinProgController.h"
#include "BuiltinProgInterpretator.h"

char* analisator( char* mas)
{
	int p = 0, lenname = 0, lenarg = 0,flag = 0;
	BPC_RETURNS TypeOfResult;
	char* result = NULL;

	// flag=0 - первый проход; 1 - если есть "|"; 2- выход
	while ((flag == 0) || (flag==1) )
	{
		while (*(mas + p) == ' ')
			p++;

		//находим длину названия команды
		lenname = p;
		while (*(mas + lenname) != ' ')
			lenname++;
		lenname = lenname - p;

		//указатель на строку с названием команды
		char* ptrname = NULL;
		ptrname = (char*)malloc(lenname * sizeof(char));

		*(ptrname + lenname) = '\0';
		int i = 0;
		while (i != lenname)
		{
			*(ptrname + i) = mas[p];
			p++;
			i++;

		}

		if (flag == 0)
		{ 
			//находим длину строки аргументов
			lenarg = p;
			while ((*(mas + lenarg) != '\0') && (*(mas + lenarg) != '|'))
				lenarg++;
			lenarg = lenarg - p;

			// указатель на строку с аругментами
			char* ptrarg = (char*)malloc(lenarg * sizeof(char));
			i = 0;
			while (i != lenarg)
			{
				*(ptrarg + i) = mas[p];
				p++;
				i++;
	
			}
			*(ptrarg + lenarg) = '\0';

			result = BPC_Execute(ptrname, ptrarg, &TypeOfResult);
			printf("%c", *(mas + p));
			if (*(mas+p) == '\0')
				flag = 2;
			else
			{
				p++;
				flag = 1;
			}
		}
		else
		{
			// помещаем результат предыдущей функции в строку аргументов следующей 
			result = BPC_Execute(ptrname, result, &TypeOfResult);

			while ( (*(mas + p) != '\0') && (*(mas + p) != '|') )
				p++;
			if (*(mas + p) == '\0')
				flag = 2;
			else
				p++;
		}

	}
	return result;
}