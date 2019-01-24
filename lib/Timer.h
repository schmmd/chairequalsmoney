#if !defined (TIMER_H)
#define TIMER_H
//-------------------------------------
// Copyright Reliable Software (c) 1999
//-------------------------------------

#include <windows.h>

class CTimer
{
public:
	CTimer (HWND hwnd = 0, int id = -1) 
		: _hwnd (hwnd), _id (id)
	{}

	void Create (HWND hwnd, int id)
	{
		_hwnd = hwnd;
		_id = id;
	}

	void Set (int milliSec)
	{
		SetTimer(_hwnd, _id, milliSec, NULL);
		// _hwnd.SetTimer (_id, milliSec);
	}

	bool Kill ()
	{
		return KillTimer(_hwnd, _id) == 0 ? true : false;
		// _hwnd.KillTimer (_id);
	}
	int  GetId () const { return _id; }
private:
	HWND	_hwnd;
	int		_id;
};

#endif