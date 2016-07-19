#include "tail.h"

char* TailOfFile(char *args)
{
	SingleLinklistNode *ArgumentsList = NULL; // ������� ������ ��� �������� ����������
	char *fileName = (char*)malloc(MAX_PATH + 1);// ������ ��� ��� �����
	strcpy(fileName, "");
	int n = 0;// ������� ����������� �����
	int n_args;
	FILE *fp = NULL;
	n_args = ParsOfArgs(args, &ArgumentsList);
	if (n_args != 2)
	{
		printf("list of arguments is wrong\n");
		while (ArgumentsList)
			SingleLinklistRemoveDownmost(&ArgumentsList);
		return -1;
	}
	sscanf((char*)ArgumentsList->value, "%d", &n);//���������� ���-�� �����
	if (n <= 0)// ���-�� �� ��� � ����������
	{
		printf("Incorrect argument\n");
		return -1;
	}
	SingleLinklistRemoveDownmost(&ArgumentsList);
	if (strlen(ArgumentsList->value) >= MAX_PATH)
	{
		printf("list of arguments is wrong\n");
		SingleLinklistRemoveDownmost(&ArgumentsList);
		return -1;
	}
	strcpy(fileName, (char*)ArgumentsList->value);//������ ��� �����
	SingleLinklistRemoveDownmost(&ArgumentsList);
	char *slesh = NULL;
	slesh = strstr(fileName, "\\");
	if (slesh == NULL)
	{
		int len = strlen(fileName);
		int lenP = strlen(CurrentDirectory);
		if (lenP + len < MAX_PATH + 1)
		{
			char *tmp = (char*)malloc(MAX_PATH + 1);
			strcpy(tmp, CurrentDirectory);
			strcat(tmp, "\\");
			strcat(tmp, fileName);
			strcpy(fileName, tmp);
			free(tmp);
		}
		else
		{
			printf("File's name is wrong\n");
			free(fileName);
			return -1;

		}
	}
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
		char **mas = (char **)malloc(n * sizeof(char *));//�������� ������ ��� ������
		for (int i = 0; i < n; i++)
		{
			mas[i] = (char*)malloc(pos);//�������� ������ ��� ������ � �������������� ��
			memset(mas[i], 0, pos);
		}
		int size = 0;
		int j = 0;
		char c;
		do {
			c = fgetc(fp);//read 
			mas[size%n][j] = c;//��������� ������ � �������������� �������, ���� \n
			j++;
			if (c == '\n')
			{
				size++;
				j = 0;
				memset(mas[size%n], 0, pos);
			}
		} while (c != EOF);
		for (int i = 0; i < n; i++)
		{
			printf("%s", mas[i]);//�������� ������
		}
		for (int i = 0; i < n; i++)// ����������� ������
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