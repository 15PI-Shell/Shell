#include "WorkWithComline.h"

void ConsoleInitialisation()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = cor.Y = 0;
	startPrintPoint.X = startPrintPoint.Y = 0;
	DoubleStrlistAddUpmost(&CurrHist, "");//��������� "�����" � �������
	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//�������� �� �������������� �������� backspace'��
}

void ClearComline()
{
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	for (int i = 0; i < MAX_CONSOLE_INPUT; ++i)
		printf(" ");
}

void ReprintConsoleBuffer()
{
	ClearComline();
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	printf("%s", Buff);
	SetConsoleCursorPosition(hConsole, cor);
}

void IncCursor()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cur++;
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X) % inf.dwSize.X;
}

void DecCursor()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cur--;
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X) % inf.dwSize.X;
}

void ResetCur()
{
	cur = 0;
}

void AddNewCommandInHistory(char *str, DoubleListStringNode *CurHist)
{

}

void OnNextLine()
{
	cor.X = 0;
	cor.Y++;
	startPrintPoint.Y = cor.Y;
	SetConsoleCursorPosition(hConsole, cor);
	ResetCur();
	memset(Buff, 0, MAX_CONSOLE_INPUT);
}

void ConsolePrintChar(int key)
{
	int buffLen = strlen(Buff);
	if (buffLen < MAX_CONSOLE_INPUT && isprint(key) && !(key >= '�' && key <= '�') && !(key >= '�' && key <= '�'))
	{
		//���� ������ �������� � �� ��������� (�� �)
		for (int i = buffLen; i > cur; --i)
			Buff[i] = Buff[i - 1];
		Buff[cur] = (char)key;
		IncCursor();
		ReprintConsoleBuffer();
	}
}

/*-----------Keys------------------------------*/

void ConsoleCursorMoveLeft()
{
	if (cur > 0)
	{
		DecCursor();
		SetConsoleCursorPosition(hConsole, cor);
	}
}

void ConsoleCursoreMoveRight()
{
	if (Buff[cur])
	{
		IncCursor();
		SetConsoleCursorPosition(hConsole, cor);
	}
}

void ConsoleGetNextHistory(int *flagOfAutocomplition)
{
	if (CurrHist->down != NULL)
	{
		CurrHist = CurrHist->down;
		Buff = CurrHist->value;
		cor.X = 0;
		ResetCur();
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
}

void ConsoleGetPrewHistory(int *flagOfAutocomplition)
{
	if (CurrHist->up != NULL)
	{
		CurrHist = CurrHist->up;
		Buff = CurrHist->value;
		cor.X = 0;
		ResetCur(cur);
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
}

void ConsoleDeleteCurElem()
{
	if (Buff[cur])
	{
		int len = strlen(Buff);
		for (int i = cur; i <= len; i++)
			Buff[i] = Buff[i + 1];
		ReprintConsoleBuffer();
	}
}

void ConsoleBackSpace()
{
	if (cur > 0)
	{
		int len = strlen(Buff);
		for (int i = cur - 1; i < len; i++)
			Buff[i] = Buff[i + 1];
		DecCursor();
		ReprintConsoleBuffer();
	}
}

void ConsoleEnter(int *flagOfAutocomplition)
{
	char *str;
	str = (char*)malloc(MAX_CONSOLE_INPUT + 2); memset(str, 0, MAX_CONSOLE_INPUT + 2);
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
	DoubleStrlistAddDownmost(&CurrHist, str);
	while (CurrHist->down)//���������� ��������� �������, ������ ����� � ����� ����
		CurrHist = CurrHist->down;
	strcmp(CurrHist->value, Buff);
	OnNextLine();

}

/*-Autocompletion-*/

void ConsoleAutocomplition(int *flagOfAutocomplitionList)
{
	int Buflen = strlen(Buff);
	char *entry;
	entry = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	int list, EnLen, posEntry, k;
	SingleListStringNode* LastFound = NULL;
	list = DetermineEntry(Buff, Buflen, entry, &posEntry);//DetemineEntry(Buff, Buflen, entry, &posEntry);
	EnLen = strlen(entry);
	switch (list)
	{
	case 1: //AutoComplitionOfCommand(entry, EnLen);
		break;
	case 2:	LastFound = FindFiles(entry); break;
		if (LastFound == NULL) return; // ���������� �� �������
		if (LastFound->up == 0) {
			*(Buff + posEntry) = LastFound->value;
			ReprintConsoleBuffer();
		} //���������� ������������, ��������
		else //������� ��������� ����������, ���� ���� ����
		{
			k = getch();
			if (k == key_tab)
			{
				*flagOfAutocomplitionList = PrintListOfAutocomplition(LastFound);
			}
		} return;
	}
}

int DetermineEntry(char *Buff, int Buflen, char *entry, int *PosEntryStart) {
	for (int i = Buflen - 1; i > 0; i--)
	{
		if (Buff[i] == ' ')
		{
			if (i == 0)
			{
				return 0;
			}
			for (int j = i + 1; j < Buff; j++)
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
	int y = cor.Y;
	for (int i = 0; i < n; i++)
	{
		y++;
		ClearComline();
	}
	startPrintPoint.Y = y - n;
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	return;
}

void ChekFFlagOfAutoComplition(int *flag)
{
	if (*flag != 0)
	{
		DeleteListOfAutocomlition(*flag);
		*flag = 0;
	}
}