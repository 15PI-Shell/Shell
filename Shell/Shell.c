#include "Shell.h"

int main()
{
	BPC_Init();//регистрируем программы в контроллере
	//ДЕМО!
	char prefix[100];
	int demoMode;
	BPC_Returns ret;
	
	printf("Created programs: HelloWorld & HelloAcuion (two names of a one program)\n");
	while (1)
	{
		printf("Execute(1) / Find(2)? ");
		scanf("%d", &demoMode);
		printf("\n\n");
		if (demoMode == 2)
		{
			printf("Enter a prefix: ");
			scanf("%s", prefix);
			SingleListStringNode* list = BPC_GetHints(prefix);//получаем список вариантов
			while (list)
			{//выводим его
				printf("%s ", list->value);
				list = list->up;
			}
			printf("\n");
		}
		else
		{
			printf("Enter a program name: ");
			scanf("%s", prefix);
			if (BPC_Execute(prefix, 0, &ret) == (void*)-2)//выполняем программу. -2 = программа не существует
				printf("Error: A program doesn't exist\n");
		}
		printf("\n\n");
	}
	return 0;
}
