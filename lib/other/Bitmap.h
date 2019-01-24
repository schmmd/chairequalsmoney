#if !defined (BITMAP_H)
#define BITMAP_H
//-------------------------------------
// Copyright Reliable Software (c) 1999
//-------------------------------------
#include "WinEx.h"
#include <windows.h>
#include "canvas.h"
// class Canvas;

class Bitmap
{
public:
    Bitmap ()
        : _hBitmap (0)
    {}
    Bitmap (Canvas & canvas, int dx, int dy)
        : _hBitmap (0)
    {
        CreateCompatible (canvas, dx, dy);
    }
    // Transfer semantics
    Bitmap (Bitmap & bmp)
        : _hBitmap (bmp.Release ())
    {}
    void operator = (Bitmap & bmp)
    {
        Free ();
        _hBitmap = bmp.Release ();
    }
    HBITMAP Release ()
    {
        HBITMAP h = _hBitmap;
        _hBitmap = 0;
        return h;
    }
    ~Bitmap ()
    {
        Free ();
    }
    // implicit conversion for use with Windows API
    operator HBITMAP () { return _hBitmap; }
    void CreateCompatible (Canvas & canvas, int width, int height);
    // Load from resources
    void Load (HINSTANCE hInst, char const * resName)
    {
        Free ();
        _hBitmap = (HBITMAP) ::LoadImage (hInst, 
            resName,
            IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
        if (_hBitmap == 0)
            throw WinException ("Cannot load bitmap from resources", resName);
    }
    void Load (HINSTANCE hInst, int id)
    {
        Free ();
        _hBitmap = (HBITMAP) ::LoadImage (hInst, 
            MAKEINTRESOURCE (id),
            IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
        if (_hBitmap == 0)
            throw WinException ("Cannot load bitmap from resources");
    }
    // Load from file
    void Load (char * path)
    {
        Free ();
        _hBitmap = (HBITMAP) ::LoadImage (0, path, 
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (_hBitmap == 0)
            throw WinException ("Cannot load bitmap from file", path);
    }
	void Save (char * path, HWND hwnd, int width, int height)
	{
		HDC hdc = CreateCompatibleDC (GetDC(hwnd));
		PBITMAPINFO bitmapinfo = CreateBitmapInfoStruct(hwnd, _hBitmap);
		CreateBMPFile(hwnd, path, bitmapinfo, _hBitmap, hdc);
		GlobalFree(bitmapinfo);
	}

	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
	{ 
		BITMAP bmp; 
		PBITMAPINFO pbmi; 
		WORD    cClrBits; 

		// Retrieve the bitmap color format, width, and height. 
		if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
			return 0;
			//errhandler("GetObject", hwnd); 

		// Convert the color format to a count of bits. 
		cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
		if (cClrBits == 1) 
			cClrBits = 1; 
		else if (cClrBits <= 4) 
			cClrBits = 4; 
		else if (cClrBits <= 8) 
			cClrBits = 8; 
		else if (cClrBits <= 16) 
			cClrBits = 16; 
		else if (cClrBits <= 24) 
			cClrBits = 24; 
		else cClrBits = 32; 

		// Allocate memory for the BITMAPINFO structure. (This structure 
		// contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
		// data structures.) 

		 if (cClrBits != 24) 
			 pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
						sizeof(BITMAPINFOHEADER) + 
						sizeof(RGBQUAD) * (1<< cClrBits)); 

		 // There is no RGBQUAD array for the 24-bit-per-pixel format. 

		 else 
			 pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
						sizeof(BITMAPINFOHEADER)); 

		// Initialize the fields in the BITMAPINFO structure. 

		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
		pbmi->bmiHeader.biWidth = bmp.bmWidth; 
		pbmi->bmiHeader.biHeight = bmp.bmHeight; 
		pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
		pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
		if (cClrBits < 24) 
			pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

		// If the bitmap is not compressed, set the BI_RGB flag. 
		pbmi->bmiHeader.biCompression = BI_RGB; 

		// Compute the number of bytes in the array of color 
		// indices and store the result in biSizeImage. 
		// For Windows NT, the width must be DWORD aligned unless 
		// the bitmap is RLE compressed. This example shows this. 
		// For Windows 95/98/Me, the width must be WORD aligned unless the 
		// bitmap is RLE compressed.
		pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
									  * pbmi->bmiHeader.biHeight; 
		// Set biClrImportant to 0, indicating that all of the 
		// device colors are important. 
		 pbmi->bmiHeader.biClrImportant = 0; 
		 return pbmi; 
	} 
	void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, 
					  HBITMAP hBMP, HDC hDC) 
	{ 
		 HANDLE hf;                 // file handle 
		BITMAPFILEHEADER hdr;       // bitmap file-header 
		PBITMAPINFOHEADER pbih;     // bitmap info-header 
		LPBYTE lpBits;              // memory pointer 
		DWORD dwTotal;              // total count of bytes 
		DWORD cb;                   // incremental count of bytes 
		BYTE *hp;                   // byte pointer 
		DWORD dwTmp; 

		pbih = (PBITMAPINFOHEADER) pbi; 
		lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

		if (!lpBits) 
			return;
			 // errhandler("GlobalAlloc", hwnd); 

		// Retrieve the color table (RGBQUAD array) and the bits 
		// (array of palette indices) from the DIB. 
		if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
			DIB_RGB_COLORS)) 
		{
			return;
			// errhandler("GetDIBits", hwnd); 
		}

		// Create the .BMP file. 
		hf = CreateFile(pszFile, 
					   GENERIC_READ | GENERIC_WRITE, 
					   (DWORD) 0, 
						NULL, 
					   CREATE_ALWAYS, 
					   FILE_ATTRIBUTE_NORMAL, 
					   (HANDLE) NULL); 
		if (hf == INVALID_HANDLE_VALUE) 
			return;
			// errhandler("CreateFile", hwnd); 
		hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
		// Compute the size of the entire file. 
		hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
					 pbih->biSize + pbih->biClrUsed 
					 * sizeof(RGBQUAD) + pbih->biSizeImage); 
		hdr.bfReserved1 = 0; 
		hdr.bfReserved2 = 0; 

		// Compute the offset to the array of color indices. 
		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
						pbih->biSize + pbih->biClrUsed 
						* sizeof (RGBQUAD); 

		// Copy the BITMAPFILEHEADER into the .BMP file. 
		if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
			(LPDWORD) &dwTmp,  NULL)) 
		{
			return;
		   // errhandler("WriteFile", hwnd); 
		}

		// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
		if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
					  + pbih->biClrUsed * sizeof (RGBQUAD), 
					  (LPDWORD) &dwTmp, ( NULL)) )
			return;
			// errhandler("WriteFile", hwnd); 

		// Copy the array of color indices into the .BMP file. 
		dwTotal = cb = pbih->biSizeImage; 
		hp = lpBits; 
		if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
			return;
			   // errhandler("WriteFile", hwnd); 

		// Close the .BMP file. 
		 if (!CloseHandle(hf)) 
			 return;
			   // errhandler("CloseHandle", hwnd); 

		// Free memory. 
		GlobalFree((HGLOBAL)lpBits);
	}
    void GetSize (int &width, int &height)
    {
        BITMAP bm;
        ::GetObject (_hBitmap, sizeof (bm), &bm);
        width = bm.bmWidth;
        height = bm.bmHeight;
    }
protected:
    Bitmap (HBITMAP hBitmap)
        : _hBitmap (hBitmap)
    {}
    void Free ()
    {
        if (_hBitmap) 
            ::DeleteObject (_hBitmap);
    }

    HBITMAP _hBitmap;
};

// Used for bit blitting
class Blitter
{
public:
    Blitter (Bitmap & bmp)
        : _bmp (bmp), _mode (SRCCOPY),
          _xDst (0), _yDst (0),
          _xSrc (0), _ySrc (0)
    {
        bmp.GetSize (_widthDst, _heightDst);
		bmp.GetSize (_widthSrc, _heightSrc);
    }
    void SetMode (DWORD mode)
    {
        _mode = mode;
    }
    void SetDest (int x, int y)
    {
        _xDst = x;
        _yDst = y;
    }
    void SetSrc (int x, int y)
    {
        _xSrc = x;
        _ySrc = y;
    }
    void SetSrcArea (int width, int height)
    {
        _widthSrc = width;
        _heightSrc = height;
    }
    void SetDstArea (int width, int height)
    {
        _widthDst = width;
        _heightDst = height;
    }

    // transfer bitmap to canvas
    void BlitTo (Canvas & canvas);

private:
    Bitmap & _bmp;
    int     _xDst, _yDst;
    int     _xSrc, _ySrc;
	int     _widthDst, _heightDst;
    int     _widthSrc, _heightSrc;
    DWORD   _mode;
};

#endif
