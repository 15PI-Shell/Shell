#include "Autocompletion.h"

void Autocomplition(char *Buff, int Buflen, int *flag)
{
	char *entry;
	entry= (char*)malloc(MAX_CONSOLE_INPUT + 2);
	int list, EnLen, posEntry, k;
	SingleListStringNode* LastFound = NULL;
	list = DetemineEntry(Buff, Buflen, entry, &posEntry);
	EnLen = strlen(entry);
	switch (list)
	{
	case 1: AutoComplitionOfCommand(entry, EnLen);break;
	case 2:	LastFound = FindFiles(entry);break;
		if (LastFound == NULL) return; // ���������� �� �������
		if (LastFound->up == 0) { 
			*(Buff + posEntry) = LastFound->value; 
			reprintConsoleBuffer();
		} //���������� ������������, ��������
		else //������� ��������� ����������, ���� ���� ����
		{
			k = getch();
			if (k == key_tab)
			{
				flag = PrintListOfAutocomplition(LastFound);
			}
		} return;
	}
}	
int DetermineEntry(char *Buff, int Buflen, char *entry, int *PosEntryStart){
	for (int i = Buflen-1; i >0; i--)
	{
		if (Buff[i]==' ')
		{
			if (i==0)
			{
				return 0;
			}
			for (int j = i+1; j< Buff; j++)
			{
				entry[Buflen - j - 1] = Buff[j];
			}
			return 2;
		}
	}
	entry = Buff; return 1;
}

int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions)
{	
	int cnt = 1;
	printf("\n__________________________________________________________________________________________________");
		while (ListOfAutocomplitions = NULL) {
			printf("\n%s", ListOfAutocomplitions->value);
			cnt++;
			ListOfAutocomplitions->up;
		}	
		SetConsoleCursorPosition(hConsole, startPrintPoint);
		return cnt;
}
void DeleteListOfAutocomlition(int n)
{
	int y =cor.Y;
	for (int i = 0; i < n; i++)
	{
		y++;
		ClearComline();
	}
	startPrintPoint.Y = y - n;
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	return;
}