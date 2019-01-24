#ifndef _H_PROC
#define _H_PROC

#include "lib\app.h"
#include "lib\timer.h"
#include "lib\menuitem.h"
#include "view.h"
#include "world.h"
#include "main.h"

#include <windows.h>

//
// Override some message handlers
//

class CGenericProc
{
	friend LRESULT CALLBACK CClass::Procedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetWindowHandle (HWND hwnd) { _hwnd = hwnd; }
public:
	virtual void Create (CREATESTRUCT * create) {}
	virtual bool Destroy () { return false; }
	virtual bool SysCommand (int cmd) { return false; }
	virtual bool Command (WORD wNotifyCode, WORD wID, HWND hwndCtrl) { return false; }
	virtual bool Size (int width, int height) { return false; }
	virtual bool Timer (int id, TIMERPROC * proc) { return false; }
	virtual bool MenuCommand (int cmdId, bool isAccel) { return false; }
	virtual bool CtrlCommand (int notifCode, int ctrlId, HWND hwnd) { return false; }
	virtual bool Paint (HDC hdc) { return false; }
	virtual bool LeftClick (int x, int y) { return false; }
	virtual bool KeyDown (int iVirtKey) { return false; }
protected:
	HWND _hwnd;
};

class CProcedure: public CGenericProc
{
public:
	CProcedure () 
		: _world (0)
	{}

	void Create (CREATESTRUCT * create);
	// bool SysCommand (int cmd);
	bool MenuCommand (int cmdId, bool isAccel);
	bool Destroy () { ::PostQuitMessage (0); return true; }
	bool Size (int width, int height);
	bool LeftClick(int x, int y);
	bool Paint (HDC hdc);
	bool KeyDown (int iVirtKey); 
	bool Timer (int id, TIMERPROC * proc);

	void Exit () { ::DestroyWindow(_hwnd); }

	void UpdateAndPaint();

	void SaveState()
	{
		if (_world->Exists())
		{
			int filterindex = _world->_roomset.GetFilterindex();
			if (filterindex == 1 || filterindex == 2)
				_world->SaveState();
		}
	}

	void Save()
	{
		if (_world->Exists())
		{
			int iFilterIndex = _world->_roomset.GetFilterindex();

			if (iFilterIndex == 1)
				_world->_roomset.Save(true);
			else if (iFilterIndex == 2)
				_world->_roomset.Save(false);
			else if (iFilterIndex == 3)
				_world->_roomset.SaveBookmark(_world->_undo);
		}
	}

	bool Open()
	{
		int iFilterIndex = _world->_roomset.GetFilterindex();
		_world->_undo.Empty();
		_world->_redo.Empty();

		// Load the file and check for errors
		if (((iFilterIndex == 1 || iFilterIndex == 2) && !_world->_roomset.Load()) || iFilterIndex == 3 && !_world->_roomset.LoadBookmark(_world->_redo))
		{
			_view.SetMessage("Illegal File Format");
			_view.SetError(true);
			UpdateAndPaint();
			return false;
		}
		else
		{
			_view.SetMessage("");
			_view.SetError(false);
		}

		// If it is an XSB
		if (iFilterIndex == 1 || iFilterIndex == 2)
		{
			// Load the highscores and the saved state
			if (!_world->_roomset.LoadHighscores(_world->_redo))
			{
				// There is an error. Abort load or delete highscores?
				if(IDYES == MessageBox(NULL, "The highscore file contains solutions which do not work with the stored rooms. Would you like to load the file and delete the highscores?", "Chair Equals Money", MB_YESNO | MB_ICONQUESTION))
				{
					// Delte Highscores
					_world->_roomset.DeleteHighscores();
				}
				else
				{
					// Abort load
					_world->_roomset.Free();
					UpdateAndPaint();
					return false;
				}
			}
		}

		// Copy the current room to the game
		_world->_roomset.Current()->Copy(_world->_game);

		// Return to the saved state (replay history)
		if (_world->Redo())
		{
			while(_world->Redo());
			EnableRedoMenus(MF_GRAYED);
			EnableUndoMenus(MF_ENABLED);
		}

		// Update menus
		EnableSaveMenus(MF_ENABLED);
		EnableGameMenus(MF_ENABLED);
		UpdateNavMenus();

		// Update display
		UpdateAndPaint();

		return true;
	}

	void UpdateNavMenus();
	void UpdateUndoRedoMenus();

	void EnableSaveMenus(UINT uEnable);
	void EnableGameMenus(UINT uEnable);
	void EnableUndoMenus(UINT uEnable);
	void EnableRedoMenus(UINT uEnable);

	void UpdateWindowCaption()
	{
		char szBuffer[256];
		sprintf(szBuffer, "Chair Equals Money - [%s]", _world->_roomset.GetFilename());
		SendMessage(_hwnd, WM_SETTEXT, (WPARAM) 0, (LPARAM) szBuffer);
	}

	int GetFilterIndex(char* szExt, int iFilterIndex)
	{
		if (stricmp(szExt, "ubk") == 0)
			return 3;
		else
		{
			if (iFilterIndex == 2)
				return 2;
			else
				return 1;
		}

		return iFilterIndex;
	}

private:

	CWorld*		_world;
	CView		_view;

	HMENU _hmenu;

	CTimer _timerPlay;
	CTimer _timerNext;

	CCheckMenuItem _nextlevelonwin;
};

#endif