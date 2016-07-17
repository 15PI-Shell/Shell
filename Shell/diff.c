#include "diff.h"

char* diff(char* args)
{
	SingleLinklistNode *ListOfArgs;
	int cnt_args;
	cnt_args = ParsOfArgs(args, &ListOfArgs);
	if (cnt_args != 2)
	{
		while (ListOfArgs)
			SingleLinklistRemoveDownmost(&ListOfArgs);
		printf("List of arguments is wrong\n");
		return -1;
	}
	FILE *fp1 = NULL, *fp2 = NULL;
	char *f1 = (char*)malloc(260);
	char *f2 = (char*)malloc(260);
	strcpy(f2, ListOfArgs->value);
	fp2 = fopen(f2, "r");
	SingleLinklistRemoveDownmost(&ListOfArgs);
	strcpy(f1, ListOfArgs->value);
	SingleLinklistRemoveDownmost(&ListOfArgs);
	fp1 = fopen(f1, "r");
	if ((fp1) && (fp2))
	{
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
	else
	{
		printf("files opening error\n");
			return -1;
	}
}