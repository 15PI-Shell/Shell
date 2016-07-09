// Command history.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdlib.h> 
#include <stdio.h>
#include <conio.h>
#include <windows.h>
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cor = { 0,0 }, o = { 0 , 0 };
typedef struct History
{
	char *Command;
	History *pre;
	History *next;
};
History* InsertCom(char *Com, History * last)
{
	History *newP = NULL;
	newP = (History*)malloc(sizeof(History)); 
	if (newP == NULL) {
		printf("no memory available\n"); return NULL;
	}
	newP->next = NULL; 
	newP->pre = NULL;
	newP->Command = Com;
	if(last!=NULL)
	{
		newP->pre = last;
		last->next = newP;
	}
	last = newP;
	return last;
}
void getCom(History *current, char* Buf, int x, int y)
{
	if (current == NULL) return;
	Buf = current->Command;
	cor.X = x; cor.Y = y;
	SetConsoleCursorPosition(hConsole, cor);
	printf("%s", Buf);
	current->pre;
	return ;

}

int main()
{
	History *last = NULL; History *crnt = NULL;
	char BUFFER[]="hello", Buffer[]="World", *ps;
	int k;
	last = InsertCom(BUFFER, last);
	last = InsertCom(Buffer, last);
	k = getch();
	switch (k)
	{
	case 224: k = getch();
		if(k==72)getCom(last, BUFFER, 12, 7);
	default: break;
	}
	getch();
	return 0;
}

