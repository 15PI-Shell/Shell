#include "ParserOfArguments.h"


void ParsOfArgs(char* str)
{
	SingleListStringNode* args = NULL;
	int flag = 0;
	char* sp;
	int len = strlen(str);
	char*str1;
	str1 = str;
	int cnt = 0;
	int cnt_spc = 0; //���������� ��� �������� ��������

	for (int i = 0; i < len; i++, str1++) //�������� �� ���������� �������
	{
		if (*str1 == '\"')
		{
			cnt++;
		}
	}
	if (cnt % 2 != 0) //������� - ������
	{
		printf("The string is incorrect");
		return;
	}

	for (int i = 0; i<len; str++, i++)
	{
		if (*str != ' ')	      //���� �� ������, �� ���������, �������� �� ����. ������ ��������
		{
			if (*str != '\"')
			{
				sp = strtok(str, " \""); //���� ������ ������ �� �������
			}							//��������� ����� ������� ��������� ����������
			else
			{
				sp = strtok(str, "\"");	//�������� ��� ���������� ��� ���������
			}
			SingleStrlistAddDownmost(&args, sp); //������ � ������ ���������
			len -= strlen(sp) + cnt_spc;		//����� ������ ������������� ������, ������� ���������� ���������� ������
			str += strlen(sp);					//������� ����� �����, ������� ��� ������� � ������
			cnt_spc = 0;
		}
		else cnt_spc++; //����� ������� ���������� ��������
	}
}
