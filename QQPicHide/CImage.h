#ifndef __CIMAGE_H__
#define __CIMAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <gdiplus.h>

#define CIMAGE_DC_CACHE_SIZE 4

class CImage;

class CImageDC
{
public:
	CImageDC(const CImage& image);
	~CImageDC() throw();
	
	operator HDC() const throw();
	
private:
	const CImage& m_image;
	HDC m_hDC;
};

class CImage
{
private:
	class CDCCache
	{
	public:
		CDCCache() throw();
		~CDCCache() throw();
		
		HDC GetDC() throw();
		void ReleaseDC(HDC) throw();
		
	private:
		HDC m_ahDCs[CIMAGE_DC_CACHE_SIZE];
	};
	
	class CInitGDIPlus
	{
	public:
		CInitGDIPlus() throw();
		~CInitGDIPlus() throw();
		
		bool Init() throw();
		void ReleaseGDIPlus() throw();
		void IncreaseCImageCount() throw();
		void DecreaseCImageCount() throw();
		
	private:
		ULONG_PTR		 m_dwToken;
		CRITICAL_SECTION m_sect;
		LONG			 m_nCImageObjects;
		DWORD			 m_dwLastError;
	};
	
	public:
		static const DWORD createAlphaChannel;
		
		static const DWORD excludeGIF;
		static const DWORD excludeBMP;
		static const DWORD excludeEMF;
		static const DWORD excludeWMF;
		static const DWORD excludeJPEG;
		static const DWORD excludePNG;
		static const DWORD excludeTIFF;
		static const DWORD excludeIcon;
		static const DWORD excludeOther;
		static const DWORD excludeDefaultLoad;
		static const DWORD excludeDefaultSave;
		static const DWORD excludeValid;
		
		enum DIBOrientation
		{
			DIBOR_DEFAULT,
			DIBOR_TOPDOWN,
			DIBOR_BOTTOMUP
		};
		
	public:
		CImage() throw();
		virtual ~CImage() throw();
		
		operator HBITMAP() const throw();
		BOOL AlphaBlend(HDC hDestDC, int xDest, int yDest, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER) const throw();
		BOOL AlphaBlend(HDC hDestDC, const POINT& pointDest, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER) const throw();
		BOOL AlphaBlend(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER) const throw();
		BOOL AlphaBlend(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, BYTE bSrcAlpha = 0xff, BYTE bBlendOp = AC_SRC_OVER) const throw();
		void Attach(HBITMAP hBitmap, DIBOrientation eOrientation = DIBOR_DEFAULT) throw();
		BOOL BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP = SRCCOPY) const throw();
		BOOL BitBlt(HDC hDestDC, const POINT& pointDest, DWORD dwROP = SRCCOPY) const throw();
		BOOL BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP = SRCCOPY) const throw();
		BOOL BitBlt(HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, DWORD dwROP = SRCCOPY) const throw();
		BOOL Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0) throw();
		BOOL CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, const DWORD* pdwBitmasks = NULL, DWORD dwFlags = 0) throw();
		void Destroy() throw();
		HBITMAP Detach() throw();
		BOOL Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight) const throw();
		BOOL Draw(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc) const throw();
		BOOL Draw(HDC hDestDC, int xDest, int yDest) const throw();
		BOOL Draw(HDC hDestDC, const POINT& pointDest) const throw();
		BOOL Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight ) const throw();
		BOOL Draw(HDC hDestDC, const RECT& rectDest) const throw();
		BOOL Draw(HDC hDestDC, const RECT& rectDest, Gdiplus::InterpolationMode interpolationMode) const throw();
		const void* GetBits() const throw();
		void* GetBits() throw();
		int GetBPP() const throw();
		void GetColorTable(UINT iFirstColor, UINT nColors, RGBQUAD* prgbColors) const throw();
		HDC GetDC() const throw();
		int GetHeight() const throw();
		int GetMaxColorTableEntries() const throw();
		int GetPitch() const throw();
		const void* GetPixelAddress(int x, int y) const throw();
		void* GetPixelAddress(int x, int y) throw();
		COLORREF GetPixel(int x,int y) const throw();
		LONG GetTransparentColor() const throw();
		int GetWidth() const throw();
		bool IsDIBSection() const throw();
		bool IsIndexed() const throw();
		bool IsNull() const throw();
		HRESULT Load(LPCTSTR pszFileName) throw();
		HRESULT Load(IStream* pStream) throw();
		void LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName) throw();
		void LoadFromResource(HINSTANCE hInstance, UINT nIDResource) throw();
		BOOL MaskBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD dwROP = SRCCOPY) const throw();
		BOOL MaskBlt(HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, HBITMAP hbmMask, const POINT& pointMask, DWORD dwROP = SRCCOPY) const throw();
		BOOL MaskBlt(HDC hDestDC, int xDest, int yDest, HBITMAP hbmMask, DWORD dwROP = SRCCOPY) const throw();
		BOOL MaskBlt(HDC hDestDC, const POINT& pointDest, HBITMAP hbmMask, DWORD dwROP = SRCCOPY) const throw();
		BOOL PlgBlt(HDC hDestDC, const POINT* pPoints, HBITMAP hbmMask = NULL) const throw();
		BOOL PlgBlt(HDC hDestDC, const POINT* pPoints, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, HBITMAP hbmMask = NULL, int xMask = 0, int yMask = 0) const throw();
		BOOL PlgBlt(HDC hDestDC, const POINT* pPoints, const RECT& rectSrc, HBITMAP hbmMask = NULL, const POINT& pointMask = CPoint( 0, 0 )) const throw();
		void ReleaseDC() const throw();
		HRESULT Save(IStream* pStream, REFGUID guidFileType) const throw();
		HRESULT Save(LPCTSTR pszFileName, REFGUID guidFileType = GUID_NULL) const throw();
		void SetColorTable(UINT iFirstColor, UINT nColors, const RGBQUAD* prgbColors) throw();
		void SetPixel(int x, int y, COLORREF color) throw();
		void SetPixelIndexed(int x, int y, int iIndex) throw();
		void SetPixelRGB(int x, int y, BYTE r, BYTE g, BYTE b) throw();
		void SetHasAlphaChannel(bool bHasAlphaChannel) throw();
		LONG SetTransparentColor(LONG iTransparentColor) throw();
		COLORREF SetTransparentColor(COLORREF clrTransparentColor) throw();
		BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP = SRCCOPY) const throw();
		BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP = SRCCOPY) const throw();
		BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP = SRCCOPY) const throw();
		BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP = SRCCOPY) const throw();
		BOOL TransparentBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, UINT crTransparent = CLR_INVALID) const throw();
		BOOL TransparentBlt(HDC hDestDC, const RECT& rectDest, UINT crTransparent = CLR_INVALID) const throw();
		BOOL TransparentBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, UINT crTransparent = CLR_INVALID) const throw();
		BOOL TransparentBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, UINT crTransparent = CLR_INVALID) const throw();
		
		static BOOL IsTransparencySupported() throw();

	private:
		HBITMAP	 m_hBitmap;
		void*	 m_pBits;
		int		 m_nWidth;
		int		 m_nHeight;
		int		 m_nPitch;
		int		 m_nBPP;
		bool	 m_bIsDIBSection;
		bool	 m_bHasAlphaChannel;
		LONG	 m_iTransparentColor;
		COLORREF m_clrTransparentColor;
		
		static CInitGDIPlus* GetInitGDIPlusInstance()
		{
			static CInitGDIPlus gdiPlus;
			return &gdiPlus;
		}
		
	public:
		static void ReleaseGDIPlus()
		{
			GetInitGDIPlusInstance()->ReleaseGDIPlus();
		}
		
	private:
		static CLSID FindCodecForExtension(LPCTSTR pszExtension, const Gdiplus::ImageCodecInfo* pCodecs, UINT nCodecs);
		static CLSID FindCodecForFileType(REFGUID guidFileType, const Gdiplus::ImageCodecInfo* pCodecs, UINT nCodecs);
		
		
		static bool ShouldExcludeFormat(REFGUID guidFileType, DWORD dwExclude) throw();
		void UpdateBitmapInfo(DIBOrientation eOrientation);
		HRESULT CreateFromGdiplusBitmap(Gdiplus::Bitmap& bmSrc) throw();
		
		static bool InitGDIPlus() throw();
		
		static int ComputePitch(int nWidth, int nBPP)
		{
			return( (((nWidth*nBPP)+31)/32)*4 );
		}
		
		COLORREF GetTransparentRGB() const;
		
	private:
		mutable	HDC		m_hDC;
		mutable int		m_nDCRefCount;
		mutable HBITMAP m_hOldBitmap;
		
		static CDCCache* GetCDCCacheInstance()
		{
			static CDCCache cache;
			return &cache;
		}
		
		static bool CImageStaticInitialize()
		{
			GetInitGDIPlusInstance();
			GetCDCCacheInstance();
			return true;
		}
		
		static bool isCImageStaticInitialized;
};

#endif  // __CIMAGE_H__

