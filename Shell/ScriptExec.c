#include "ScriptExec.h"

char* ScriptExec(char* path)
{
	SingleLinklistNode* l = 0;
	if (1 != ParsOfArgs(path, &l))
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

	if (!f)
		return -1;

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);
	free(arg);
	string[fsize] = 0;

	int ret = 0;

	ret = EvalScript(string, v);
	if (ret)
		printf("Script finished\n");
	free(string);

	return ret ? 0 : -1;
}