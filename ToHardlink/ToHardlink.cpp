// ToHardlink.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MyApp.h"
#include <locale.h>
#include <vector>
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
			std::vector<CString> strVec;
			for (int i(1); i < argc; ++i) strVec.emplace_back(argv[i]);
			auto iter = std::find_if(strVec.begin(), strVec.end(), [](const CString& str)
				{
					return str.CompareNoCase(_T("--verbose")) == 0;
				});
			if (iter != strVec.end())
			{
				strVec.erase(iter);
				theApp.SetVerbose(TRUE);
			}
			if (strVec.empty())
			{
				_ftprintf(stdout, _T("\nExample: ToHardLink.exe *.mp4;*.mkv --verbose\n"));
				return nRetCode = 1;
			}
			CString strFilter(strVec.front());
			_ftprintf(stdout, _T("[INFO]Finding files %s ...\n"), (LPCTSTR)strFilter);
			theApp.FindFiles(_T("."), strFilter);
			_ftprintf(stdout, _T("[INFO]Found files %d...\n"), theApp.GetFileCount());
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
