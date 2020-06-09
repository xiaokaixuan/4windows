#include "stdafx.h"
#include <cstdio>
#include <clocale>

int showHelp(int errnum)
{
	switch (errnum)
	{
	case 1:
		_fputts(_T("ERROR: missing lnk file !"), stderr);
		break;
	case 2:
		_fputts(_T("ERROR: file can not open !"), stderr);
		break;
	case 3:
		_fputts(_T("ERROR: file is not lnk format !"), stderr);
		break;
	}
	return errnum;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc < 2) 
	{
		return showHelp(1);
	}
	_tsetlocale(LC_ALL, _T("chs"));
	FILE* pFile = _tfopen(argv[1], _T("rb+"));
	if (!pFile)
	{
		return showHelp(2);
	}
	unsigned char ch = 0;
	fread(&ch, sizeof(ch), 1, pFile);
	if (ch != 'L')
	{
		return showHelp(3);
	}
	fseek(pFile, 0x15, SEEK_SET);
	if (fread(&ch, sizeof(ch), 1, pFile) <= 0)
	{
		fclose(pFile);
		return showHelp(3);
	}
	ch ^= 0x10;
	fseek(pFile, -1, SEEK_CUR);
	fwrite(&ch, sizeof(ch), 1, pFile);
	fclose(pFile);
	return 0;
}

