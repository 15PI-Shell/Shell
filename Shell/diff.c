#include "diff.h"

char* diff(char* args)
{
	SingleLinklistNode *ListOfArgs=NULL;
	int cnt_args=0;
	cnt_args = ParsOfArgs(args, &ListOfArgs);
	if (cnt_args != 2)
	{
		printf("list of arguments is wrong\n");
		while (ListOfArgs)
			SingleLinklistRemoveDownmost(&ListOfArgs);
		return -1;
	}
	FILE *fp1 = NULL, *fp2 = NULL;
	if (strlen(ListOfArgs->value) > MAX_PATH)
	{
		printf("list of arguments is wrong\n");
		SingleLinklistRemoveDownmost(&ListOfArgs);
		return -1;
	}
	char *f2 = (char*)malloc(MAX_PATH + 1);
	strcpy(f2, ListOfArgs->value);
	char *slesh = NULL;
	slesh = strstr(f2, "\\");
	if (slesh == NULL)
	{
		int len = strlen(f2);
		int lenP = strlen(CurrentDirectory);
		if (lenP + len < MAX_PATH + 1)
		{
			char *tmp = (char*)malloc(MAX_PATH + 1);
			strcpy(tmp, CurrentDirectory);
			strcat(tmp, "\\");
			strcat(tmp, f2);
			strcpy(f2, tmp);
			free(tmp);
		}
		else
		{
			printf("File's name is wrong\n");
			free(f2);
			return -1;

		}
	}
	fp2 = fopen(f2, "r");
	if (fp2 == NULL)
	{
		free(f2);
		printf("files opening error\n");
		return -1;
	}

	SingleLinklistRemoveDownmost(&ListOfArgs);
	if (strlen(ListOfArgs->value) >= MAX_PATH)
	{
		printf("list of arguments is wrong\n");
		SingleLinklistRemoveDownmost(&ListOfArgs);
		return -1;
	}

	char *f1 = (char*)malloc(MAX_PATH + 1);
	strcpy(f1, ListOfArgs->value);
	slesh = NULL;
	slesh = strstr(f1, "\\");
	if (slesh == NULL)
	{
		int len = strlen(f1);
		int lenP = strlen(CurrentDirectory);
		if (lenP + len < MAX_PATH + 1)
		{
			char *tmp = (char*)malloc(MAX_PATH + 1);
			strcpy(tmp, CurrentDirectory);
			strcat(tmp, "\\");
			strcat(tmp, f1);
			strcpy(f1, tmp);
			free(tmp);
		}
		else
		{
			printf("File's name is wrong\n");
			free(f1);
			free(f2);
			fclose(fp2);
			return -1;

		}
	}
	if (strcmp(f1, f2)==0)
	{
		printf("Arguments are wrong\n");
		fclose(fp2);
		free(f2), free(f1);
		
		return -1;
	}
	SingleLinklistRemoveDownmost(&ListOfArgs);
	fp1 = fopen(f1, "r");
	if(fp1==NULL)
	{
		free(f2), free(f1);
		close(fp2);
		printf("files opening error\n");
		return -1;
	}
	
		fseek(fp1, 0, SEEK_END); fseek(fp2, 0, SEEK_END);
		long pos1 = ftell(fp1);// ищем конец файла
		long pos2 = ftell(fp2);// ищем конец файла
		if ((pos1 > 0) && (pos2 > 0))// проверяем на пустоту
		{
			char *str1 = (char*)malloc(pos1);
			char *str2 = (char*)malloc(pos2);
			memset(str1, 0, pos1);
			memset(str2, 0, pos2);
			int i=0, j=0;
			int cmp;
			char c1, c2;
			rewind(fp1);
			rewind(fp2);
			do
			{
				do 
				{
					c1 = fgetc(fp1);
					str1[i] = c1;
					i++;
				} while ((c1 != '\n')&&(c1!=EOF));
				do
				{
					c2 = fgetc(fp2);
					str2[j] = c2;
					j++;
				} while ((c2 != '\n')&&(c2!=EOF));
				cmp = strcmp(str1, str2);
				if (cmp < 0)
				{
					printf("%s < %s\n", f1, f2);
				}
				else
				{
					if (cmp == 0)
					{
						printf("%s = %s\n", f1, f2);
					}
					else
					{
						printf("%s > %s\n", f1, f2);
					}
				}
				i=0;
				j=0;
				memset(str1, 0, pos1);
				memset(str2, 0, pos2);
			} while ((c1 != EOF) || (c2 != EOF));
			free(str1);
			free(str2);
		}
		else
		{
			if (pos1 == 0)
				printf("file %s is empty\n", f1);
			if (pos2 == 0) printf("file %s is empty\n", f2);
		}
		free(f1);
		free(f2);
		fclose(fp1);
		fclose(fp2);
		return 0;
	}
	