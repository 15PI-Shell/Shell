#include "InfoSection.h"

char* InfSect(int* Size, InstallerConfig* struc)
{
	HANDLE hFile;
	WIN32_FIND_DATA FindFileData;

	short int NumFiles = struc->NumOfFiles;
	char sig[] = " HELL-INSTALLER-INFOSECTION";
	sig[0] = 'S';
	int lsig = strlen(sig);

	int s = lsig + RESERVED_40_kb + RESERVED_2_b + NumFiles*(RESERVED_512_b + RESERVED_4_b);
	*Size = s;
	char* mas = (char*)malloc(*Size);
	memcpy(mas, sig, lsig);

	char leftByte, rightByte;
	leftByte = (NumFiles) % 256;
	rightByte = (NumFiles) / 256;

	int reserve = RESERVED_40_kb;
	int bNumFiles = RESERVED_2_b;
	int skp = lsig + reserve + bNumFiles + 1;
	char *str = struc->msg;
	int lmsg = strlen(str);
	int on = RESERVED_32_kb + lsig + 1;
	strcpy(mas + on, str);
	mas[skp - bNumFiles] = leftByte;
	mas[skp - bNumFiles + 1] = rightByte;

	for (int i = skp; i < *Size;)
	{
		char* FileName = struc->FilePath->value;
		int LenOfFile = strlen(FileName) + 1;				//длина пути файла

		memcpy(mas + i, FileName, LenOfFile);	//добавляем путь файла в массив
		i += RESERVED_512_b;

		hFile = FindFirstFileA(FileName, &FindFileData);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("File doesn't exist\n");
			return 0;
		}

		mas[i] = FindFileData.nFileSizeHigh % 256;		//размер файла
		mas[i + 1] = FindFileData.nFileSizeHigh / 256;
		mas[i + 2] = FindFileData.nFileSizeLow % 256;
		mas[i + 3] = FindFileData.nFileSizeLow / 256;
		i += RESERVED_4_b;

		struc->FilePath = struc->FilePath->up;
	}

	return mas;
}
