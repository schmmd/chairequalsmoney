#include "view.h"
#include "lib\other\canvas.h"
#include "main.h"
#include "resource.h"

#include <stdio.h>

CView::CView()
: _bError(false)
{
	strcpy(_szMessage, "");

	_tiledim[SMALL]	= 16;
	_tiledim[MEDIUM]= 24;
	_tiledim[LARGE]	= 32;
}

CView::~CView()
{

}

void CView::Create	(HINSTANCE hInstance, CWorld * world)
{
	_world = world;

	// Load Bitmaps
	_bmpTitle		.Load (hInstance, IDB_TITLE);

	_bmpWall		[LARGE]	.Load (hInstance, IDB_LWALL);
	_bmpPerson		[LARGE]	.Load (hInstance, IDB_LPERSON);
	_bmpPersonspot	[LARGE]	.Load (hInstance, IDB_LPERSONSPOT);
	_bmpMoney		[LARGE]	.Load (hInstance, IDB_LMONEY);
	_bmpSpot		[LARGE]	.Load (hInstance, IDB_LSPOT);
	_bmpChair		[LARGE]	.Load (hInstance, IDB_LCHAIR);

	_bmpWall		[MEDIUM]	.Load (hInstance, IDB_MWALL);
	_bmpPerson		[MEDIUM]	.Load (hInstance, IDB_MPERSON);
	_bmpPersonspot	[MEDIUM]	.Load (hInstance, IDB_MPERSONSPOT);
	_bmpMoney		[MEDIUM]	.Load (hInstance, IDB_MMONEY);
	_bmpSpot		[MEDIUM]	.Load (hInstance, IDB_MSPOT);
	_bmpChair		[MEDIUM]	.Load (hInstance, IDB_MCHAIR);

	_bmpWall		[SMALL]	.Load (hInstance, IDB_SWALL);
	_bmpPerson		[SMALL]	.Load (hInstance, IDB_SPERSON);
	_bmpPersonspot	[SMALL]	.Load (hInstance, IDB_SPERSONSPOT);
	_bmpMoney		[SMALL]	.Load (hInstance, IDB_SMONEY);
	_bmpSpot		[SMALL]	.Load (hInstance, IDB_SSPOT);
	_bmpChair		[SMALL]	.Load (hInstance, IDB_SCHAIR);

	// Create fonts
	_hfont  = CreateFont(16, 0, 0, 0, 1000, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, "Ariel");

	// Create colors
	// _colorrefAlive = RGB(000, 000, 000);
	// _colorrefDead  = RGB(150, 150, 150);

	// Create brushes
	// _hbrOnBar  = CreateSolidBrush(RGB(000, 175, 190));
	// _hbrOffBar = CreateSolidBrush(RGB(000, 065, 071));
}

void CView::Size	(int width, int height)
{
	_width  = width;
	_height = height;

	_client.left   = 0;
	_client.right  = width;
	_client.top    = 0;
	_client.bottom = height;
}

void CView::Update	(Canvas & canvas)
{
	if (_width == 0 || _height == 0)
		return;

    Bitmap bmp (canvas, _width, _height);
    BitmapCanvas bmpCanvas (canvas, bmp);

	bmpCanvas.WhiteWash(_client);

	if (_bError)
	{
		int width = bmpCanvas.GetTextWidth(_szMessage);
		bmpCanvas.Text((_client.right - width) / 2, 10, _szMessage, strlen(_szMessage));
	}
	else if (_world->_roomset.IsEmpty())
	{
		int width, height;
		_bmpTitle.GetSize(width, height);

		Blitter blt(_bmpTitle);
		blt.SetDest((_client.right - width) / 2, (_client.bottom - height) / 2);
		blt.BlitTo(bmpCanvas);
	}
	else
	{
		int textwidth, textheight;
		bmpCanvas.GetTextSize(textwidth, textheight);

		// Find the largest tilesize possible
		int tilesize = SMALL;
		int width = _world->_game.Width(), height = _world->_game.Height();
		for (int i = 2; i >= 0; i--)
		{
			if (width * _tiledim[i] <= _client.right && height * _tiledim[i] <= _client.bottom - textheight * 5)
			{
				tilesize = i;
				break;
			}
		}

		// Solve for game dimentions
		RECT game;
		game.top    = _client.top + textheight * 3;
		game.left   = (_client.right - width * _tiledim[tilesize]) / 2;
		game.right  = _client.right - _client.left;
		game.bottom = game.top + height * _tiledim[tilesize];

		// Blit board to screen
		for (int y = 0; y < _world->_game.Height(); y++)
		{
			for (int x = 0; x < _world->_game.Width(); x++)
			{
				char tile = _world->_game.Get(x, y);

				Bitmap* bmp = 0;
				switch(tile)
				{
				case '#':
					bmp = &_bmpWall[tilesize];
					break;
				case '@':
					bmp = &_bmpPerson[tilesize];
					break;
				case '+':
					bmp = &_bmpPersonspot[tilesize];
					break;
				case '$':
					bmp = &_bmpChair[tilesize];
					break;
				case '*':
					bmp = &_bmpMoney[tilesize];
					break;
				case '.':
					bmp = &_bmpSpot[tilesize];
					break;
				}

				if (bmp != NULL)
				{
					Blitter blt(*bmp);
					blt.SetDest(game.left + x * _tiledim[tilesize], game.top + y * _tiledim[tilesize]);
					blt.BlitTo(bmpCanvas);
				}
			}
		}

		// Write text
		char szBuffer[128];

		y = _client.top;
		if (_world->_game.IsCompleted())
		{
			if (_world->_game.IsHighscore())
				sprintf(szBuffer, "%s Record!", _world->_game.GetTitle());
			else
				sprintf(szBuffer, "%s Completed", _world->_game.GetTitle());
		}
		else
			sprintf(szBuffer, "%s by %s", _world->_game.GetTitle(), _world->_game.GetAuthor());

		bmpCanvas.Text((_client.right - bmpCanvas.GetTextWidth(szBuffer)) / 2, y, szBuffer, strlen(szBuffer));

		y += textheight;

		int filterindex = _world->_roomset.GetFilterindex();
		if (filterindex == 1 || filterindex == 2)
		{
			sprintf(szBuffer, "Record: %i steps and %i pushes", _world->_game.GetHighstep(), _world->_game.GetHighpush());
			bmpCanvas.Text((_client.right - bmpCanvas.GetTextWidth(szBuffer)) / 2, y, szBuffer, strlen(szBuffer));
		}

		y = game.bottom + textheight;
		sprintf(szBuffer, "Chairs: %.2i    Steps: %.3i    Pushes: %.3i", _world->_game.GetChairs(), _world->_game.GetSteps(), _world->_game.GetPushes());
		bmpCanvas.Text((_client.right - bmpCanvas.GetTextWidth(szBuffer)) / 2, y, szBuffer, strlen(szBuffer));
	}

	_bmpBack = bmp;
}

void CView::Paint	(Canvas & canvas)
{
	if (_width == 0 || _height == 0)
		return;

	Blitter bltBack(_bmpBack);
	bltBack.BlitTo (canvas);
}