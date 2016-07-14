#include "MathInterpreter.h"

int ptr = 0, failed = 0;
char* str;

double Term();
double Summand();
double Multipler();
double Const();
double Variable;//�� ���� �� ����� ��������� �����, ������� ���

double Term()//��������� = ��������� [+/- ���������]
{
	if (failed)
		return 0;//���� ���-�� ������, �� ��������� ��������

	double ans;
	ans = Summand();
	if (str[ptr] == ' ')
		ptr++;
	switch (str[ptr])
	{
	case '-'://��� case ����������� �� ���� �����, ������� �����
	case '+':
		ans += Term();
		break;
	case ')':
		ptr++;//���������� )
			  //��������� �����������
		break;
	case '\0':
		break;
	default:
		failed = 1;
		return 0;
	}
	return ans;
}

double Summand()//��������� = ��������� [* ���������]
{
	if (failed)
		return 0;//���� ���-�� ������, �� ��������� ��������

	if (str[ptr] == ' ')
		ptr++;
	int modify = 1;
	switch (str[ptr])
	{
	case '+':
		ptr++;
		break;
	case '-':
		modify = -1;
		ptr++;
		break;
	}//����� ��� ��������� ��� �����

	double ans;
	ans = Multipler();
	if (str[ptr] == ' ')
		ptr++;
	switch (str[ptr])
	{
	case '*':
		ptr++;//���������� *
		ans *= Summand();
		break;
	case '/':
		ptr++;
		double checkAns = Summand();
		if (checkAns)
			ans /= checkAns;
		else
		{
			failed = 1;
			return 0;
		}
	}
	return ans * modify;
}

double Multipler()//��������� = ��������� | ���������
{
	if (failed)
		return 0;//���� ���-�� ������, �� ��������� ��������

	if (str[ptr] == ' ')
		ptr++;
	double ans;
	if (str[ptr] == '(')//��������� � �������
	{
		ptr++;//���������� (
		ans = Term();
	}
	else
		ans = Const();
	return ans;
}

double Const()
{
	if (failed)
		return 0;//���� ���-�� ������, �� ��������� ��������

	if (str[ptr] == ' ')
		ptr++;
	if (str[ptr] < '0' || str[ptr] > '9')
	{
		failed = 1;
		return 0;
	}
	double ans = 0;
	while (str[ptr] >= '0' && str[ptr] <= '9')
	{
		ans *= 10;
		ans += str[ptr] - '0';
		ptr++;
	}
	if (str[ptr] == '.')
	{
		double fract = 0;
		int k = 0;
		ptr++;
		while (str[ptr] >= '0' && str[ptr] <= '9')
		{
			fract *= 10;
			fract += str[ptr] - '0';
			ptr++;
			k++;
		}
		while (k--)
			fract *= 0.1;
		ans += fract;
	}
	return ans;
}

int MathInterpreter(char* expression, double** result)
{
	ptr = failed = 0;
	str = expression;
	
	double* ans = (double*)malloc(sizeof(double));
	*ans = Term();

	if (str[ptr] != 0)//�������� ��� �����-�� �������, �������� ������ ����������� ������
		failed = 1;

	*result = ans;

	if (failed)
		*result = ptr;//�������� ��� �����

	return !failed;
}