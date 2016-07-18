#include "InfoSection.h"

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
	
	char sig[] = " HELL-INSTALLER-INFOSECTION";
	sig[0] = 'S';
	int lsig = strlen(sig);
	
	int s= lsig + RESERVED_40_kb + RESERVED_2_b + NumFiles*(RESERVED_512_b + RESERVED_4_b) + size;
	*Size = s;
	char* mas = (char*)malloc(*Size+1);
	memcpy(mas,sig,lsig);

	char leftByte, rightByte;
	leftByte = (NumFiles) % 256;
	rightByte = (NumFiles) / 256;
	
	int reserve = RESERVED_40_kb;
	int bNumFiles = RESERVED_2_b;
	int skp = lsig + reserve + bNumFiles + 1;
	char *str = struc->msg;
	int lmsg = strlen(str);
	int j = 0;
	int k = 0;
	int i = lsig;
	int of = lsig + lmsg; //длина сигнатуры и привественного сообщения
	int on = RESERVED_32_kb + lsig + 1; 
	strcpy(mas + on,str);
	mas[skp - bNumFiles] = leftByte;
	mas[skp - bNumFiles + 1] = rightByte;
	for (i = skp; i < *Size; i++)
	{
			FILE* file;
			char* FileName = struc->FilePath->value;
			int LenOfFile = strlen(FileName) + 1;				//длина пути файла
			file = fopen(FileName, "rb");
			if (file == NULL) printf("Could not open %s", FileName);
			memcpy(mas + i, FileName, LenOfFile);	//добавляем путь файла в массив
			i += RESERVED_512_b - LenOfFile + 1;

			mas[i] = FindFileData.nFileSizeHigh % 256;		//размер файла
			mas[i + 1] = FindFileData.nFileSizeHigh / 256;
			mas[i + 2] = FindFileData.nFileSizeLow % 256;
			mas[i + 3] = FindFileData.nFileSizeLow / 256;
			i += RESERVED_4_b;

			while (!feof(file))
			{
				i += fread((mas + i), sizeof(char), 4096, file);	//тело файла
			}
			fclose(file);
			struc->FilePath = struc->FilePath->up;
	}
	
	return mas;
}

