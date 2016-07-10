#include "Shell.h"
#include "FindFile.h"

int main()
{
	char prefix[100];
	char parametrs[100]="";
	int demoMode;
	GetCurrentDirectoryA(sizeof(CurrentDirectory), CurrentDirectory);
	setlocale(LC_ALL, "rus");
	printf("Поиск или открытие файлов:\n");
	printf("Текущая директория: %s\n", CurrentDirectory);
	while (1)
	{
		printf("Запуск(1) / Поиск(2) / Изменить дирректорию(3)? ");
		scanf("%d", &demoMode);
		printf("\n\n");
		switch (demoMode)
		{
		case 2:
			printf("Введите префикс файла: ");
			scanf("%s", prefix);
			SingleListStringNode* list = FindFiles(prefix);//получаем список вариантов
			while (list)
			{//выводим его
				printf("%s\n", list->value);
				list = list->up;
			}
			printf("\n");
			break;
			
		case 1:
			printf("Введите имя файла с указанием расширения (путь указывается по желанию): ");
			scanf("%s", prefix);
			ExecResult result = FileExecute(prefix, parametrs);
			switch (result)
			{
			case ExecResult_Success:
				printf("Success\n");
				break;
			case ExecResult_NotEnoughResources:
				printf("Not Enough Resources\n");
				break;
			case ExecResult_FileNotFound:
				printf("File Not Found\n");
				break;
			case ExecResult_WrongExe:
				printf("The .exe file is invalid\n");
				break;
			case ExecResult_AccessDenied:
				printf("Access Denied\n");
				break;
			case ExecResult_WrongAssociation:
				printf("Wrong Association\n");
				break;
			case ExecResult_UnknownError:
				printf("Unknown Error\n");
				break;
			}
			break;
		case 3:
			printf("Введите путь к новой директории: ");
			scanf("%s", CurrentDirectory);
			break;
		}
		printf("\n\n");
	}
	return 0;
}
