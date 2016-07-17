#include "InfoSection.c.h"

char* InfSect(int* Size, InstallerConfig* struc)
{

	char* mas;
	char bytes[] = { 0x53, 0x48, 0x45, 0x4c, 0x4c, 0x2d, 0x49,
		0x4e, 0x53, 0x54,0x41, 0x4c, 0x4c, 0x45,
		0x52, 0x2d, 0x49, 0x4e, 0x46, 0x4f, 0x53,
		0x45, 0x43, 0x54, 0x49, 0x4f, 0x4e };
	mas = bytes; //запись сигнатуры 
	char leftByte, rightByte;
	leftByte = (struc->NumOfFiles) % 256;
	rightByte = (struc->NumOfFiles) / 256;
	int reserve = 40960;
	int bNumFiles = 2;
	int sbytes = sizeof(bytes);
	int skp = sbytes + reserve + bNumFiles;
	char *str = struc->msg;
	int j = 0;
	HANDLE hFile;
	WIN32_FIND_DATA FindFileData;
	FILE *fl;
	int i = sbytes;
	for (i; i < skp; i++, mas++)
	{

		if (reserve > i >= (32768 + sbytes)) //приветственное сообщние
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
	}
	for (int k = 0, i = skp; k < bNumFiles; i++, k++)
	{
		char* FileName = struc->FilePath->value;
		mas[i] = (FileName)[k]; //путь файла
		i += 512;
		hFile = FindFirstFile(FileName, &FindFileData);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Invalid File Handle. GetLastError reports %d\n",
				GetLastError());
			return (0);
		}
		mas[i] = FindFileData.nFileSizeHigh;  //размер файла
		mas[i += 2] = FindFileData.nFileSizeLow;
		fl = fopen(FileName,"rb");
		if (f1 == NULL) printf("Could not open %s", FileName);
		while(!feof(fl)) fread((mas+i),sizeof(char),4096,fl); //тело файла
	}


	Size = (int*)sizeof(mas);
	return mas;
}