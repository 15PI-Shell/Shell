#include "tail.h"

char* TailOfFile(char *args)
{
	SingleLinklistNode *ArgumentsList = NULL; // создаем список для парсинга аргументов
	char *fileName = (char*)malloc(MAX_PATH + 1);// строка под имя файла
	int n = 0;// счетчик считываемых строк
	int n_args;
	FILE *fp = NULL;
	n_args = ParsOfArgs(args, &ArgumentsList);
	if (n_args != 2)
	{
		printf("list of arguments is wrong\n");
		return -1;
	}
	sscanf((char*)ArgumentsList->value, "%d", n);//определяем кол-во строк
	if (n <= 0)// что-то не так с аргументом
	{
		printf("Incorrect argument\n");
		return -1;
	}
	ArgumentsList = ArgumentsList->up;
	strcpy(fileName, (char*)ArgumentsList->value);//читаем имя файла
	fp = fopen(fileName, "r");
	if (fp == 0) {
		printf("File reading Error\n");//если не удлось открыть или файла просто нет
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	long pos = ftell(fp);// ищем конец файла
	if (pos > 0)// проверяем на пустоту
	{
		rewind(fp);
		char **mas = (char **)calloc(n, sizeof(char *));//выделяем памято под массив
		int size = 0;
		while (!feof(fp))
		{
			fgets(mas[size% n], 255, fp);
			size++;
		}
		for (int i = 0; i < n; i++)
		{
			printf("%s\n", *mas[i]);
		}
		free(mas);
		return 0;
	}
	else
	{
		printf("%s file is empty\n", fileName);
		return 0;
	}
	close(fp);
	free(fileName);
}