#ifndef _LIB_EXAPI
#define _LIB_EXAPI
#pragma once

/********** Start icon definitons. ***********/

// .ico files definitions.
#pragma pack(push, 1)
typedef struct tagICONHEADER
{
	WORD idReserved;
    WORD idType;
    WORD idCount;
}ICONHEADER, *LPICONHEADER;

typedef struct tagICONDIRENTRY
{   
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    DWORD dwImageOffset;
}ICONDIRENTRY, *LPICONDIRENTRY;

// RT_GROUP_ICON definitions.
typedef struct tagGRPICONDIRENTRY
{   
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    WORD nID;
}GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct tagGRPICONDIR
{
    WORD idReserved;
    WORD idType;
    WORD idCount;
    GRPICONDIRENTRY idEntries[1];
}GRPICONDIR, *LPGRPICONDIR;
#pragma pack( pop )

/********** End icon definition. ***********/

// APIs list

UINT kxGetFileRows(LPCTSTR lpFileName);

BOOL kxIsObjectOnHeap(LPVOID pObject);
BOOL kxIsObjectOnStack(LPVOID pObject);

BOOL kxDeleteIcon(LPCTSTR lpExeName, LPCTSTR lpResName);
BOOL kxSaveIcon(LPCTSTR lpExeName, LPCTSTR lpResName, LPCTSTR lpIcoName);
BOOL kxInsertIcon(LPCTSTR lpExeName, LPCTSTR lpIconFile, LPCTSTR lpResName);

void kxGetSystrayIconRect(LPCTSTR lpTipText, LPRECT lpRect);
void kxAnimateWindowToSystray(HWND hwnd, LPCTSTR lpTipText);
void kxRestoreWindowFromSystray(HWND hwnd, LPCRECT lpRect, LPCTSTR lpTipText);

#endif //_LIB_EXAPI