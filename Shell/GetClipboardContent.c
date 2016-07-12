#include "GetClipboardCont.h"
#include "WorkWithConsole.h"

void GetClipboardContent(char* InpStr)
{
	if (OpenClipboard(NULL)) //открываем буфер обмена
	{
		if (!IsClipboardFormatAvailable(CF_TEXT)) return; //если буфер обмена содержит не текст, то выходим
		HANDLE hClipMemory = GetClipboardData(CF_TEXT); //извлечение текста из буфера обмена
		if (hClipMemory == NULL) //если там пусто, то возвращаемся
		{
			CloseClipboard();
			return;
		}
		char* str = (char*)malloc(GlobalSize(hClipMemory));//строка, в которую вставляем наш текст
		char* pClipMemory = (char*)GlobalLock(hClipMemory);//блокируем память
		strcpy(str, pClipMemory);
		if (str <= MAX_CONSOLE_INPUT)
		{
			strcpy(InpStr, str);
			GlobalUnlock(hClipMemory);//разблокируем память
			CloseClipboard();
		}
		else
		{
			CloseClipboard();
			return;
		}
	}

}