// ParsArgs.cpp: ���������� ����� ����� ��� ����������� ����������.
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
		if (*str != ' ')	      //���� �� ������, �� ���������, �������� �� ����. ������ ��������
		{
			if (*str == '\"')	//���� ��������, �� ��������� �������� �� ������������� �������
			{
				str1 = str;
				str1++;
				for (int j = i; j < len; j++, str1++)
				{
					if (*str1 == '\"')	//���� ���� �������������, �� ��������� ����
					{
						flag = 1;
						break;
					}
				}
				if (flag != 1)	//����� ������� �� �������
				{
					printf("The string is incorrect");
					break;
				}

			}
			else                //���� �� �������
			{
				if (flag != 1)	//���� ���� ������, �������� �����  
				{
					sp = strtok(str, " ");
				}
				else            //���� ���� ������, �� �������� ���������� ������� � �������� ����
				{
					sp = strtok(str, "\"");
					flag = 0;
				}
				SingleStrlistAddDownmost(&args, sp);	//����� � ������ ���� ���������
				len -= strlen(str);				//��������� ����� ������
				str += strlen(sp);				//������������� ����� �����, ������� ��� � ������
			}
		}
	}
}
