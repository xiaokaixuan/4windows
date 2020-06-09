#include "StdAfx.h"
#include <clocale>
#include <shlwapi.h>
#include "CImage.h"

#ifndef ASSERT
#include <cassert>
#define ASSERT assert
#endif // ASSERT

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

const DWORD CImage::createAlphaChannel	= 0x01;
const DWORD CImage::excludeGIF			= 0x01;
const DWORD CImage::excludeBMP			= 0x02;
const DWORD CImage::excludeEMF			= 0x04;
const DWORD CImage::excludeWMF			= 0x08;
const DWORD CImage::excludeJPEG			= 0x10;
const DWORD CImage::excludePNG			= 0x20;
const DWORD CImage::excludeTIFF			= 0x40;
const DWORD CImage::excludeIcon			= 0x80;
const DWORD CImage::excludeOther		= 0x80000000;
const DWORD CImage::excludeDefaultLoad	= 0;
const DWORD CImage::excludeDefaultSave	= excludeIcon|excludeEMF|excludeWMF;
const DWORD CImage::excludeValid		= 0x800000ff;
bool CImage::isCImageStaticInitialized	= CImage::CImageStaticInitialize();

CImageDC::CImageDC(const CImage& image)
: m_image( image )
, m_hDC( image.GetDC() )
{
	if( m_hDC == NULL )
	{
		throw E_OUTOFMEMORY;
	}
}

CImageDC::~CImageDC() throw()
{
	m_image.ReleaseDC();
}

CImageDC::operator HDC() const throw()
{
	return( m_hDC );
}

CImage::CInitGDIPlus::CInitGDIPlus() throw()
: m_dwToken( 0 )
, m_nCImageObjects( 0 )
, m_dwLastError(S_OK)
{
	_wsetlocale(LC_ALL, L"chs");
	InitializeCriticalSection(&m_sect);
}

CImage::CInitGDIPlus::~CInitGDIPlus() throw()
{
	ReleaseGDIPlus();
	DeleteCriticalSection(&m_sect);
}

bool CImage::CInitGDIPlus::Init() throw()
{
	EnterCriticalSection(&m_sect);
	bool fRet = true;

	if( m_dwToken == 0 )
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;
		Gdiplus::Status status = Gdiplus::GdiplusStartup( &m_dwToken, &input, &output );
		if( status != Gdiplus::Ok )
		{
			fRet = false;
		}
	}
	LeaveCriticalSection(&m_sect);
	return fRet;
}

void CImage::CInitGDIPlus::ReleaseGDIPlus() throw()
{
	EnterCriticalSection(&m_sect);
	if( m_dwToken != 0 )
	{
		Gdiplus::GdiplusShutdown( m_dwToken );
	}
	m_dwToken = 0;
	LeaveCriticalSection(&m_sect);
}

void CImage::CInitGDIPlus::IncreaseCImageCount() throw()
{
	EnterCriticalSection(&m_sect);
	m_nCImageObjects++;
	LeaveCriticalSection(&m_sect);
}

void CImage::CInitGDIPlus::DecreaseCImageCount() throw()
{
	EnterCriticalSection(&m_sect);
	if( --m_nCImageObjects == 0 )
	{
		ReleaseGDIPlus();
	}
	LeaveCriticalSection(&m_sect);
}

CImage::CDCCache::CDCCache() throw()
{
	int iDC;

	for( iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		m_ahDCs[iDC] = NULL;
	}
}

CImage::CDCCache::~CDCCache() throw()
{
	int iDC;

	for( iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		if( m_ahDCs[iDC] != NULL )
		{
			::DeleteDC( m_ahDCs[iDC] );
		}
	}
}

HDC CImage::CDCCache::GetDC() throw()
{
	HDC hDC(NULL);

	for( int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		hDC = reinterpret_cast< HDC >( InterlockedExchange( reinterpret_cast< LPLONG >(&m_ahDCs[iDC]), 0 ) );
		if( hDC != NULL )
		{
			return( hDC );
		}
	}

	hDC = ::CreateCompatibleDC( NULL );

	return( hDC );
}

void CImage::CDCCache::ReleaseDC(HDC hDC) throw()
{
	for( int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		HDC hOldDC(NULL);

		hOldDC = reinterpret_cast< HDC >( InterlockedExchange( reinterpret_cast< LPLONG >(&m_ahDCs[iDC]), (LONG)hDC ) );
		if( hOldDC == NULL )
		{
			return;
		}
		else
		{
			hDC = hOldDC;
		}
	}
	if( hDC != NULL )
	{
		::DeleteDC( hDC );
	}
}

CImage::CImage() throw()
: m_hBitmap( NULL )
, m_pBits( NULL )
, m_hDC( NULL )
, m_nDCRefCount( 0 )
, m_hOldBitmap( NULL )
, m_nWidth( 0 )
, m_nHeight( 0 )
, m_nPitch( 0 )
, m_nBPP( 0 )
, m_iTransparentColor( -1 )
, m_clrTransparentColor( (COLORREF)-1 )
, m_bHasAlphaChannel( false )
, m_bIsDIBSection( false )
{
	GetInitGDIPlusInstance()->IncreaseCImageCount();
}

CImage::~CImage() throw()
{
	Destroy();
	GetInitGDIPlusInstance()->DecreaseCImageCount();
}

CImage::operator HBITMAP() const throw()
{
	return( m_hBitmap );
}

BOOL CImage::AlphaBlend(HDC hDestDC, int xDest, int yDest, BYTE bSrcAlpha, BYTE bBlendOp) const throw()
{
	return AlphaBlend( hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, m_nWidth, m_nHeight, bSrcAlpha, bBlendOp );
}

BOOL CImage::AlphaBlend(HDC hDestDC, const POINT& pointDest, BYTE bSrcAlpha, BYTE bBlendOp) const throw()
{
	return AlphaBlend( hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0, 0, m_nWidth, m_nHeight, bSrcAlpha, bBlendOp );
}

BOOL CImage::AlphaBlend(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BYTE bSrcAlpha, BYTE bBlendOp) const throw()
{
	BLENDFUNCTION blend;
	BOOL bResult;

	blend.SourceConstantAlpha = bSrcAlpha;
	blend.BlendOp = bBlendOp;
	blend.BlendFlags = 0;
	if( m_bHasAlphaChannel )
	{
		blend.AlphaFormat = 1;
	}
	else
	{
		blend.AlphaFormat = 0;
	}

	GetDC();

	bResult = ::AlphaBlend( hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, blend );

	ReleaseDC();

	return( bResult );
}

BOOL CImage::AlphaBlend(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, BYTE bSrcAlpha, BYTE bBlendOp) const throw()
{
	return AlphaBlend(hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left, rectSrc.top,
		rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top, bSrcAlpha,
		bBlendOp);
}

void CImage::Attach(HBITMAP hBitmap, DIBOrientation eOrientation) throw()
{
	ASSERT( m_hBitmap == NULL );
	ASSERT( hBitmap != NULL );

	m_hBitmap = hBitmap;

	UpdateBitmapInfo(eOrientation);
}

BOOL CImage::BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP) const throw()
{
	return BitBlt(hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, dwROP);
}

BOOL CImage::BitBlt(HDC hDestDC, const POINT& pointDest, DWORD dwROP) const throw()
{
	return BitBlt(hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0, 0, dwROP);
}

BOOL CImage::BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );

	GetDC();

	BOOL bResult = ::BitBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC,
		xSrc, ySrc, dwROP );

	ReleaseDC();

	return bResult;
}

BOOL CImage::BitBlt(HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, DWORD dwROP) const throw()
{
	return BitBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, pointSrc.x, pointSrc.y,
		dwROP );
}

BOOL CImage::Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags) throw()
{
	return( CreateEx( nWidth, nHeight, nBPP, BI_RGB, NULL, dwFlags ) );
}

BOOL CImage::CreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression, const DWORD* pdwBitfields, DWORD dwFlags) throw()
{
	LPBITMAPINFO pbmi;
	HBITMAP hBitmap;

	ASSERT( (eCompression == BI_RGB) || (eCompression == BI_BITFIELDS) );
	if( dwFlags&createAlphaChannel )
	{
		ASSERT( (nBPP == 32) && (eCompression == BI_RGB) );
	}

	pbmi = (LPBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256];
	if( pbmi == NULL )
		return FALSE;

	memset( &pbmi->bmiHeader, 0, sizeof( pbmi->bmiHeader ) );
	pbmi->bmiHeader.biSize = sizeof( pbmi->bmiHeader );
	pbmi->bmiHeader.biWidth = nWidth;
	pbmi->bmiHeader.biHeight = nHeight;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = USHORT( nBPP );
	pbmi->bmiHeader.biCompression = eCompression;
	if( nBPP <= 8 )
	{
		ASSERT( eCompression == BI_RGB );
		memset( pbmi->bmiColors, 0, 256*sizeof( RGBQUAD ) );
	}

	else
	{
		if( eCompression == BI_BITFIELDS )
		{
			ASSERT( pdwBitfields != NULL );
			memcpy(pbmi->bmiColors, pdwBitfields, 3*sizeof( DWORD ));
		}
	}

	hBitmap = ::CreateDIBSection( NULL, pbmi, DIB_RGB_COLORS, &m_pBits, NULL, 0 );
	if( hBitmap == NULL )
	{
		return( FALSE );
	}

	Attach( hBitmap, (nHeight < 0) ? DIBOR_TOPDOWN : DIBOR_BOTTOMUP );

	if( dwFlags&createAlphaChannel )
	{
		m_bHasAlphaChannel = true;
	}

	delete []pbmi;

	return( TRUE );
}

void CImage::Destroy() throw()
{
	HBITMAP hBitmap;

	if( m_hBitmap != NULL )
	{
		hBitmap = Detach();
		::DeleteObject( hBitmap );
	}
}

HBITMAP CImage::Detach() throw()
{
	HBITMAP hBitmap;

	ASSERT( m_hBitmap != NULL );
	ASSERT( m_hDC == NULL );

	hBitmap = m_hBitmap;
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBPP = 0;
	m_nPitch = 0;
	m_iTransparentColor = -1;
	m_clrTransparentColor = (COLORREF)-1;
	m_bHasAlphaChannel = false;
	m_bIsDIBSection = false;

	return( hBitmap );
}

BOOL CImage::Draw(HDC hDestDC, const RECT& rectDest) const throw()
{
	return Draw( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, 0, 0, m_nWidth,
		m_nHeight );
}

BOOL CImage::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight) const throw()
{
	return Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 0, m_nWidth, m_nHeight);
}

BOOL CImage::Draw(HDC hDestDC, const POINT& pointDest) const throw()
{
	return Draw( hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0, 0, m_nWidth, m_nHeight );
}

BOOL CImage::Draw(HDC hDestDC, int xDest, int yDest) const throw()
{
	return Draw( hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, m_nWidth, m_nHeight );
}

BOOL CImage::Draw(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc) const throw()
{
	return Draw( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left, rectSrc.top,
		rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top );
}

BOOL CImage::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight) const throw()
{
	BOOL bResult;

	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL);
	ASSERT( nDestWidth > 0 );
	ASSERT( nDestHeight > 0 );
	ASSERT( nSrcWidth > 0 );
	ASSERT( nSrcHeight > 0 );

	GetDC();

	if( ((m_iTransparentColor != -1) || (m_clrTransparentColor != (COLORREF)-1)) && IsTransparencySupported() )
	{
		bResult = ::TransparentBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight,
			m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, GetTransparentRGB() );
	}
	else if( m_bHasAlphaChannel && IsTransparencySupported() )
	{
		BLENDFUNCTION bf;

		bf.BlendOp = 0;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 0xff;
		bf.AlphaFormat = 1;
		bResult = ::AlphaBlend( hDestDC, xDest, yDest, nDestWidth, nDestHeight,
			m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, bf );
	}
	else
	{
		bResult = ::StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight,
			m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, SRCCOPY );
	}

	ReleaseDC();

	return( bResult );
}

BOOL CImage::Draw(HDC hDestDC, const RECT& rectDest, Gdiplus::InterpolationMode interpolationMode) const throw()
{
	if (!InitGDIPlus())
	{
		return FALSE;
	}

	Gdiplus::Bitmap bm(m_hBitmap, NULL);
	if (bm.GetLastStatus() != Gdiplus::Ok)
	{
		return FALSE;
	}

	Gdiplus::Graphics dcDst(hDestDC);
	dcDst.SetInterpolationMode(interpolationMode);

	Gdiplus::Status status = dcDst.DrawImage(&bm, rectDest.left, rectDest.top, rectDest.right - rectDest.left, rectDest.bottom - rectDest.top);

	return status == Gdiplus::Ok;
}

const void* CImage::GetBits() const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );

	return( m_pBits );
}

void* CImage::GetBits() throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );

	return( m_pBits );
}

int CImage::GetBPP() const throw()
{
	ASSERT( m_hBitmap != NULL );

	return( m_nBPP );
}

void CImage::GetColorTable(UINT iFirstColor, UINT nColors, RGBQUAD* prgbColors) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( m_pBits != NULL );
	ASSERT( IsIndexed() );

	GetDC();

	::GetDIBColorTable( m_hDC, iFirstColor, nColors, prgbColors );

	ReleaseDC();
}

 HDC CImage::GetDC() const throw()
{
	ASSERT( m_hBitmap != NULL );

	m_nDCRefCount++;
	if( m_hDC == NULL )
	{
		m_hDC = GetCDCCacheInstance()->GetDC();
		m_hOldBitmap = HBITMAP( ::SelectObject( m_hDC, m_hBitmap ) );
	}

	return( m_hDC );
}

bool CImage::ShouldExcludeFormat(REFGUID guidFileType, DWORD dwExclude) throw()
{
	static const GUID* apguidFormats[] =
	{
		&Gdiplus::ImageFormatGIF,
		&Gdiplus::ImageFormatBMP,
		&Gdiplus::ImageFormatEMF,
		&Gdiplus::ImageFormatWMF,
		&Gdiplus::ImageFormatJPEG,
		&Gdiplus::ImageFormatPNG,
		&Gdiplus::ImageFormatTIFF,
		&Gdiplus::ImageFormatIcon,
		NULL
	};

	ASSERT( (dwExclude|excludeValid) == excludeValid );
	for( int iFormat = 0; apguidFormats[iFormat] != NULL; iFormat++ )
	{
		if( guidFileType == *apguidFormats[iFormat] )
		{
			return( (dwExclude&(1<<iFormat)) != 0 );
		}
	}

	return( (dwExclude&excludeOther) != 0 );
}

int CImage::GetHeight() const throw()
{
	ASSERT( m_hBitmap != NULL );

	return( m_nHeight );
}

 int CImage::GetMaxColorTableEntries() const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );

	if( IsIndexed() )
	{
		return( 1<<m_nBPP );
	}
	else
	{
		return( 0 );
	}
}

 int CImage::GetPitch() const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );

	return( m_nPitch );
}

COLORREF CImage::GetPixel(int x, int y) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );

	GetDC();

	COLORREF clr = ::GetPixel( m_hDC, x, y );

	ReleaseDC();

	return( clr );
}

const void* CImage::GetPixelAddress(int x, int y) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );

	return( LPBYTE( m_pBits )+(y*m_nPitch)+((x*m_nBPP)/8) );
}

void* CImage::GetPixelAddress(int x, int y) throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );

	return( LPBYTE( m_pBits )+(y*m_nPitch)+((x*m_nBPP)/8) );
}

LONG CImage::GetTransparentColor() const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( (m_nBPP == 4) || (m_nBPP == 8) );

	return( m_iTransparentColor );
}

int CImage::GetWidth() const throw()
{
	ASSERT( m_hBitmap != NULL );

	return( m_nWidth );
}

bool CImage::IsDIBSection() const throw()
{
	return( m_bIsDIBSection );
}

bool CImage::IsIndexed() const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );

	return( m_nBPP <= 8 );
}

bool CImage::IsNull() const throw()
{
	return( m_hBitmap == NULL );
}

HRESULT CImage::Load(IStream* pStream) throw()
{
	if( !InitGDIPlus() )
	{
		return( E_FAIL );
	}

	Gdiplus::Bitmap bmSrc( pStream );
	if( bmSrc.GetLastStatus() != Gdiplus::Ok )
	{
		return( E_FAIL );
	}

	return( CreateFromGdiplusBitmap( bmSrc ) );
}

HRESULT CImage::Load(LPCTSTR pszFileName) throw()
{
	if( !InitGDIPlus() )
	{
		return( E_FAIL );
	}

	WCHAR wszFileName[MAX_PATH] = { 0 };
#if defined(UNICODE) || defined(_UNICODE)
	lstrcpyn(wszFileName, pszFileName, MAX_PATH);
#else
	mbstowcs(wszFileName, pszFileName, MAX_PATH);
#endif

	Gdiplus::Bitmap bmSrc( wszFileName );
	if( bmSrc.GetLastStatus() != Gdiplus::Ok )
	{
		return( E_FAIL );
	}

	return( CreateFromGdiplusBitmap( bmSrc ) );
}

HRESULT CImage::CreateFromGdiplusBitmap(Gdiplus::Bitmap& bmSrc) throw()
{
	Gdiplus::PixelFormat eSrcPixelFormat = bmSrc.GetPixelFormat();
	UINT nBPP = 32;
	DWORD dwFlags = 0;
	Gdiplus::PixelFormat eDestPixelFormat = PixelFormat32bppRGB;
	if( eSrcPixelFormat&PixelFormatGDI )
	{
		nBPP = Gdiplus::GetPixelFormatSize( eSrcPixelFormat );
		eDestPixelFormat = eSrcPixelFormat;
	}
	if( Gdiplus::IsAlphaPixelFormat( eSrcPixelFormat ) )
	{
		nBPP = 32;
		dwFlags |= createAlphaChannel;
		eDestPixelFormat = PixelFormat32bppARGB;
	}

	BOOL bSuccess = Create( bmSrc.GetWidth(), bmSrc.GetHeight(), nBPP, dwFlags );
	if( !bSuccess )
	{
		return( E_FAIL );
	}

	Gdiplus::ColorPalette* pPalette = NULL;
	if( Gdiplus::IsIndexedPixelFormat( eSrcPixelFormat ) )
	{
		UINT nPaletteSize = bmSrc.GetPaletteSize();
		pPalette = reinterpret_cast< Gdiplus::ColorPalette* >( new BYTE[nPaletteSize] );

		if( pPalette == NULL )
			return E_OUTOFMEMORY;

		bmSrc.GetPalette( pPalette, nPaletteSize );

		RGBQUAD argbPalette[256];
		ASSERT( (pPalette->Count > 0) && (pPalette->Count <= 256) );
		for( UINT iColor = 0; iColor < pPalette->Count; iColor++ )
		{
			Gdiplus::ARGB color = pPalette->Entries[iColor];
			argbPalette[iColor].rgbRed = (BYTE)( (color>>RED_SHIFT) & 0xff );
			argbPalette[iColor].rgbGreen = (BYTE)( (color>>GREEN_SHIFT) & 0xff );
			argbPalette[iColor].rgbBlue = (BYTE)( (color>>BLUE_SHIFT) & 0xff );
			argbPalette[iColor].rgbReserved = 0;
		}

		SetColorTable( 0, pPalette->Count, argbPalette );

		delete []pPalette;
	}

	if( eDestPixelFormat == eSrcPixelFormat )
	{
		// The pixel formats are identical, so just memcpy the rows.
		Gdiplus::BitmapData data;
		Gdiplus::Rect rect( 0, 0, GetWidth(), GetHeight() );
		if(bmSrc.LockBits( &rect, Gdiplus::ImageLockModeRead, eSrcPixelFormat, &data )!=Gdiplus::Ok)
		{
			return E_OUTOFMEMORY;
		}

		size_t nBytesPerRow = ((nBPP*GetWidth() + 7)&~7) / 8;
		BYTE* pbDestRow = static_cast< BYTE* >( GetBits() );
		BYTE* pbSrcRow = static_cast< BYTE* >( data.Scan0 );
		for( int y = 0; y < GetHeight(); y++ )
		{
			memcpy(pbDestRow, pbSrcRow, nBytesPerRow);
			pbDestRow += GetPitch();
			pbSrcRow += data.Stride;
		}

		bmSrc.UnlockBits( &data );
	}
	else
	{
		// Let GDI+ work its magic
		Gdiplus::Bitmap bmDest( GetWidth(), GetHeight(), GetPitch(), eDestPixelFormat, static_cast< BYTE* >( GetBits() ) );
		Gdiplus::Graphics gDest( &bmDest );

		gDest.DrawImage( &bmSrc, 0, 0 );
	}

	return( S_OK );
}

void CImage::LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName) throw()
{
	HBITMAP hBitmap;

	hBitmap = HBITMAP( ::LoadImage( hInstance, pszResourceName, IMAGE_BITMAP, 0,
		0, LR_CREATEDIBSECTION ) );

	Attach( hBitmap );
}

void CImage::LoadFromResource(HINSTANCE hInstance, UINT nIDResource) throw()
{
	LoadFromResource( hInstance, MAKEINTRESOURCE( nIDResource ) );
}

BOOL CImage::MaskBlt(HDC hDestDC, int xDest, int yDest, int nWidth, int nHeight, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask, DWORD dwROP) const throw()
{
	BOOL bResult;

	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );

	GetDC();

	bResult = ::MaskBlt( hDestDC, xDest, yDest, nWidth, nHeight, m_hDC, xSrc,
		ySrc, hbmMask, xMask, yMask, dwROP );

	ReleaseDC();

	return( bResult );
}

BOOL CImage::MaskBlt(HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, HBITMAP hbmMask, const POINT& pointMask, DWORD dwROP) const throw()
{
	return MaskBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, pointSrc.x, pointSrc.y,
		hbmMask, pointMask.x, pointMask.y, dwROP );
}

BOOL CImage::MaskBlt(HDC hDestDC, int xDest, int yDest, HBITMAP hbmMask, DWORD dwROP) const throw()
{
	return MaskBlt( hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, hbmMask,
		0, 0, dwROP );
}

BOOL CImage::MaskBlt(HDC hDestDC, const POINT& pointDest, HBITMAP hbmMask, DWORD dwROP) const throw()
{
	return MaskBlt( hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0,
		0, hbmMask, 0, 0, dwROP );
}

BOOL CImage::PlgBlt(HDC hDestDC, const POINT* pPoints, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, HBITMAP hbmMask, int xMask, int yMask) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );

	GetDC();

	BOOL bResult = ::PlgBlt( hDestDC, pPoints, m_hDC, xSrc, ySrc, nSrcWidth,
		nSrcHeight, hbmMask, xMask, yMask );

	ReleaseDC();

	return( bResult );
}

BOOL CImage::PlgBlt(HDC hDestDC, const POINT* pPoints, const RECT& rectSrc, HBITMAP hbmMask, const POINT& pointMask) const throw()
{
	return PlgBlt( hDestDC, pPoints, rectSrc.left, rectSrc.top, rectSrc.right-
		rectSrc.left, rectSrc.bottom-rectSrc.top, hbmMask, pointMask.x,
		pointMask.y );
}

BOOL CImage::PlgBlt(HDC hDestDC, const POINT* pPoints, HBITMAP hbmMask) const throw()
{
	return PlgBlt( hDestDC, pPoints, 0, 0, m_nWidth, m_nHeight, hbmMask, 0, 0 );
}

void CImage::ReleaseDC() const throw()
{
	HBITMAP hBitmap;

	ASSERT( m_hDC != NULL );

	m_nDCRefCount--;
	if( m_nDCRefCount == 0 )
	{
		hBitmap = HBITMAP( ::SelectObject( m_hDC, m_hOldBitmap ) );
		ASSERT( hBitmap == m_hBitmap );
		GetCDCCacheInstance()->ReleaseDC( m_hDC );
		m_hDC = NULL;
	}
}

CLSID CImage::FindCodecForExtension(LPCTSTR pszExtension, const Gdiplus::ImageCodecInfo* pCodecs, UINT nCodecs)
{
	WCHAR wszExtension[MAX_PATH] = { 0 };

#if defined(UNICODE) || defined(_UNICODE)
	lstrcpyn(wszExtension, pszExtension, MAX_PATH);
#else
	mbstowcs(wszExtension, pszExtension, MAX_PATH);
#endif

	for( UINT iCodec = 0; iCodec < nCodecs; iCodec++ )
	{
		int nLen = wcslen(pCodecs[iCodec].FilenameExtension);
		LPWSTR pwszExtensions = new WCHAR[nLen + 1];
		ZeroMemory(pwszExtensions, nLen*sizeof(WCHAR));

		lstrcpynW(pwszExtensions, pCodecs[iCodec].FilenameExtension, nLen + 1);

		LPCWSTR pwszExtension = wcstok(pwszExtensions, L";");
		while (pwszExtension)
		{
			LPCWSTR pwszExtension2 = ::PathFindExtensionW(pwszExtension);
			if( pwszExtension2 )
			{
				if( wcsicmp(pwszExtension2, wszExtension) == 0 )
				{
					return( pCodecs[iCodec].Clsid );
				}
			}
			pwszExtension = wcstok(NULL, L";");
		}
		delete []pwszExtensions;
	}

	return( CLSID_NULL );
}

CLSID CImage::FindCodecForFileType(REFGUID guidFileType, const Gdiplus::ImageCodecInfo* pCodecs, UINT nCodecs)
{
	for( UINT iCodec = 0; iCodec < nCodecs; iCodec++ )
	{
		if( pCodecs[iCodec].FormatID == guidFileType )
		{
			return( pCodecs[iCodec].Clsid );
		}
	}

	return( CLSID_NULL );
}

HRESULT CImage::Save(IStream* pStream, REFGUID guidFileType) const throw()
{
	if( !InitGDIPlus() )
	{
		return( E_FAIL );
	}

	UINT nEncoders;
	UINT nBytes;
	Gdiplus::Status status;

	status = Gdiplus::GetImageEncodersSize( &nEncoders, &nBytes );
	if( status != Gdiplus::Ok )
	{
		return( E_FAIL );
	}

	Gdiplus::ImageCodecInfo* pEncoders = reinterpret_cast< Gdiplus::ImageCodecInfo* >( new BYTE[nBytes] );

	if( pEncoders == NULL )
		return E_OUTOFMEMORY;

	status = Gdiplus::GetImageEncoders( nEncoders, nBytes, pEncoders );
	if( status != Gdiplus::Ok )
	{
		return( E_FAIL );
	}

	CLSID clsidEncoder = FindCodecForFileType( guidFileType, pEncoders, nEncoders );
	if( clsidEncoder == CLSID_NULL )
	{
		return( E_FAIL );
	}

	if( m_bHasAlphaChannel )
	{
		ASSERT( m_nBPP == 32 );
		Gdiplus::Bitmap bm( m_nWidth, m_nHeight, m_nPitch, PixelFormat32bppARGB, static_cast< BYTE* >( m_pBits ) );
		status = bm.Save( pStream, &clsidEncoder, NULL );
		if( status != Gdiplus::Ok )
		{
			return( E_FAIL );
		}
	}
	else
	{
		Gdiplus::Bitmap bm( m_hBitmap, NULL );
		status = bm.Save( pStream, &clsidEncoder, NULL );
		if( status != Gdiplus::Ok )
		{
			return( E_FAIL );
		}
	}

	delete []pEncoders;

	return( S_OK );
}

HRESULT CImage::Save(LPCTSTR pszFileName, REFGUID guidFileType) const throw()
{
	if( !InitGDIPlus() )
	{
		return( E_FAIL );
	}

	UINT nEncoders;
	UINT nBytes;
	Gdiplus::Status status;

	status = Gdiplus::GetImageEncodersSize( &nEncoders, &nBytes );
	if( status != Gdiplus::Ok )
	{
		return( E_FAIL );
	}

	Gdiplus::ImageCodecInfo* pEncoders = reinterpret_cast< Gdiplus::ImageCodecInfo* >( new BYTE[nBytes] );

	if( pEncoders == NULL )
		return E_OUTOFMEMORY;

	status = Gdiplus::GetImageEncoders( nEncoders, nBytes, pEncoders );
	if( status != Gdiplus::Ok )
	{
		return( E_FAIL );
	}

	CLSID clsidEncoder = CLSID_NULL;
	if( guidFileType == GUID_NULL )
	{
		// Determine clsid from extension
		clsidEncoder = FindCodecForExtension( ::PathFindExtension( pszFileName ), pEncoders, nEncoders );
	}
	else
	{
		// Determine clsid from file type
		clsidEncoder = FindCodecForFileType( guidFileType, pEncoders, nEncoders );
	}
	if( clsidEncoder == CLSID_NULL )
	{
		return( E_FAIL );
	}

	WCHAR wszFileName[MAX_PATH] = { 0 };
#if defined(UNICODE) || defined(_UNICODE)
	lstrcpyn(wszFileName, pszFileName, MAX_PATH);
#else
	mbstowcs(wszFileName, pszFileName, MAX_PATH);
#endif

	if( m_bHasAlphaChannel )
	{
		ASSERT( m_nBPP == 32 );
		Gdiplus::Bitmap bm( m_nWidth, m_nHeight, m_nPitch, PixelFormat32bppARGB, static_cast< BYTE* >( m_pBits ) );
		status = bm.Save( wszFileName, &clsidEncoder, NULL );
		if( status != Gdiplus::Ok )
		{
			return( E_FAIL );
		}
	}
	else
	{
		Gdiplus::Bitmap bm( m_hBitmap, NULL );
		status = bm.Save( wszFileName, &clsidEncoder, NULL );
		if( status != Gdiplus::Ok )
		{
			return( E_FAIL );
		}
	}

	delete []pEncoders;

	return( S_OK );
}

void CImage::SetColorTable(UINT iFirstColor, UINT nColors, const RGBQUAD* prgbColors) throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	ASSERT( IsIndexed() );

	GetDC();

	::SetDIBColorTable( m_hDC, iFirstColor, nColors, prgbColors );

	ReleaseDC();
}

void CImage::SetPixel(int x, int y, COLORREF color) throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );

	GetDC();

	::SetPixel( m_hDC, x, y, color );

	ReleaseDC();
}

void CImage::SetPixelIndexed(int x, int y, int iIndex) throw()
{
	SetPixel( x, y, PALETTEINDEX( iIndex ) );
}

void CImage::SetPixelRGB(int x, int y, BYTE r, BYTE g, BYTE b) throw()
{
	SetPixel( x, y, RGB( r, g, b ) );
}

 void CImage::SetHasAlphaChannel(bool bHasAlphaChannel) throw()
{
	m_bHasAlphaChannel = bHasAlphaChannel;
}

 LONG CImage::SetTransparentColor(LONG iTransparentColor) throw()
{
	LONG iOldTransparentColor;

	ASSERT( m_hBitmap != NULL );
	ASSERT( (m_nBPP == 4) || (m_nBPP == 8) );
	ASSERT( iTransparentColor < GetMaxColorTableEntries() );
	ASSERT( iTransparentColor >= -1 );

	iOldTransparentColor = m_iTransparentColor;
	m_iTransparentColor = iTransparentColor;

	return( iOldTransparentColor );
}

 COLORREF CImage::SetTransparentColor(COLORREF clrTransparentColor) throw()
{
	COLORREF clrOldTransparentColor;

	clrOldTransparentColor = m_clrTransparentColor;
	m_clrTransparentColor = clrTransparentColor;

	return( clrOldTransparentColor );
}

BOOL CImage::StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight,  DWORD dwROP) const throw()
{
	return StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 0,
		m_nWidth, m_nHeight, dwROP );
}

BOOL CImage::StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP) const throw()
{
	return StretchBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, 0, 0, m_nWidth, m_nHeight,
		dwROP );
}

BOOL CImage::StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );

	GetDC();

	BOOL bResult = ::StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC,
		xSrc, ySrc, nSrcWidth, nSrcHeight, dwROP );

	ReleaseDC();

	return( bResult );
}

BOOL CImage::StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP) const throw()
{
	return StretchBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left, rectSrc.top,
		rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top, dwROP );
}

BOOL CImage::TransparentBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, UINT crTransparent) const throw()
{
	return TransparentBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0,
		0, m_nWidth, m_nHeight, crTransparent );
}

BOOL CImage::TransparentBlt(HDC hDestDC, const RECT& rectDest, UINT crTransparent) const throw()
{
	return TransparentBlt( hDestDC, rectDest.left, rectDest.top,
		rectDest.right-rectDest.left, rectDest.bottom-rectDest.top,
		crTransparent );
}

BOOL CImage::TransparentBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, UINT crTransparent) const throw()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );

	GetDC();

	if( crTransparent == CLR_INVALID )
	{
		crTransparent = GetTransparentRGB();
	}

	BOOL bResult = ::TransparentBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight,
		m_hDC, xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent );

	ReleaseDC();

	return( bResult );
}

BOOL CImage::TransparentBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, UINT crTransparent) const throw()
{
	return TransparentBlt( hDestDC, rectDest.left, rectDest.top,
		rectDest.right-rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left,
		rectSrc.top, rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top,
		crTransparent );
}

 BOOL CImage::IsTransparencySupported() throw()
{
	return( TRUE );
}

 void CImage::UpdateBitmapInfo(DIBOrientation eOrientation)
{
	DIBSECTION dibsection;
	int nBytes;

	nBytes = ::GetObject( m_hBitmap, sizeof( DIBSECTION ), &dibsection );
	if( nBytes == sizeof( DIBSECTION ) )
	{
		m_bIsDIBSection = true;
		m_nWidth = dibsection.dsBmih.biWidth;
		m_nHeight = abs( dibsection.dsBmih.biHeight );
		m_nBPP = dibsection.dsBmih.biBitCount;
		m_nPitch = ComputePitch( m_nWidth, m_nBPP );
		m_pBits = dibsection.dsBm.bmBits;
		if( eOrientation == DIBOR_DEFAULT )
		{
			eOrientation = (dibsection.dsBmih.biHeight > 0) ? DIBOR_BOTTOMUP : DIBOR_TOPDOWN;
		}
		if( eOrientation == DIBOR_BOTTOMUP )
		{
			m_pBits = LPBYTE( m_pBits )+((m_nHeight-1)*m_nPitch);
			m_nPitch = -m_nPitch;
		}
	}
	else
	{
		// Non-DIBSection
		ASSERT( nBytes == sizeof( BITMAP ) );
		m_bIsDIBSection = false;
		m_nWidth = dibsection.dsBm.bmWidth;
		m_nHeight = dibsection.dsBm.bmHeight;
		m_nBPP = dibsection.dsBm.bmBitsPixel;
		m_nPitch = 0;
		m_pBits = 0;
	}
	m_iTransparentColor = -1;
	m_bHasAlphaChannel = false;
}

 COLORREF CImage::GetTransparentRGB() const
{
	RGBQUAD rgb;

	ASSERT( m_hDC != NULL );  // Must have a DC
	ASSERT( m_iTransparentColor != -1 || m_clrTransparentColor != (COLORREF)-1 );

	if (m_iTransparentColor != -1)
	{
		::GetDIBColorTable( m_hDC, m_iTransparentColor, 1, &rgb );
		return( RGB( rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue ) );
	}
	else
	{
		return m_clrTransparentColor;
	}
}

 bool CImage::InitGDIPlus() throw()
{
	bool bSuccess = GetInitGDIPlusInstance()->Init();
	return( bSuccess );
}
