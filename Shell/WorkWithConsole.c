#include "WorkWithConsole.h"

void DeleteListOfAutocomletion();
SingleLinklistNode *LastFoundList = 0;
DoubleLinklistNode *CurrHist;
char *Buff;
HANDLE hConsole;
COORD cor, startPrintPoint;
int cur;
int FlagAutocompletions = 0;
int DoubleTabFlag = 0;
int cnt = 0;
FILE *fpHistory = NULL;
char *HistoryPath;
COORD XYlist;
LPCWSTR History;
/*-----------------------------------------??????? ?????? ? ????????? ???????---------------------------------------------------------------*/
void ClearComline()
{
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	for (int i = 0; i < MAX_CONSOLE_INPUT; ++i)
	{
		printf(" ");
		Buff[i] = '\0';
	}
}

void ReprintConsoleBuffer()
{
	char *tmp = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	strcpy(tmp, Buff);
	ClearComline();
	strcpy(Buff, tmp);
	free(tmp);
	SetConsoleCursorPosition(hConsole, startPrintPoint);
	printf("%s", Buff);
	SetConsoleCursorPosition(hConsole, cor);
}
/*----------------------------------------??????? ?????? ? ????????------------------------------------*/
void GetConsoleCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cor = inf.dwCursorPosition;
}// ???? ??? ?? ?????, ??????????? ????? ?????????? ?????????????


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
	cor = startPrintPoint;
	SetConsoleCursorPosition(hConsole, cor);
}
void CursorOnEndString()
{
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(hConsole, &inf);
	cur = strlen(Buff);
	cor.Y = (cur + startPrintPoint.Y * inf.dwSize.X + startPrintPoint.X) / inf.dwSize.X;
	cor.X = (cur + startPrintPoint.Y * inf.dwSize.X + startPrintPoint.X) % inf.dwSize.X;
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
	if (buffLen < MAX_CONSOLE_INPUT && isprint(key) && !(key >= '�' && key <= '�') && !(key >= '�' && key <= '�'))
	{
		//???? ?????? ???????? ? ?? ????????? (?? ?)
		for (int i = buffLen; i > cur; --i)
			Buff[i] = Buff[i - 1];
		Buff[cur] = (char)key;
		IncCursor();
		ReprintConsoleBuffer();
	}
}
/*------------------------------------------????????????? ???????-----------------------------------------*/
void ReadHistory()
{
	fpHistory = fopen(HistoryPath, "r");
	if (fpHistory != NULL)
	{
		fseek(fpHistory, 0, SEEK_END);
		long pos = ftell(fpHistory);
		if (pos > 0)
		{
			rewind(fpHistory);
			char *str = (char*)malloc(MAX_CONSOLE_INPUT);
			while (fgets(str, MAX_CONSOLE_INPUT, fpHistory))
			{

				str[strlen(str) - 1] = '\0';
				DoubleLinklistInsertAbove(CurrHist, str, strlen(str));
				cnt++;
			}
			free(str);
		}
		fclose(fpHistory);
	}
}
void WriteHistory()
{
	fpHistory = fopen(HistoryPath, "w");
	if (fpHistory == NULL)
	{
		printf("\nSoftware failure Please restart the program\n");
		exit(0);
	}
	while (CurrHist->up)
	{
		CurrHist = CurrHist->up;
	}

	while (CurrHist->down)
	{
		if (strcmp((char*)CurrHist->value, ""))
		{
			fprintf(fpHistory, "%s\n", (char*)CurrHist->value);
		}
		CurrHist = CurrHist->down;
	}
	fclose(fpHistory);
}
void ConsoleInitialisation()
{
	CurrentDirectory = (char*)malloc(MAX_PATH + 1);
	GetCurrentDirectoryA(MAX_PATH, CurrentDirectory);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("%s>", CurrentDirectory);
	GetConsoleCursorPosition();
	startPrintPoint = cor;
	HistoryPath = malloc(MAX_PATH);
	strcpy(HistoryPath, getenv("USERPROFILE"));
	strcat(HistoryPath, "\\Documents\\15PI - SHELL");//???????? ??????????

	CreateDirectoryA(HistoryPath, NULL);
	SetFileAttributesA(HistoryPath, FILE_ATTRIBUTE_HIDDEN);
	strcat(HistoryPath, "\\history.txt");//????????? ??? ????? ????? ????????? ???? ? ????
	DoubleLinklistAddUpmost(&CurrHist, "", 1);//????????? "?????" ? ???????
	ReadHistory();
	Buff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
	memset(Buff, 0, MAX_CONSOLE_INPUT + 2);//???????? ?? ?????????????? ???????? backspace'??
	ReprintConsoleBuffer();
}

/*---------------------------------------????????? ??????????? ??????------------------------------------*/

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
		ClearComline();
		CurrHist = CurrHist->down;
		strcpy(Buff, CurrHist->value);
		ResetCursor();
		cur = 0;
		ReprintConsoleBuffer();
	}
	if (CurrHist->down == NULL)
	{
		ClearComline();
		ResetCursor();

	}
}

void ConsoleGetPrewHistory()
{
	DoubleTabFlag = 0;
	DeleteListOfAutocomletion();
	if (CurrHist->up != NULL)
	{
		ClearComline();
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
	while (CurrHist->down)//?????????? ????????? ???????, ?????? ????? ? ????? ????
		CurrHist = CurrHist->down;
	if ((CurrHist->up == NULL) || ((CurrHist->up != 0) && (strcmp(CurrHist->up->value, Buff))))
	{
		//DoubleLinklistRemoveUpmost(&CurrHist->up);
		cnt++;
		if (cnt > 100)
		{
			DoubleLinklistRemoveUpmost(&CurrHist->up);
			cnt--;
			while (CurrHist->down)//?????????? ????????? ???????, ?????? ????? ? ????? ????
				CurrHist = CurrHist->down;
		}

		DoubleLinklistInsertAbove(CurrHist, Buff, strlen(Buff));
		WriteHistory();
	}
	CursorOnEndString();
	printf("\n");
	free(CmdInterpretator(Buff));
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
			if (cur == 0)
			{
				strcat(str, Buff);
				strcpy(Buff, str);
			}
			else
			{
				char *EndBuff = (char*)malloc(MAX_CONSOLE_INPUT + 2);
				int i = 0;  int j = 0;
				for (i = 0; i <= lenB - cur; i++)
				{
					EndBuff[i] = Buff[i + cur + 1];
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
				}
				int eLen = strlen(entry);
				for (int i = 0; i < eLen; i++)
				{
					if (((entry[i] == '*') || (entry[i] == '?')) || (entry[i] == '\\'))
					{
						return 0;
					}
				}
				return 2;
			}
			else return 0;
		}
		if ((Buff[i] == '&') || (Buff[i] == '|') || (Buff[i] == '>'))
		{
			*PosEntryStart = i + 1; int k = 0;
			for (int j = i + 1; j < Buflen; j++)
			{

				entry[k] = Buff[j]; k++;
			}
			int eLen = strlen(entry);
			for (int i = 0; i < eLen; i++)
			{
				if (((entry[i] == '*') || (entry[i] == '?') || (entry[i] == '\\')))
				{
					return 0;
				}
			}
			return 1;

		}

	}
	strcpy(entry, Buff);
	int eLen = strlen(entry);
	*PosEntryStart = 0;
	for (int i = 0; i < eLen; i++)
	{
		if (((entry[i] == '*') || (entry[i] == '?')) || (entry[i] == '\\'))
		{
			return 0;
		}
	}
	return 1;
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
	} // ?????????? ?? ???????
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
	} //?????????? ????????????, ????????
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

		XYlist.X = 0;
		XYlist.Y = startPrintPoint.Y + 5;
		FlagAutocompletions = 1;
		SetConsoleCursorPosition(hConsole, XYlist);
		printf("____________________________________________________");
		while (LastFoundList != NULL) {
			printf("\n%s", (char*)LastFoundList->value);
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
		startPrintPoint = XYlist;
		for (int i = 0; i < FlagAutocompletions; i++)
		{
			ClearComline();
			startPrintPoint.Y++;
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


