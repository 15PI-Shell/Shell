#include "SearchLogicalSymbol.h"

char* SLSProc(char* arg, int* s, int* flag)
{
	int f = *s;
	if (*s == strlen(arg)) return 0;
	while ((arg[f] != '&') && (arg[f] != '|') && (strlen(arg) > f))
		f++;

	if ((f == strlen(arg)) && (*flag == 2)) return 0; // not || && 
	
	char *str = (char*)malloc(sizeof(char)*(f - *s + 1));

	if ((arg[f] == '&') || (*flag == 0))
	{
		*flag = 0;
		strncpy(str, arg + (*s), f - (*s)); 
		/*int i = 0;
		while ((i + *s) != f)
		{
			str[i] = arg[i + *s];
			i++;
		}*/
		str[f - *s] = '\0'; 
	}

	if ((arg[f] == '|') || (*flag == 1))
	{
		*flag = 1;
		strncpy(str, arg + *s, f - *s);
		/*int i = 0;
		while ((i + *s) != f)
		{
			str[i] = arg[i + *s];
			i++;
		}*/
		str[f - *s] = '\0';
	}
	if ((arg[f] == '&') || (arg[f] == '|'))
		f += 2;

	*s = f;
	return str;
}