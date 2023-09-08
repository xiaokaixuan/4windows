#include "StdAfx.h"
#include "ExAPI.h"

UINT kxGetFileRows(LPCTSTR lpFileName)
{
	UINT uCnt(0);
	BOOL bCanCover(TRUE);
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hMap != INVALID_HANDLE_VALUE)
		{
			LPCTSTR lpStart = (LPCTSTR)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
			LPCTSTR lpEnd = lpStart + GetFileSize(hFile, NULL);
			for (LPCTSTR lpBuff(lpStart);lpBuff < lpEnd; ++lpBuff)
			{
				if (*lpBuff == _T('\n'))
				{
					bCanCover = TRUE;
				}
				else if (*lpBuff != _T('\r') && bCanCover)
				{
					uCnt += 1;
					bCanCover = FALSE;
				}
			}
			UnmapViewOfFile(lpStart);
			CloseHandle(hMap);
		}
		CloseHandle(hFile);
	}
	return uCnt;
}

BOOL kxIsObjectOnStack(LPVOID pObject)
{
	BOOL bRet(FALSE);
	int nStack(0);
	MEMORY_BASIC_INFORMATION mi = {0};
	DWORD dwGet = VirtualQuery(&bRet, &mi, sizeof(mi));
	if (dwGet > 0)
	{
		bRet = ((DWORD)mi.BaseAddress >= (DWORD)pObject && (DWORD)pObject < (DWORD)mi.BaseAddress + mi.RegionSize);
	}
	return bRet;
}

BOOL kxIsObjectOnHeap(LPVOID pObject)
{
    BOOL bRet(FALSE);
    DWORD dwHeaps = GetProcessHeaps(0, NULL);
    LPHANDLE pHeaps = new HANDLE[dwHeaps];
    if(pHeaps != NULL)
    {
        MEMORY_BASIC_INFORMATION mi = {0};
        GetProcessHeaps(dwHeaps, pHeaps);
		
        for(DWORD i=0; i<dwHeaps; ++i)
        {
            VirtualQuery((LPVOID)pHeaps[i], &mi, sizeof(mi));
            
            if((DWORD)pObject >= (DWORD)mi.BaseAddress && (DWORD)pObject < (DWORD)mi.BaseAddress + mi.RegionSize)
            {
                bRet = TRUE;
                break;
            }
        }
    }
    delete []pHeaps;
	
    return bRet;
}

BOOL kxDeleteIcon(LPCTSTR lpExeName, LPCTSTR lpResName)
{
	BOOL bRet(FALSE);
	HMODULE hInst = LoadLibrary(lpExeName);
	if (NULL != hInst)
	{
		HRSRC hRes = FindResource(hInst, lpResName, RT_GROUP_ICON);
		if (NULL != hRes)
		{
			HGLOBAL hGlobal = LoadResource(hInst, hRes);
			if (NULL != hGlobal)
			{
				LPVOID lpData = LockResource(hGlobal);
				if (NULL != lpData)
				{
					GRPICONDIR grpDir = {0};
					CopyMemory(&grpDir, lpData, sizeof(grpDir));
					DWORD dwGrpSize = sizeof(GRPICONDIR) + (grpDir.idCount - 1)*sizeof(GRPICONDIRENTRY);
					LPGRPICONDIR lpGrp = (LPGRPICONDIR)new BYTE[dwGrpSize]();
					CopyMemory(lpGrp, lpData, dwGrpSize);

					UnlockResource(hGlobal);
					FreeLibrary(hInst);
					hInst = NULL;
					
					HANDLE hUpdate = BeginUpdateResource(lpExeName, FALSE);
					if (NULL != hUpdate)
					{
						bRet = UpdateResource(hUpdate, RT_GROUP_ICON, lpResName
							, MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT)
							, NULL, 0);
						for (int i(0); i < grpDir.idCount; ++i)
						{
							bRet = UpdateResource(hUpdate, RT_ICON
								, MAKEINTRESOURCE(lpGrp->idEntries[i].nID)
								, MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT)
								, NULL, 0);
						}
						EndUpdateResource(hUpdate, FALSE);
					}
					delete []lpGrp;
					lpGrp = NULL;
				}
			}
		}
	}
	if (NULL != hInst)
	{
		FreeLibrary(hInst);
		hInst = NULL;
	}
	return bRet;
}

BOOL kxInsertIcon(LPCTSTR lpExeName, LPCTSTR lpIconFile, LPCTSTR lpResName)
{   
	BOOL bRet(FALSE);
	DWORD dwReaded(0);

	HANDLE hFile = CreateFile(lpIconFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		kxDeleteIcon(lpExeName, lpResName);

		ICONHEADER header = {0};
		ReadFile(hFile, &header, sizeof(header), &dwReaded, NULL);
		if (dwReaded >= sizeof(header))
		{
			LPICONDIRENTRY pIcons = new ICONDIRENTRY[header.idCount]();
			ReadFile(hFile, pIcons, header.idCount*sizeof(ICONDIRENTRY), &dwReaded, NULL);
			if (dwReaded >= header.idCount*sizeof(ICONDIRENTRY))
			{
				DWORD dwGrpSize = sizeof(GRPICONDIR) + (header.idCount - 1)*sizeof(GRPICONDIRENTRY);
				LPGRPICONDIR lpGrp = (LPGRPICONDIR)new BYTE[dwGrpSize]();
				lpGrp->idType = header.idType;
				lpGrp->idCount = header.idCount;
				lpGrp->idReserved = header.idReserved;

				HMODULE hInst = ::LoadLibrary(lpExeName);
				if (hInst != NULL)
				{
					int nIndex(0);
					WORD wID(1);
					for (; wID <= MAXWORD; ++wID)
					{
						if (NULL == FindResource(hInst, MAKEINTRESOURCE(wID), RT_ICON))
						{
							CopyMemory(lpGrp->idEntries + nIndex, pIcons + nIndex, 12);
							lpGrp->idEntries[nIndex].nID = wID;
							nIndex += 1;
							if (nIndex >= header.idCount)
							{
								break;
							}
						}
					}
					FreeLibrary(hInst);

					if (wID <= MAXWORD)
					{
						HANDLE hUpdate = BeginUpdateResource(lpExeName, FALSE);
						if (NULL != hUpdate)
						{
							bRet = UpdateResource(hUpdate, RT_GROUP_ICON, lpResName
								, MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), lpGrp, dwGrpSize);
							
							for (int i(0); i < header.idCount; ++i)
							{
								LPBYTE lpIcoDat = new BYTE[pIcons[i].dwBytesInRes]();
								SetFilePointer(hFile, pIcons[i].dwImageOffset, NULL, FILE_BEGIN);
								ReadFile(hFile, lpIcoDat, pIcons[i].dwBytesInRes, &dwReaded, NULL);
								if (dwReaded >= pIcons[i].dwBytesInRes)
								{
									bRet = UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(lpGrp->idEntries[i].nID)
										, MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), lpIcoDat, pIcons[i].dwBytesInRes);
								}
								delete []lpIcoDat;
								lpIcoDat = NULL;
							}
							EndUpdateResource(hUpdate, FALSE);
						}
					}
				}
				delete []lpGrp;
				lpGrp = NULL;
			}
			delete []pIcons;
			pIcons = NULL;
		}
		CloseHandle(hFile);
	}
	return bRet;
}

BOOL kxSaveIcon(LPCTSTR lpExeName, LPCTSTR lpResName, LPCTSTR lpIcoName)
{
	BOOL bRet(FALSE);

	DWORD dwWritten(0);

	HMODULE hInst = LoadLibrary(lpExeName);
	if (NULL != hInst)
	{
		HRSRC hRes = FindResource(hInst, lpResName, RT_GROUP_ICON);
		if (NULL != hRes)
		{
			HGLOBAL hGlobal = LoadResource(hInst, hRes);
			if (NULL != hGlobal)
			{
				LPVOID lpBuf = LockResource(hGlobal);
				if (NULL != lpBuf)
				{
					GRPICONDIR grpDir = {0};
					CopyMemory(&grpDir, lpBuf, sizeof(GRPICONDIR));
					
					DWORD dwGrpSize = sizeof(GRPICONDIR) + (grpDir.idCount - 1)*sizeof(GRPICONDIRENTRY);
					LPGRPICONDIR lpGrp = (LPGRPICONDIR)new BYTE[dwGrpSize]();
					CopyMemory(lpGrp, lpBuf, dwGrpSize);
					
					UnlockResource(hGlobal);
					
					ICONHEADER header = {grpDir.idReserved, grpDir.idType, grpDir.idCount};
					LPICONDIRENTRY lpIcons = new ICONDIRENTRY[header.idCount]();
					CopyMemory(lpIcons, lpGrp->idEntries, 12);
					lpIcons[0].dwImageOffset = sizeof(header) + header.idCount*sizeof(ICONDIRENTRY); 
					for (int i(1); i < header.idCount; ++i)
					{
						CopyMemory(lpIcons + i, lpGrp->idEntries + i, 12);
						lpIcons[i].dwImageOffset = lpIcons[i-1].dwImageOffset + lpIcons[i-1].dwBytesInRes;
					}
					
					HANDLE hFile = CreateFile(lpIcoName, GENERIC_WRITE, 0, NULL
						, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (INVALID_HANDLE_VALUE != hFile)
					{
						bRet = WriteFile(hFile, &header, sizeof(header), &dwWritten, NULL);
						bRet = WriteFile(hFile, lpIcons, sizeof(ICONDIRENTRY)*header.idCount, &dwWritten, NULL);
						
						for (int nIndex(0); nIndex < header.idCount; ++nIndex)
						{
							WORD wId = lpGrp->idEntries[nIndex].nID;
							HRSRC hRes(NULL);
							HGLOBAL hGlobal(NULL);
							LPVOID  lpBuf(NULL);
							if ((hRes = FindResource(hInst, MAKEINTRESOURCE(wId), RT_ICON)) != NULL &&
								(hGlobal = LoadResource(hInst, hRes)) != NULL &&
								(lpBuf = LockResource(hGlobal)) != NULL)
							{
								int nResSize = SizeofResource(hInst, hRes);
								bRet = WriteFile(hFile, lpBuf, nResSize, &dwWritten, NULL);
								UnlockResource(hGlobal);
							}
						}
						CloseHandle(hFile);
					}
					delete []lpIcons;
					lpIcons = NULL;
					delete []lpGrp;
					lpGrp = NULL;						
				}
			}
		}
		FreeLibrary(hInst);
	}

	return bRet;
}

void kxGetSystrayIconRect(LPCTSTR lpTipText, LPRECT lpRect)
{
	HWND hwnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (hwnd != NULL)
	{
		hwnd = FindWindowEx(hwnd, NULL, _T("TrayNotifyWnd"), NULL);
		if (hwnd != NULL)
		{
			hwnd = FindWindowEx(hwnd, NULL, _T("SysPager"), NULL);
			if (hwnd != NULL)
			{
				hwnd = FindWindowEx(hwnd, NULL, _T("ToolbarWindow32"), NULL);
			}
		}
	}
	if (hwnd != NULL)
	{
		DWORD proID(0);
		GetWindowThreadProcessId(hwnd, &proID);
		if (proID != 0)
		{
			HANDLE hPro = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, proID);
			
			typedef struct tagSYSTRAYINFO
			{
				TBBUTTON tbbtn;
				RECT     rect;
				TCHAR    szTxt[MAX_PATH];
			} SYSTRAYINFO, *LPSYSTRAYINFO;
			
			LPVOID pBuf(NULL);
			if (hPro != NULL)
			{
				pBuf = VirtualAllocEx(hPro, NULL, sizeof(SYSTRAYINFO), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			}
			if (pBuf != NULL)
			{
				SYSTRAYINFO info;
				ZeroMemory(&info, sizeof(info));
				LPSYSTRAYINFO pInfo = (LPSYSTRAYINFO)pBuf;
				
				if (WriteProcessMemory(hPro, pBuf, &info, sizeof(info), NULL))
				{
					int iCnt = SendMessage(hwnd, TB_BUTTONCOUNT, 0, 0);
					int i(0);
					for (; i < iCnt; ++i)
					{
						ZeroMemory(&info, sizeof(info));
						
						SendMessage(hwnd, TB_GETBUTTON, i, (LPARAM)&pInfo->tbbtn);
						ReadProcessMemory(hPro, pInfo, &info.tbbtn, sizeof(info.tbbtn), NULL);
						
						SendMessage(hwnd, TB_GETBUTTONTEXT, info.tbbtn.idCommand, (LPARAM)pInfo->szTxt);
						ReadProcessMemory(hPro, pInfo->szTxt, info.szTxt, sizeof(info.szTxt), NULL);
						
						if (lstrcmpi(info.szTxt, lpTipText) == 0)
						{
							SendMessage(hwnd, TB_GETITEMRECT, i, (LPARAM)&pInfo->rect);
							ReadProcessMemory(hPro, &pInfo->rect, &info.rect, sizeof(info.rect), NULL);
							break;
						}
					}
					
					CPoint lt(info.rect.left, info.rect.top);
					CPoint rb(info.rect.right, info.rect.bottom);
					ClientToScreen(hwnd, &lt);
					ClientToScreen(hwnd, &rb);
					
					lpRect->left = lt.x;
					lpRect->top = lt.y;
					lpRect->right = rb.x;
					lpRect->bottom = rb.y;
				}
				VirtualFreeEx(hPro, pBuf, 0, MEM_RELEASE);
			}
			CloseHandle(hPro);
		}
	}
}

void kxAnimateWindowToSystray(HWND hwnd, LPCTSTR lpTipText)
{
	CRect destRc, curRc;
	kxGetSystrayIconRect(lpTipText, &destRc);
	GetWindowRect(hwnd, &curRc);
	DrawAnimatedRects(hwnd, IDANI_CAPTION, &curRc, &destRc);
	ShowWindow(hwnd, SW_HIDE);
}

void kxRestoreWindowFromSystray(HWND hwnd, LPCRECT lpRect, LPCTSTR lpTipText)
{
	CRect destRc(*lpRect), curRc;
	kxGetSystrayIconRect(lpTipText, &curRc);
	DrawAnimatedRects(hwnd, IDANI_CAPTION, &curRc, &destRc);
	ShowWindow(hwnd, SW_SHOW);
}
