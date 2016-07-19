#include "ScriptExec.h"

char* ScriptExec(char* path)
{
	SingleLinklistNode* l = 0;
	if (!ParsOfArgs(path, &l))
		return -1;

	int v = 0;
	if (l->up && !strcmp(l->value, "-v"))
		v = 1;

	char* arg = malloc(300);
	strcpy(arg, CurrentDirectory);
	strcat(arg, "\\");
	if (l->up)
		strcat(arg, l->up->value);
	else
		strcat(arg, l->value);
	FILE *f = fopen(arg, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);
	free(arg);
	string[fsize] = 0;

	if (EvalScript(string, v))
		printf("Script finished");
	free(string);

	return 0;
}