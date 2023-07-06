#include <windows.h>
#include <stdio.h>
#include "RGBs.h"

void WINAPI parseGammaRGB(int *gammaRGB, int argc, char* argv[]);
typedef BOOL (WINAPI *Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);
typedef BOOL (WINAPI *Type_EnumDisplayDevicesA)(PVOID Unused, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags);

int main(int argc, char* argv[])
{
	int gammaRGB[3] = { 128, 128, 128 };
	parseGammaRGB(gammaRGB, argc, argv);
	
	HMODULE hGDI32 = LoadLibrary("gdi32.dll");
	HMODULE hUser32 = LoadLibrary("user32.dll");
	Type_SetDeviceGammaRamp pGetDeviceGammaRamp = (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "GetDeviceGammaRamp");
	Type_SetDeviceGammaRamp pSetDeviceGammaRamp = (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "SetDeviceGammaRamp");
	Type_EnumDisplayDevicesA pEnumDisplayDevicesA = (Type_EnumDisplayDevicesA)GetProcAddress(hUser32, "EnumDisplayDevicesA");
	WORD GammaArray[3][256] = { 0 };
	for (int iIndex = 0; iIndex < 256; ++iIndex)
	{
		int iArrayR = iIndex * (gammaRGB[0] + 128),
			iArrayG = iIndex * (gammaRGB[1] + 128),
			iArrayB = iIndex * (gammaRGB[2] + 128);
		if (iArrayR > 65535) iArrayR = 65535;
		if (iArrayG > 65535) iArrayG = 65535;
		if (iArrayB > 65535) iArrayB = 65535;
		GammaArray[0][iIndex] = (WORD)iArrayR;
		GammaArray[1][iIndex] = (WORD)iArrayG;
		GammaArray[2][iIndex] = (WORD)iArrayB;
	}
	DISPLAY_DEVICE dd = { 0 };
	dd.cb = sizeof(dd);
	HDC hGammaDC = NULL;
	for (DWORD dwDevNum = 0; pEnumDisplayDevicesA(NULL, dwDevNum, &dd, 0); ++dwDevNum)
	{
		if (!(dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) continue;
		hGammaDC = CreateDCA((LPCSTR)dd.DeviceName, NULL, NULL, NULL);
		pSetDeviceGammaRamp(hGammaDC, GammaArray);
		DeleteDC(hGammaDC);
	}
	return 0;
}

void WINAPI parseGammaRGB(int *gammaRGB, int argc, char* argv[])
{
	if (argc >= 4)
	{
		// ex. setgamma 128 128 128
		gammaRGB[0] = atoi(argv[1]);
		gammaRGB[1] = atoi(argv[2]);
		gammaRGB[2] = atoi(argv[3]);
	}
	else if (argc >= 2)
	{
		// ex. setgamma 6000K
		for (int i = 0; i < sizeof(RGBs)/sizeof(TEMP_RGB); ++i)
		{
			if (!stricmp(argv[1], RGBs[i].name))
			{
				gammaRGB[0] = RGBs[i].rValue - 128;
				gammaRGB[1] = RGBs[i].gValue - 128;
				gammaRGB[2] = RGBs[i].bValue - 128;
				break;
			}
		}
	}
}

