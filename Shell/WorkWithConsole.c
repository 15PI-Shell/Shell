﻿#include "WorkWithConsole.h"

void DeleteListOfAutocomletion();
SingleLinklistNode *LastFoundList = 0;
char *Buff;
HANDLE hConsole;
COORD cor, startPrintPoint;
int cur;
int FlagAutocompletions = 0;
int DoubleTabFlag = 0;
/*-----------------------------------------Функции работы с командной строкой---------------------------------------------------------------*/
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
/*----------------------------------------Функции работы с курсором------------------------------------*/
void GetConsoleCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cor = inf.dwCursorPosition;
}// пока что не нужна, понадобится когда заработает интерпретатор


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
void ResetCursor()
{
	cor.X = startPrintPoint.X;
	SetConsoleCursorPosition(hConsole, cor);
}
void CursorOnEndString()
{
	int dx = strlen(Buff) - cur;
	cor.X += dx;
	cur = strlen(Buff);
	SetConsoleCursorPosition(hConsole, cor);
}
void OnNextLine()
{
	GetConsoleCursorPosition();
	cor.Y++;
	cor.X = 0;
	SetConsoleCursorPosition(hConsole, cor);
	printf("%s>", CurrentDirectory);
	GetConsoleCursorPosition();
	startPrintPoint = cor;
	ResetCursor();
	cur = 0;
}
void ConsolePrintChar(int key)
{
	DoubleTabFlag = 0;
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
/*------------------------------------------Инициализация консоли-----------------------------------------*/
void ConsoleInitialisation()
{
	CurrentDirectory = (char*)malloc(MAX_PATH + 1);
	GetCurrentDirectoryA(MAX_PATH, CurrentDirectory);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("%s>", CurrentDirectory);
	GetConsoleCursorPosition();
	startPrintPoint = cor;
	CurrHist = 0;
	DoubleLinklistAddUpmost(&CurrHist, "", 1);//добавляем "ничто" в историю
	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//поправка на перетаскивание символов backspace'ом
	ReprintConsoleBuffer();
}

/*---------------------------------------Обработка специальных клавиш------------------------------------*/

void ConsoleCursorMoveLeft()
{
	DoubleTabFlag = 0;
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
	GetConsoleCursorPosition();
}

void ConsoleGetNextHistory()
{
	DoubleTabFlag = 0;
	DeleteListOfAutocomletion();
	if (CurrHist->down != NULL)
	{
		CurrHist = CurrHist->down;
		strcpy(Buff, CurrHist->value);
		ResetCursor();
		cur = 0;
		ReprintConsoleBuffer();
	}
}

void ConsoleGetPrewHistory()
{
	DoubleTabFlag = 0;
	DeleteListOfAutocomletion();
	if (CurrHist->up != NULL)
	{
		CurrHist = CurrHist->up;
		strcpy(Buff, CurrHist->value);
		ResetCursor();
		cur = 0;
		ReprintConsoleBuffer();
	}
}

void ConsoleDeleteCurElem()
{
	DoubleTabFlag = 0;
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
	DoubleTabFlag = 0;
	if (cur > 0)
	{
		int len = strlen(Buff);
		for (int i = cur - 1; i < len; i++)
			Buff[i] = Buff[i + 1];
		DecCursor();
		ReprintConsoleBuffer();
	}
}

void ConsoleEnter()
{
	DoubleTabFlag = 0;
	DeleteListOfAutocomletion();
	while (CurrHist->down)//сбрасываем указатель истории, всегда сидим в самом низу
		CurrHist = CurrHist->down;
	if ((CurrHist->up == NULL) || ((CurrHist->up != 0) && (strcmp(CurrHist->up->value, Buff))))
	{
		DoubleLinklistInsertAbove(CurrHist, Buff, strlen(Buff));
	}
	CursorOnEndString();
	printf("\n");
	CmdInterpretator(Buff);
	OnNextLine();
	memset(Buff, 0, MAX_CONSOLE_INPUT);
}

void PastInConsole()
{
	char *str = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	GetClipboardContent(str);
	int lenB = strlen(Buff), lenS = strlen(str);
	if (lenB + lenS < MAX_CONSOLE_INPUT)
	{
		if ((cur == lenB - 1) || (lenB == 0)) strcat(Buff, str);
		else
		{
			char *EndBuff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
			int i = 0;  int j = 0;
			for (i = 0; i <= lenB - cur; i++)
			{
				EndBuff[i] = Buff[i + cur];
			}
			for (i = lenB; i > cur; i--)
			{
				Buff[i] = '\0';
			}
			strcat(Buff, str);
			strcat(Buff, EndBuff);
			GetClipboardContent(str);
			free(EndBuff);
		}

		ReprintConsoleBuffer();
		free(str);
	}
	return;

}
/*-------------------------------------------Autocompletion-----------------------------------------------*/
int DetermineEntry(char *entry, int *PosEntryStart) {
	int Buflen = strlen(Buff);
	for (int i = Buflen - 1; i > 0; i--)
	{
		if (Buff[i] == ' ')
		{
			if (i != 0)
			{
				*PosEntryStart = i + 1; int k = 0;
				for (int j = i + 1; j < Buflen; j++)
				{

					entry[k] = Buff[j]; k++;
				} printf("\n%s", entry); return 2;

			}
			else return 0;
		}
	}
	strcpy(entry, Buff); *PosEntryStart = 0; return 1;
}

void ConsoleAutocompletion()
{
	COORD pos = cor;
	int crn = cur;
	CursorOnEndString();
	if (DoubleTabFlag) {
		PrintListOfAutocompletion();
		DoubleTabFlag = 0;
		return;
	}
	DeleteListOfAutocomletion();
	while (LastFoundList != 0)
	{
		SingleLinklistRemoveDownmost(&LastFoundList);
	}
	int Buflen = strlen(Buff);
	char *entry;
	entry = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(entry, 0, MAX_CONSOLE_INPUT + 2);
	int list, EnLen, posEntry;
	SingleLinklistNode *LastFoundFile = NULL, *LastFoundCommand = NULL;
	list = DetermineEntry(entry, &posEntry);
	EnLen = strlen(entry);
	if (0 != *entry)
		switch (list)
		{
		case 1: LastFoundCommand = BPC_GetHints(entry);
			LastFoundFile = FindFilesAndDirsPrefix(entry);
			LastFoundList = LastFoundCommand;
			break;
		case 2:	LastFoundFile = FindFilesAndDirsPrefix(entry);
			break;
		default: return;
		}
	SingleLinklistConcat(LastFoundFile, &LastFoundList);
	if (LastFoundList == NULL) {
		cur = crn; cor = pos;
		SetConsoleCursorPosition(hConsole, pos);
		return;
	} // дополнения не найдены
	if (LastFoundList->up == 0) {
		int len = strlen(LastFoundList->value);
		if (len > MAX_CONSOLE_INPUT - posEntry) {
			cur = crn; cor = pos;
			SetConsoleCursorPosition(hConsole, pos);
			DoubleTabFlag = 1;	return;
		}
		int k = 0;
		for (int i = posEntry; i < len + posEntry; i++)
		{
			Buff[i] = ((char*)LastFoundList->value)[k];
			k++;
		}

		CursorOnEndString();
		ConsolePrintChar(' ');
	} //дополнение единственное, печатаем
	else {
		FlagAutocompletions = 1;
		cur = crn; cor = pos;
		SetConsoleCursorPosition(hConsole, cor);
	}
	DoubleTabFlag = 1;
	free(entry);
	return;
}

void  PrintListOfAutocompletion()
{
	if (FlagAutocompletions)
	{
		FlagAutocompletions = 3;
		printf("\n\n\n\n____________________________________________________");
		while (LastFoundList != NULL) {
			printf("\n%s", LastFoundList->value);
			FlagAutocompletions++;
			LastFoundList = LastFoundList->up;
		}
		SetConsoleCursorPosition(hConsole, startPrintPoint); cor = startPrintPoint; cur = 0;
	}
}
void DeleteListOfAutocomletion()
{
	if (FlagAutocompletions)
	{
		COORD posPrint = startPrintPoint, posCor = cor;
		char*tmp = (char*)malloc(MAX_CONSOLE_INPUT + 2);
		strcpy(tmp, Buff);

		for (int i = 0; i < FlagAutocompletions; i++)
		{
			startPrintPoint.Y++;
			ClearComline();
		}
		startPrintPoint = posPrint;
		cor = posCor;
		strcpy(Buff, tmp);
		SetConsoleCursorPosition(hConsole, cor);
		FlagAutocompletions = 0;
		free(tmp);
	}
	return;
}