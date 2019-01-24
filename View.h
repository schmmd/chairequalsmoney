#ifndef _H_VIEW
#define _H_VIEW

#include <windows.h>
#include "world.h"
#include "lib\other\bitmap.h"

class CView
{
public:
	CView();
	~CView();

	void Create	(HINSTANCE hInstance, CWorld * world);
	void Size	(int width, int height);
	void Update	(Canvas & canvas);
	void Paint	(Canvas & canvas);

	void SetMessage(char* szMessage)
	{
		strcpy(_szMessage, szMessage);
	}

	bool IsErrorMessage()
	{
		return strcmp(_szMessage, "") != 0;
	}

	void SetError(bool bError)
	{
		_bError = bError;
	}

	bool GetError()
	{
		return _bError;
	}

private:
	CWorld* _world;

	Bitmap _bmpBack;

	Bitmap _bmpTitle;
	Bitmap _bmpWall[3], _bmpPerson[3], _bmpPersonspot[3], _bmpMoney[3], _bmpSpot[3], _bmpChair[3];

	int _tiledim[3];

	HFONT _hfont;
	// HBRUSH  _hbrOnBar;
	// COLORREF _colorrefAlive;

	RECT _client;
	int _width, _height;

	char _szMessage[64];
	bool _bError;
};

#define LARGE	2
#define MEDIUM	1
#define SMALL	0

#endif