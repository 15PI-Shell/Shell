// ConsoleApplication6.cpp: определяет точку входа для консольного приложения.
//

#include <stdio.h>
#include<conio.h>
#include <windows.h>
#define enter '\r'
#define tab 9
#define up 72
#define down 80
#define left 75
#define right 77
#define arrows 224
#define del 83
#define BackSpase 8
#define Space 32

using namespace std;
int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cor = { 0,0 }, o = { 0 , 0 };
	char Buff[100], tmp[100];
	char *pCom;
	int i;
	for ( i = 0; i < 100; i++)
	{
		Buff[i] = '\0';
	}
	int cur = 0, key, next, pre;
	while (1)
	{
		key = getch();
		switch (key)
		{
		case arrows: key = getch();
			switch (key)
			{
			case left: cor.X = cor.X - 1; SetConsoleCursorPosition(hConsole, cor); cur--;break;
			case right:if (Buff[cur-1] != '\0')
			{
				cor.X = cor.X + 1;
				SetConsoleCursorPosition(hConsole, cor); cur++;
			} break;
			case up:   
				break;
			case down: 
				break;
			case del:  SetConsoleCursorPosition(hConsole, cor);
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
		case enter: 
			cor.X = 0; cor.Y = cor.Y + 1; SetConsoleCursorPosition(hConsole, cor); cur=0; 
			for (int i = 0; i < 100; i++)
		{
			Buff[i] = '\0';
			}break;
		case BackSpase: 
			for (int i = cur-1 ; i < strlen(Buff) - 1; i++)
			{
				
					Buff[i] = Buff[i + 1];
			}
			if (strlen(Buff) > 1) Buff[strlen(Buff) - 1] = ' ';
			o.Y = cor.Y;
			SetConsoleCursorPosition(hConsole, o);
			printf("%s", Buff);
				SetConsoleCursorPosition(hConsole, cor);
			break;
		case tab: //автодополнение
			break;
		default:if ((((key >= 'a' && key <= 'z') || (key >= 'A' || key <= 'Z')) || (key >= '0' || key <= '9'))||(key==Space))
		{
			SetConsoleCursorPosition(hConsole, cor);cor.X = cor.X + 1;
			printf("%c", key); Buff[cur] = (char)key; cur++; 

		}
				break;
		}
	}
}
	