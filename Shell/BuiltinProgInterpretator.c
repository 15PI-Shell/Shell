#include "BuiltinProgController.h"
#include "BuiltinProgInterpretator.h"

char* analisator()
{
	char mas[] ="Hellow a b c";
	int p = 0, lenname=0,i=0,lenarg=sizeof(mas);

	printf("*\n");
	getch();
	while ((mas + p) == ' ')
		p++;

	lenname = p;

	while ((mas + lenname) != ' ')
		lenname++;
	
	lenname = lenname - p;
	char* ptrname = NULL;
	ptrname = (char*)malloc(lenname * sizeof(char));

	return ptrname;
	/*
	*(ptrname + lenname) = '\0';
	p++;
	while (*(ptrname + p) != '\0')
	{
		*(ptrname + p) = mas[p];
		p++;
	}
*/
	
}