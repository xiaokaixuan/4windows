// ToHardlink.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MyApp.h"
#include <locale.h>
#include "ToHardlink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_ftprintf(stderr, _T("ERROR: MFC Initialize FAIL.\n"));
			nRetCode = 1;
		}
		else
		{
			_tsetlocale(LC_ALL, _T("chs"));
			if (argc > 1 && !lstrcmpi(argv[1], _T("-v")))
			{
				theApp.SetVerbose(TRUE);
			}
			_ftprintf(stdout, _T("[INFO]Finding files...\n"));
			theApp.FindFiles(_T("."));
			_ftprintf(stdout, _T("[INFO]Processing files...\n"));
			theApp.ProcessFiles();
			_ftprintf(stdout, _T("[INFO]Process finish.\n"));
		}
	}
	else
	{
		_ftprintf(stderr, _T("ERROR: GetModuleHandle FAIL.\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
