#pragma once
#include "FindFile.h"
#include "KeyDefines.h"
#include "WorkWithComline.h"

//void ComplitionOfCommand(char *entry, int EnLen);//����� �������������� � ����� ������ 
int PrintListOfAutocomplition(SingleListStringNode* ListOfAutocomplitions);//���������� ����� ��������� ��������� ��������������, ���������� ���-�� ���������� �����
void DeleteListOfAutocomlition(int n);//���� ������������� �������������� �� ���������, ������� � ������
void ConsoleAutocomplition( int Buflen, int *flag);// �������� ������� ��������������, � ����� �������� ��������� �� ������ ��������������, ���-0, ��-���-�� ���������� ����� 
int  DetermineEntry(char *Buff, int Buflen, char *entry, int *PosEntryStart);// ���������� ������, � �������� �������� ��������������, ���������� 1 - ���� �������������� ��������, 2-�������������� ����� �����, 0-� ������ ������(�������� ���� ������ ����)
void ChekFFlagOfAutoComplition(int *flag);
