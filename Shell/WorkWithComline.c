﻿#include "WorkWithComline.h"

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

void ConsoleGetNextHistory(int *flagOfAutocomplition)
{
	if (CurrHist->down != NULL)
	{
		CurrHist = CurrHist->down;
		strcpy(Buff, CurrHist->value);
		ResetCursor();
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
		strcpy(Buff, CurrHist->value);
		ResetCursor();
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
	ChekFFlagOfAutoComplition(flagOfAutocomplition);
	while (CurrHist->down)//сбрасываем указатель истории, всегда сидим в самом низу
		CurrHist = CurrHist->down;
	DoubleStrlistInsertAbove(CurrHist, Buff);
	OnNextLine();
	startPrintPoint.X = strlen(CurrentDirectory) + 1;
	printf("%s>", CurrentDirectory);
}

/*-Autocompletion-*/

void ConsoleAutocomplition(int *flagOfAutocomplitionList, SingleListStringNode **LastFoundList)
{
	int Buflen = strlen(Buff);
	char *entry;
	entry = (char*)malloc(MAX_CONSOLE_INPUT + 2);memset(entry, 0, MAX_CONSOLE_INPUT + 2);
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
		SingleStrlistConcat(LastFoundFile, *LastFoundList);
		//слияние
		break;
	case 2:	LastFoundFile = FindFiles(entry); *LastFoundList = LastFoundFile;break;
	default: return;
	}
	if (*LastFoundList == NULL) return; // дополнения не найдены
	if ((*LastFoundList)->up == 0) {
		int len = strlen((*LastFoundList)->value);
		for (int i = posEntry; i < len; i++)
		{
			Buff[i] = (*LastFoundList)->value[i - posEntry];
		}
		ReprintConsoleBuffer();
	} //дополнение единственное, печатаем
	return;
}


int DetermineEntry(char *Buff, int Buflen, char **entry, int *PosEntryStart) {
	for (int i = Buflen - 1; i > 0; i--)
	{
		if (Buff[i] == ' ')
		{
			if (i == 0)
			{
				return 0;
			}
			for (int j = i + 1; j < Buflen; j++)
			{
				*(entry)[Buflen - j - 2] = Buff[j]; 
			}
			*PosEntryStart = i + 1;
			return 2;
		}
	}
	*entry = Buff; *PosEntryStart = 0; return 1;
}

int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions)
{
	int cnt = 1;
	printf("\n____________________________________________________");
	while (ListOfAutocomplitions != NULL) {
		printf("\n%s", ListOfAutocomplitions->value);
		cnt++;
		ListOfAutocomplitions = ListOfAutocomplitions->up;
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