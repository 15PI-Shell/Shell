#include "DirectoryExists.h"

BOOL DirectoryExists(LPCTSTR szPath)
{
	if (strlen(szPath) > MAX_PATH)
		return 0;

	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
