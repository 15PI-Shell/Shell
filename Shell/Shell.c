#include "Shell.h"

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cor = { 0,0 }, o = { 0 , 0 };
	DoubleListStringNode *CurrHist;
	char Buff[100], tmp[100];
	char *pch;
	for (int i = 0; i < 100; i++)
	{
		Buff[i] = '\0';
	}
	int cur = 0, key;
	while (1)
	{
		key = getch();
		switch (key)
		{
		case keys_arrows: key = getch();
			switch (key)
			{
			case key_left: cor.X = cor.X - 1; SetConsoleCursorPosition(hConsole, cor); cur--;break;
			case key_right:if (Buff[cur - 1] != '\0')
			{
				cor.X = cor.X + 1;
				SetConsoleCursorPosition(hConsole, cor); cur++;
			} break;
			case key_up: if (CurrHist->up != NULL)
			{
				CurrHist = CurrHist->up; *Buff= CurrHist->value;
				cor.X = 0;cur = 0;
				SetConsoleCursorPosition(hConsole, cor);
				printf("%s", Buff);
			}
				break;
			case key_down:if (CurrHist->down != NULL)
			{
				CurrHist = CurrHist->down; *Buff = CurrHist->value;
				cor.X = 0;
				cur = 0;
				SetConsoleCursorPosition(hConsole, cor);
				printf("%s", Buff);
			}
				break;
			case key_del:  SetConsoleCursorPosition(hConsole, cor);
				for (int i = cur; i < strlen(Buff) - 1; i++)
				{
					Buff[i] = Buff[i + 1];
				}
				if (strlen(Buff) > 1) Buff[strlen(Buff) - 1] = ' ';
				o.Y = cor.Y;
				SetConsoleCursorPosition(hConsole, o);
				printf("%s", Buff);
				SetConsoleCursorPosition(hConsole, cor);
			}break;
		case key_enter:
			cor.X = 0; cor.Y = cor.Y + 1; SetConsoleCursorPosition(hConsole, cor); cur = 0;
			DoubleStrlistInsertAbove(CurrHist, Buff);
			for (int i = 0; i < 100; i++)
			{
				Buff[i] = '\0';
			}break;
		case key_BackSpase:
			for (int i = cur - 1; i < strlen(Buff) - 1; i++)
			{

				Buff[i] = Buff[i + 1];
			}
			if (strlen(Buff) > 1) Buff[strlen(Buff) - 1] = ' ';
			o.Y = cor.Y;
			SetConsoleCursorPosition(hConsole, o);
			printf("%s", Buff);
			SetConsoleCursorPosition(hConsole, cor);
			break;
		case key_tab: //автодополнение
			break;
		default:if ((((key >= 'a' && key <= 'z') || (key >= 'A' || key <= 'Z')) || (key >= '0' || key <= '9')) || (key == key_Space))
		{
			SetConsoleCursorPosition(hConsole, cor);cor.X = cor.X + 1;
			printf("%c", key); Buff[cur] = (char)key; cur++;

		}
				break;
		}
	}
	return 0;

}
