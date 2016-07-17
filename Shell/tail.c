#include "tail.h"

char* TailOfFile(char *args)
{
	SingleLinklistNode *ArgumentsList = NULL; // создаем список дл€ парсинга аргументов
	char *fileName = (char*)malloc(MAX_PATH + 1);// строка под им€ файла
	strcpy(fileName, "");
	int n = 0;// счетчик считываемых строк
	int n_args;
	int spaces;
	FILE *fp = NULL;
	n_args = ParsOfArgs(args, &ArgumentsList);
	if (n_args != 2)
	{
		printf("list of arguments is wrong\n");
		while(ArgumentsList)
			SingleLinklistRemoveDownmost(&ArgumentsList);
		return -1;
	}
	sscanf((char*)ArgumentsList->value, "%d", &n);//определ€ем кол-во строк
	if (n <= 0)// что-то не так с аргументом
	{
		printf("Incorrect argument\n");
		return -1;
	}
	SingleLinklistRemoveDownmost(&ArgumentsList);
	strcpy(fileName, (char*)ArgumentsList->value);//читаем им€ файла
	SingleLinklistRemoveDownmost(&ArgumentsList);
	fp = fopen(fileName, "r");
	if (fp == 0) {
		printf("File reading Error\n");//если не удлось открыть или файла просто нет
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	long pos = ftell(fp);// ищем конец файла
	if (pos > 0)// провер€ем на пустоту
	{
		rewind(fp);
		char **mas = (char **)malloc(n*sizeof(char *));//выдел€ем пам€ть под массив
		for (int i = 0; i < n; i++)
		{
			mas[i]=(char*)malloc(pos);//выдел€ем пам€ть под строки и инициализируем их
			memset(mas[i], 0, pos);
		}
		int size = 0;
		int j = 0;
		char c;
		do {
			c = fgetc(fp);//read 
			mas[size%n][j]=c;//заполн€ем строку в закольцованном массиве, пока \n
			j++;
			if (c=='\n')
			{
				size++;
				j = 0;
				memset(mas[size%n], 0, pos);
			}
		} while (c!= EOF);
		for (int i = 0; i < n; i++)
		{
			printf("%s", mas[i]);//печатаем строки
		}
		for (int i = 0; i < n; i++)// освобождаем пам€ть
		{
			free(mas[i]);
		}
		free(mas);
		free(fileName);
		fclose(fp);
		return 0;
	}
	else
	{
		printf("%s file is empty\n", fileName);
		free(fileName);
		fclose(fp);
		return 0;
	}
}