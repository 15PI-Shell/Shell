#pragma once
#include "FindFile.h"
#include "BuiltinProgController.h"
#include "KeyDefines.h"
#include "WorkWithComline.h"

void ComplitionOfCommand(char *entry, int EnLen);//����� �������������� � ����� ������ 
int PrintListOfAutocomplition(SingleListStringNode*);//���������� ����� ��������� ��������� ��������������, ���������� ���-�� ���������� �����
void DeleteListOfAutocomlition(int n);//���� ������������� �������������� �� ���������, ������� � ������
void Autocomplition(char *Buff, int Buflen, int *flag);// �������� ������� ��������������, � ����� �������� ��������� �� ������ ��������������, ���-0, ��-���-�� ���������� ����� 
int DetemineEntry(char *Buff, int Buflen, char *entry,int *PosEntryStart);// ���������� ������, � �������� �������� ��������������, ���������� 1 - ���� �������������� ��������, 2-�������������� ����� �����, 0-� ������ ������(�������� ���� ������ ����)

