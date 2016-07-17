#include "Shell.h"
#include "ScriptInterpreter.h"

int main()
{
	FILE *f = fopen("script.txt", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;

	BPC_Init();

	EvalScript(string);
	return 0;



	ConsoleInitialisation();
	int key;
	while (1)
	{
		key = getch();
		switch (key)
		{
		case keys_arrows:
			key = getch();
			switch (key)
			{
			case key_left: ConsoleCursorMoveLeft(); break;
			case key_right:ConsoleCursoreMoveRight(); break;
			case key_up: ConsoleGetPrewHistory(); break;
			case key_down:ConsoleGetNextHistory(); break;
			case key_del: ConsoleDeleteCurElem(); break;
			} break;
		case key_enter:ConsoleEnter(); break;
		case key_BackSpase:ConsoleBackSpace(); break;
		case key_tab: ConsoleAutocompletion(); break;
		case keys_ctrl_v: PastInConsole(); break;
		default:ConsolePrintChar(key); break;
		}
	}
	
	return 0;
}
