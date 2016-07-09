#include "BuiltinProgController.h"
#include "BuiltinProgInterpretator.h"

char* analisator( char* mas)
{
	int p = 0, lenname = 0, lenarg = 0,flag = 0;
	BPC_RETURNS* TypeOfResult = NULL;
	char* result = NULL;

	// flag=0 - ������ ������; 1 - ���� ���� "|"; 2- �����
	while ((flag == 0) || (flag==1) )
	{
		while (*(mas + p) == ' ')
			p++;

		//������� ����� �������� �������
		lenname = p;
		while (*(mas + lenname) != ' ')
			lenname++;
		lenname = lenname - p;

		//��������� �� ������ � ��������� �������
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
			//������� ����� ������ ����������
			lenarg = p;
			while ((*(mas + lenarg) != '\0') && (*(mas + lenarg) != '|'))
				lenarg++;
			lenarg = lenarg - p;

			// ��������� �� ������ � �����������
			char* ptrarg = (char*)malloc(lenarg * sizeof(char));
			i = 0;
			while (i != lenarg)
			{
				*(ptrarg + i) = mas[p];
				p++;
				i++;
	
			}
			*(ptrarg + lenarg) = '\0';

			result = BPC_Execute(ptrname, ptrarg, TypeOfResult);
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
			// �������� ��������� ���������� ������� � ������ ���������� ��������� 
			result = BPC_Execute(ptrname, result, TypeOfResult);

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