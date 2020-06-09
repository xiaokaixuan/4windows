#include "StdAfx.h"
#include "CImage.h"
#include <shlwapi.h>
#include "QQPicHide.h"

CWinApp theApp;

int ShowHelp(TCHAR* argv[])
{
	LPCTSTR lpFileName = PathFindFileName(argv[0]);
	_tprintf(_T("Usage: %s <foreground-image> <backgound-image>.\n\n"), lpFileName);
	return 0;
}

CImage* GenHiddenImage(CImage* whiteImage, CImage* blackImage);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_ftprintf(stderr, _T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		if (argc < 3) return ShowHelp(argv);
		CString strBlack(argv[1]), strWhite(argv[2]);
		CImage blackImage, whiteImage;
		blackImage.Load(strBlack);
		whiteImage.Load(strWhite);
		CImage* dstImage = GenHiddenImage(&whiteImage, &blackImage);
		dstImage->Save(_T("out.png"));
		delete dstImage;
	}
	
	return nRetCode;
}

CImage* GenHiddenImage(CImage* whiteImage, CImage* blackImage)
{
	int b_width = blackImage->GetWidth();
	int b_height = blackImage->GetHeight();
	int w_width = whiteImage->GetWidth();
	int w_height = whiteImage->GetHeight();
	
	//设定最终图片的尺寸
	int f_width = max(b_width, w_width);
	int f_height = max(b_height, w_height);

	//黑色图片距离边缘的距离
	int b_widthOffset = b_width == f_width ? 0 : (f_width - b_width) / 2;
	int b_heightOffset = b_height == f_height ? 0 : (f_height - b_height) / 2;
	
	//白色图片离边缘距离
	int w_widthOffset = w_width == f_width ? 0 : (f_width - w_width) / 2;
	int w_heightOffset = w_height == f_height ? 0 : (f_height - w_height) / 2;

	CImage* result = new CImage;
	result->Create(f_width, f_height, 32);
	result->SetHasAlphaChannel(true);

	for (int x = 0; x < f_width; x++)
	{
		for (int y = 0; y < f_height; y++)
		{
			//上下左右交叉排列黑白像素
			bool blackPixel = (x + y) % 2 == 0 ? true : false;
			
			int coor_x;
			int coor_y;
			//决定当前像素位置是否对应图一或图二某像素，如果没有，跳过循环
			bool validPixel = true;
			if (blackPixel) {
				coor_x = x - b_widthOffset;
				if (coor_x > b_width - 1) validPixel = false;
				coor_y = y - b_heightOffset;
				if (coor_y > b_height - 1) validPixel = false;
			} else {
				coor_x = x - w_widthOffset;
				if (coor_x > w_width - 1) validPixel = false;
				coor_y = y - w_heightOffset;
				if (coor_y > w_height - 1) validPixel = false;
			}
			
			validPixel = validPixel && coor_x >= 0 && coor_y >= 0;
			if (!validPixel) continue;
			
			//根据颜色计算像素灰度，设定透明度
			if (blackPixel) {
				COLORREF origin = blackImage->GetPixel(coor_x, coor_y);
				int gray = (GetRValue(origin) * 19595 + GetGValue(origin) * 38469 + GetBValue(origin) * 7472) >> 16;
				BYTE* pByte = (BYTE*)result->GetPixelAddress(x, y);
				pByte[3] = 255 - gray;
				pByte[0] = pByte[1] = pByte[2] = 0;
			} else {
				COLORREF origin = whiteImage->GetPixel(coor_x, coor_y);
				int gray = (GetRValue(origin) * 19595 + GetGValue(origin) * 38469 + GetBValue(origin) * 7472) >> 16;
				BYTE* pByte = (BYTE*)result->GetPixelAddress(x, y);
				pByte[3] = gray;
				pByte[0] = pByte[1] = pByte[2] = 255;
			}
		}
	}
	return result;
}


