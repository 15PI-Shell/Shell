#include "echo.h"

char* Echo(char* args)
{
	while (*args)
	{
		if (*args != '"')
			printf("%c", *args);
		args++;
	}
	printf("\n");
	return 0;
}