#include "echo.h"

char* Echo(char* args)
{
	if (args == 0)
		return -1;
	while (*args)
	{
		if (*args != '"')
			printf("%c", *args);
		args++;
	}
	printf("\n");
	return 0;
}