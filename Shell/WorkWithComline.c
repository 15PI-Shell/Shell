#include "WorkWithComline.h"
void ConsoleInitialisation()
{
	CurrentDirectory = (char*)malloc(MAX_PATH);
	GetCurrentDirectoryA(MAX_PATH, CurrentDirectory);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = strlen(CurrentDirectory) + 1;
	cor.Y = 0;
	startPrintPoint.X = strlen(CurrentDirectory) + 1;
	startPrintPoint.Y = 0;
	DoubleStrlistAddUpmost(&CurrHist, "");//добавляем "ничто" в историю
	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//поправка на перетаскивание символов backspace'ом
	printf("%s>", CurrentDirectory);
	ReprintConsoleBuffer();
}
void GetConsoleCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cor = inf.dwCursorPosition;
}// пока что не нужна, понадобится когда заработает интерпретатор
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
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X + startPrintPoint.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X + startPrintPoint.X) % inf.dwSize.X;
}

void DecCursor()
{
	
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cur--;
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X + startPrintPoint.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X + startPrintPoint.X) % inf.dwSize.X;
}

void ResetCur()
{
	cur = 0;
}

void ResetCursor()
{
	cor.X = startPrintPoint.X;
	SetConsoleCursorPosition(hConsole, cor);
}
void CursorOnEndString()
{
	while (Buff[cur])
	{
		IncCursor();
	}
}
void OnNextLine()
{
	GetConsoleCursorPosition();
	cor.Y++;
	cor.X = 0;
	startPrintPoint.Y = cor.Y;
	SetConsoleCursorPosition(hConsole, cor);
	printf("%s>", CurrentDirectory);
	ResetCursor();
	ResetCur();
}

void ConsolePrintChar(int key)
{
	int buffLen = strlen(Buff);
	if (buffLen < MAX_CONSOLE_INPUT && isprint(key) && !(key >= 'а' && key <= 'я') && !(key >= 'А' && key <= 'Я'))
	{
		//если символ печатаем и не кириллица (ну её)
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

void ConsoleGetNextHistory(int flag)
{
	if (CurrHist->down != NULL)
	{
		CurrHist = CurrHist->down;
		strcpy(Buff, CurrHist->value);
		ResetCursor();
		ResetCur();
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(&flag);
}

void ConsoleGetPrewHistory(int flag)
{
	if (CurrHist->up != NULL)
	{
		CurrHist = CurrHist->up;
		strcpy(Buff, CurrHist->value);
		ResetCursor();
		ResetCur(cur);
		ReprintConsoleBuffer();
	}
	ChekFFlagOfAutoComplition(&flag);
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

void ConsoleEnter(int flagOfAutocomplition)
{
	ChekFFlagOfAutoComplition(&flagOfAutocomplition);
	CursorOnEndString();
	while (CurrHist->down)//сбрасываем указатель истории, всегда сидим в самом низу
		CurrHist = CurrHist->down;
	DoubleStrlistInsertAbove(CurrHist, Buff);
	OnNextLine();
	memset(Buff, 0, MAX_CONSOLE_INPUT);

	//printf("%s>", CurrentDirectory);
	//startPrintPoint.X = strlen(CurrentDirectory) + 1;
	
}

/*-Autocompletion-*/

void ConsoleAutocomplition(int flag, SingleListStringNode **LastFoundList)
{
	ChekFFlagOfAutoComplition(&flag);
	int Buflen = strlen(Buff);
	char *entry;
	entry = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(entry, 0, MAX_CONSOLE_INPUT + 2);
	int list, EnLen, posEntry;
	SingleListStringNode *LastFoundFile = NULL, *LastFoundCommand = NULL;
	*LastFoundList = NULL;
	list = DetermineEntry(Buff, Buflen, &entry, &posEntry);//DetemineEntry(Buff, Buflen, entry, &posEntry);
	EnLen = strlen(entry);
	switch (list)
	{
	case 1: LastFoundCommand = BPC_GetHints(entry);
		LastFoundFile = FindFiles(entry);
		*LastFoundList = LastFoundCommand;
		break;
	case 2:	LastFoundFile = FindFiles(entry); break;
	default: return ;
	}
	SingleStrlistConcat(LastFoundFile, LastFoundList);
	if (*LastFoundList == NULL) return; // дополнения не найдены
	if ((*LastFoundList)->up == 0) {
		int len = strlen((*LastFoundList)->value);int k = 0;
		for (int i = posEntry; i < len+posEntry; i++)
		{
			
			Buff[i] = (*LastFoundList)->value[k];
			k++;
		}
		ReprintConsoleBuffer();
	} //дополнение единственное, печатаем
	return ;
}


int DetermineEntry(char *Buff, int Buflen, char **entry, int *PosEntryStart) {
	for (int i = Buflen - 1; i > 0; i--)
	{
		if (Buff[i] == ' ')
		{
			if (i != 0)
			{
				*PosEntryStart = i + 1; int k=0;
				for (int j = i + 1; j < Buflen; j++)
				{

					(*entry)[k] = Buff[j]; k++;
				} return 2;
				
			}
			else return 0;			
		}
	}
	*entry = Buff; *PosEntryStart = 0; return 1;
}

int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions)
{
	int cnt = 2;
	printf("\n\n____________________________________________________");
	while (ListOfAutocomplitions != NULL) {
		printf("\n%s", ListOfAutocomplitions->value);
		cnt++;
		ListOfAutocomplitions = ListOfAutocomplitions->up;
	}
	while (ListOfAutocomplitions != 0)
	{
		SingleStrlistRemoveDownmost(&LastFoundList);
	}
	//printf("\n\n%d", cnt);
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	return cnt;
}

void DeleteListOfAutocomlition(int n, SingleListStringNode*LastFoundList)
{
	COORD pos = startPrintPoint;
	char*tmp = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	strcpy(tmp, Buff);
	for (int i = 0; i < n; i++)
	{
		startPrintPoint.Y++; 
		ClearComline();
	}
	startPrintPoint = pos; strcpy(Buff, tmp);
	SetConsoleCursorPosition(hConsole, cor);
	return;
}

void ChekFFlagOfAutoComplition(int *flag)
{
	if (*flag)
	{
		DeleteListOfAutocomlition(*flag , LastFoundList);
	}
}
