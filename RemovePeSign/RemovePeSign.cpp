// RemovePeSign.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <shlwapi.h>
#include "PeHandler.h"

int ShowHelp(TCHAR* argv[])
{
	LPCTSTR lpFileName = PathFindFileName(argv[0]);
	_tprintf(_T("\nUsage: %s <executable file>.\n\n"), lpFileName);
	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc < 2) return ShowHelp(argv);
	TCHAR szNewFile[2048] = { 0 };
	lstrcpyn(szNewFile, argv[1], sizeof(szNewFile)/sizeof(TCHAR));
	PathRemoveExtension(szNewFile);
	lstrcat(szNewFile, _T("_nosign"));
	lstrcat(szNewFile, PathFindExtension(argv[1]));

	LARGE_INTEGER liFileSize = { 0 };
	LPVOID p = MapPeFile(argv[1], &liFileSize);
	do
	{
		
		DWORD dwOffset = 0, dwSize = 0;
		if (!GetPeSignOffsetAndSize(p, &dwOffset, &dwSize)) break;
		RemovePeSign(p, &liFileSize, szNewFile);
	}
	while (0);
	UnmapPeFile(p);
	return 0;
}

