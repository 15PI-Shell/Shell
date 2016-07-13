#include "DirectoryExists.h"

BOOL DirectoryExists(char* szPath)
{
	if (strlen(szPath) > MAX_PATH)
		return 0;

	DWORD dwAttrib = GetFileAttributesA(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
