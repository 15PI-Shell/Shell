#include "FindFile.h"


STRLIST_NODE* FindFile(char* FileMask)
{
	STRLIST_NODE *last = NULL;
	char way[MAX_PATH];
	char * pathPTR;
	int i = -1, j = -1, k = 0;
	HANDLE HandleFile;
	while (FileMask[++i]) //�������� �� �����. ���� ������ ������ �������, �� ��������� * � ���� �� �����
	{
		if (FileMask[i] == '*'|| FileMask[i] == '?')
		{
			k = 1;
			break;
		}
	}
	if (!k)
		strcat(FileMask, "*");

	GetCurrentDirectoryA(sizeof(way), way); //��������� ������� ���������� 
	strcat(way, "\\"); //������� ���� ��� �������
	strcat(way, FileMask); //��������� ���� �����
	HandleFile = FindFirstFileA(way, &FileData); //�������� �������� ���������� �����. � FileData ������������ ������ �����
	if (HandleFile != INVALID_HANDLE_VALUE) //���� ���������� ���� ������
	{
		//printf("\n%s\n", way);
		do
		{
			if (strcmp(FileData.cFileName, ".") && strcmp(FileData.cFileName, "..")) //���������, ��� ��� �� 2 ����������� �����: . � ..
			{
				//printf("%s\n", FileData.cFileName);
				StrlistAdd(&last, FileData.cFileName); //��������� ��� ����� � ������
			}
		} while (FindNextFileA(HandleFile, &FileData) != 0); //���������� �����
	}
	FindClose(HandleFile);

	pathPTR = getenv("PATH");

	i = -1;
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
			strcat(way, FileMask);
			HandleFile = FindFirstFileA(way, &FileData);
			if (HandleFile != INVALID_HANDLE_VALUE)
			{
				//printf("\n%s\n", way);
				do
				{
					if (strcmp(FileData.cFileName, ".") && strcmp(FileData.cFileName, ".."))
					{
						//printf("%s\n", FileData.cFileName);
						StrlistAdd(&last, FileData.cFileName);
					}
				} while (FindNextFileA(HandleFile, &FileData) != 0);
			}
		}
	}
	FindClose(HandleFile);
	return last;
}