#include "Bitmap.h"
#include "Canvas.h"

void Bitmap::CreateCompatible (Canvas & canvas, int width, int height)
{
    Free ();
    _hBitmap = ::CreateCompatibleBitmap (canvas, width, height);
}

void Blitter::BlitTo (Canvas & canvas)
{
    // Create canvas in memory using target canvas as template
    MemCanvas memCanvas (canvas);
    // Convert bitmap to the format appropriate for this canvas
    memCanvas.SelectObject (_bmp);
    // Transfer bitmap from memory canvas to target canvas
	::StretchBlt(canvas, _xDst, _yDst, _widthDst, _heightDst, 
		memCanvas, _xSrc, _ySrc, _widthSrc, _heightSrc, _mode);
	/*
    ::BitBlt (canvas, _xDst, _yDst, _width, _height, 
              memCanvas, _xSrc, _ySrc, _mode);
	*/
}
