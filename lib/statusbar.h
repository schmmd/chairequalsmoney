#ifndef _H_STATUSBAR
#define _H_STATUSBAR

#include <windows.h>
#include <commctrl.h>

class CStatusBar
{
public:
	void Create(HWND hwndParent)
	{
		// InitCommonControls();

		_hwnd = CreateWindow(STATUSCLASSNAME, "", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwndParent, NULL, GetModuleHandle(NULL), NULL);
	}

	void SetParts(int numparts, int* parts)
	{
		SendMessage(_hwnd, SB_SETPARTS, (WPARAM) numparts, (LPARAM) parts);
	}

	void SetPartText(int part, char* text)
	{
		SendMessage(_hwnd, SB_SETTEXT, (WPARAM) part, (LPARAM) text);
	}

	void Size(int width, int height)
	{
		SendMessage(_hwnd, WM_SIZE, (WPARAM) width, (LPARAM) height);
	}

	HWND GetHwnd()
	{
		return _hwnd;
	}

private:
	HWND _hwnd;
};

#endif