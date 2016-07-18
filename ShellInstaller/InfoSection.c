﻿#include "InfoSection.h"

char* InfSect(int* Size, InstallerConfig* struc)
{
	HANDLE hFile;
	WIN32_FIND_DATA FindFileData;
	FILE *fl;
	short int NumFiles = struc->NumOfFiles;
	int size = 0;

	for (int i=0; i<NumFiles; i++)  //получаем размер каждого файла
	{
		char* FileName = struc->FilePath->value;
		hFile = FindFirstFileA(FileName, &FindFileData);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Invalid File Handle \n");
			return 0;
		}
		fl = fopen(FileName, "rb");
		if (fl == NULL) printf("Could not open %s", FileName);
		
		fseek(fl, 0, SEEK_END);		//размер файла
		size += ftell(fl);
		fseek(fl, 0, SEEK_SET);
	}
	
	mas = sig;

	char leftByte, rightByte;
	leftByte = (NumFiles) % 256;
	rightByte = (NumFiles) / 256;
	
	int reserve = RESERVED_40_kb;
	int bNumFiles = RESERVED_2_b;
	int skp = lsign + reserve + bNumFiles;
	char *str = struc->msg;
	int j = 0;
	int k = 0;
	int i = lsign;
	
	for (i; i < Size; i++, mas++)
	{

		if ((RESERVED_32_kb + lsign) <= i < reserve) //приветственное сообщние
		{
			mas[i] = str[j];
			j++;

		}
		if (i == (skp - bNumFiles))  //количество файлов
		{
			*mas = leftByte;
			i++, mas++;
			*mas = rightByte;
		}
		if ((i >= skp) && (k<NumFiles))
		{
			char* FileName = struc->FilePath->value;
			int LenOfFile = strlen(FileName);				//длина пути файла
			memcpy(mas + strlen(mas), FileName, LenOfFile);	//добавляем путь файла в массив
			i += RESERVED_512_b - LenOfFile;

			mas[i] = FindFileData.nFileSizeHigh % 256;		//размер файла
			mas[i + 1] = FindFileData.nFileSizeHigh / 256;
			mas[i + 2] = FindFileData.nFileSizeLow % 256;
			mas[i + 3] = FindFileData.nFileSizeLow / 256;
			i += RESERVED_4_b;

			while (!feof(fl))
			{
				fread((mas + i), sizeof(char), 4096, fl);	//тело файла
				i++;
			}
			k++;
		}
	}
	
	return mas;
}

