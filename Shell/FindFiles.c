#include "FindFile.h"


STRLIST_NODE* FindFiles(char* PrefixFile)
{
	WIN32_FIND_DATA FileData;
	STRLIST_NODE *last = NULL;
	char way[MAX_PATH];
	char * pathPTR;
	int i = -1, j = -1;
	HANDLE HandleFile;

	GetCurrentDirectoryA(sizeof(way), way); //��������� ������� ���������� 
	strcat(way, "\\"); //������� ���� ��� �������
	strcat(way, PrefixFile); //��������� ��� �������
	HandleFile = FindFirstFileA(way, &FileData); //�������� �������� ���������� �����. � FileData ������������ ������ �����
	if (HandleFile != INVALID_HANDLE_VALUE) //���� ���������� ���� ��� ����� �������
	{
		printf("\n%s\n", way);
		do
		{
				printf("%s\n", FileData.cFileName);
				StrlistAdd(&last, FileData.cFileName); //��������� ��� �����/����� � ������
		} while (FindNextFileA(HandleFile, &FileData) != 0); //���������� �����
	}
	FindClose(HandleFile);

	pathPTR = getenv("PATH");

	while (pathPTR[++i]) //������ PATH
	{
		if (pathPTR[i] != ';') //�������� ��� ����, ��������� � PATH � ���� ��������� �����
		{
			way[++j] = pathPTR[i];
		}

		else
		{
			way[j] = 0;
			j = -1;
			strcat(way, "\\");
			strcat(way, PrefixFile);
			strcat(way, "*");
			HandleFile = FindFirstFileA(way, &FileData);
			if (HandleFile != INVALID_HANDLE_VALUE)
			{
				printf("\n%s\n", way);
				do
				{
						printf("%s\n", FileData.cFileName);
						StrlistAdd(&last, FileData.cFileName);
				} while (FindNextFileA(HandleFile, &FileData) != 0);
			}
		}
	}
	FindClose(HandleFile);
	return last;
}