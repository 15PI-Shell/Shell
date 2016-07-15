#include "tail.h"

char* TailOfFile(char *args)
{
	SingleLinklistNode *ArgumentsList = NULL; // ������� ������ ��� �������� ����������
	char *fileName = (char*)malloc(MAX_PATH + 1);// ������ ��� ��� �����
	int n = 0;// ������� ����������� �����
	FILE *fp = NULL;
	ParsOfArgs(args, &ArgumentsList);
	if (ArgumentsList)// ���� ���� ��������, �� ��� �����
	{
		sscanf((char*)ArgumentsList->value, "%d", n);//���������� ���-�� �����
		if (n <= 0)// ���-�� �� ��� � ����������
		{
			printf("Incorrect argument\n");
			return -1;
		}
		if (ArgumentsList->up)// ���� ���� ��� ���� �������� ����
		{
			ArgumentsList = ArgumentsList->up;
			strcpy(fileName, (char*)ArgumentsList->value);//������ ��� �����
			fp = fopen(fileName, "r");
			if (fp == 0) {
				printf("File reading Error\n");//���� �� ������ ������� ��� ����� ������ ���
				return -1;
			}
			fseek(fp, 0, SEEK_END);
			long pos = ftell(fp);// ���� ����� �����
			if (pos > 0)// ��������� �� �������
			{
				rewind(fp);
				char **mas = (char **)calloc(n, sizeof(char *));//�������� ������ ��� ������
				int size = 0;
				while (!feof(fp))
				{
					fgets(mas[size% n], 255, fp);
					size++;
				}
				for (int i = 0; i < n; i++)
				{
					printf("%s\n", mas[i]);
				}
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
	}
		printf("The number of arguments is not enough\n");
			return  -1;
	
}