#include "procedure.h"
#include "dlgcommon.h"
#include "dlgsimple.h"
#include "lib\other\canvas.h"
#include "resource.h"
#include "main.h"

BOOL CALLBACK SimpleDlgProc(HWND hwnd, UINT message, UINT wParam, LPARAM lParam);

#define PLY_HISTORY 1
#define PLY_REDOS	2

void CProcedure::Create(CREATESTRUCT* create)
{
	_world = new CWorld;
	_view.Create(create->hInstance, _world);

	_hmenu = GetMenu(_hwnd);

	_nextlevelonwin.Set(create->hMenu, ID_GAME_NEXTLEVELONWIN);

	_timerPlay.Create(_hwnd, 1);
	_timerNext.Create(_hwnd, 2);
}

bool CProcedure::Paint(HDC hdc)
{
	PaintCanvas canvas(_hwnd);
	_view.Paint(canvas);

	return true;
}

bool CProcedure::Size(int width, int height)
{
	_view.Size(width, height);
	UpdateAndPaint();

	return true;
}

bool CProcedure::LeftClick (int x, int y)
{ 
	if (_world->_roomset.IsEmpty() && !_view.IsErrorMessage())
	{
		SendMessage(_hwnd, WM_COMMAND, IDM_FILE_OPEN, 0);
		return true;
	}

	return false;
}

bool CProcedure::KeyDown (int iVirtKey)
{
	if (_world->KeyDown(iVirtKey))
	{
		_timerPlay.Kill();

		if (_world->_game.IsCompleted())
		{
			if (_world->Highscore())
			{
				if (_world->_roomset.GetFilterindex() != 3)
					_world->SaveHighscore();


				_view.SetMessage("New Record!");
			}
			if (_nextlevelonwin.GetCheck())
			{
				_timerNext.Set(1400);
			}
		}

		UpdateAndPaint();
		EnableUndoMenus(MF_ENABLED);
		EnableRedoMenus(MF_GRAYED);
	}

	return true;
}

bool CProcedure::Timer (int id, TIMERPROC * proc)
{
	if (id == 1)
	{
		if (_world->Redo())
		{
			EnableUndoMenus(MF_ENABLED);
			_timerPlay.Set(250);
			UpdateAndPaint();
		}
		else
		{
			EnableMenuItem(_hmenu, ID_GAME_STOP, MF_GRAYED);
			EnableRedoMenus(MF_GRAYED);

			_timerPlay.Kill();
		}
	}
	else if (id == 2)
	{
		_world->NextRoom();
		EnableUndoMenus(MF_GRAYED);
		EnableRedoMenus(MF_GRAYED);
		UpdateNavMenus();
		UpdateAndPaint();

		_timerNext.Kill();
	}

	return true;
}

bool CProcedure::MenuCommand (int cmdId, bool isAccel)
{
	int iFilterIndex, iTemp, nExt;
	CSimpleDlg dlgSimple;
	char szFile[512], szTemp[512];
	char szFilename[256];

	if(_timerPlay.Kill())
		EnableMenuItem(_hmenu, ID_GAME_STOP, MF_GRAYED);
	_timerNext.Kill();

	switch(cmdId)
	{
	case IDM_FILE_OPEN:
		if (_world->Exists())
			_world->SaveState();

		iFilterIndex = 1;
		sprintf(szFile, "C:\\Games\\Chair\\duthen (easy).xsb");
		if (OpenDlg(_hwnd, "Sokoban Level File (*.xsb; *.txt)\0*.xsb;*.txt\0Universal Bookmark File (*.ubk)\0*.ubk\0", iFilterIndex, szFile, 512, szFilename, 256, nExt))
		{
			// Find correct filter index
			iFilterIndex = GetFilterIndex(szFile + nExt, iFilterIndex);

			// Update roomset information
			_world->_roomset.SetFile(szFile);
			_world->_roomset.SetFilename(szFilename);
			_world->_roomset.SetFilterindex(iFilterIndex);

			// Open the file
			Open();

			// Update window caption (title bar)
			UpdateWindowCaption();
		}
		
		return true;

	case IDM_FILE_SAVEAS:
		iFilterIndex = 1;
		strcpy(szFile, _world->_roomset.GetFile());
		if (SaveAsDlg(_hwnd, "Multi-room Sokoban Level File (*.xsb)\0*.xsb\0Single-room Sokoban Level File (*.xsb)\0*.xsb\0Universal Bookmark File (*.ubk)\0*.ubk\0", iFilterIndex, "xsb", szFile, 512, szFilename, 256, nExt))
		{
			// Make sure the filter index is correct
			iFilterIndex = GetFilterIndex(szFile + nExt, iFilterIndex);

			// Update roomset information
			_world->_roomset.SetFile(szFile);
			_world->_roomset.SetFilename(szFilename);
			_world->_roomset.SetFilterindex(iFilterIndex);

			Save();
			Open();

			// Update window caption (title bar)
			UpdateWindowCaption();
		}
		return true;

	case IDM_FILE_SAVECOPY:
		iFilterIndex = 1;
		strcpy(szFile, _world->_roomset.GetFile());
		if (SaveAsDlg(_hwnd, "Multi-room Sokoban Level File (*.xsb)\0*.xsb\0Single-room Sokoban Level File (*.xsb)\0*.xsb\0Universal Bookmark File (*.ubk)\0*.ubk\0", iFilterIndex, "xsb", szFile, 512, szFilename, 256, nExt))
		{
			iFilterIndex = GetFilterIndex(szFile + nExt, iFilterIndex);

			iTemp = iFilterIndex;
			strcpy(szTemp, szFile);

			_world->_roomset.SetFile(szFile);
			_world->_roomset.SetFilterindex(iFilterIndex);

			Save();

			_world->_roomset.SetFile(szTemp);
			_world->_roomset.SetFilterindex(iTemp);
		}
		return true;

	case ID_HELP_DEMO:
		sprintf(szFilename, "demo.ubk");
		AddLocalPath(szFile, 512, szFilename);

		_world->_roomset.SetFile	(szFile);
		_world->_roomset.SetFilename(szFilename);
		_world->_roomset.SetFilterindex	(3);

		if (Open())
		{
			UpdateWindowCaption();
			SendMessage(_hwnd, WM_COMMAND, ID_GAME_REPLAY, 0);
		}
		else
		{
			MessageBox(_hwnd, "Error: The demo file could not be opened.", "Chair Equals Money", MB_ICONERROR);
		}
		return true;

	case ID_FILE_EXIT:
		Exit();
		return true;

	case ID_GAME_NEXTLEVELONWIN:
		_nextlevelonwin.Check(!_nextlevelonwin.GetCheck());
		return true;

	case ID_GAME_FIRST:
		if (_world->FirstRoom())
		{
			UpdateNavMenus();
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_LAST:
		if (_world->LastRoom())
		{
			UpdateNavMenus();
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_NEXT:
		if (_world->NextRoom())
		{
			UpdateNavMenus();
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_PREV:
		if (_world->PrevRoom())
		{
			UpdateNavMenus();
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_RESTART:
		if (_world->Restart())
		{
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_UNDO:
		if (_world->Undo())
		{
			EnableRedoMenus(MF_ENABLED);
			if (_world->_undo.IsEmpty())
				EnableUndoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_UNDOPUSH:
		if (_world->UndoPush())
		{
			EnableRedoMenus(MF_ENABLED);
			if (_world->_undo.IsEmpty())
				EnableUndoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_UNDOALL:
		if (_world->Undo())
		{
			while(_world->Undo());
			EnableRedoMenus(MF_ENABLED);
			EnableUndoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_REDO:
		if (_world->Redo())
			EnableUndoMenus(MF_ENABLED);
			if (_world->_redo.IsEmpty())
				EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		return true;

	case ID_GAME_REDOPUSH:
		if (_world->RedoPush())
		{
			EnableUndoMenus(MF_ENABLED);
			if (_world->_redo.IsEmpty())
				EnableRedoMenus(MF_GRAYED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_REDOALL:
		if (_world->Redo())
		{
			while(_world->Redo());
			EnableRedoMenus(MF_GRAYED);
			EnableUndoMenus(MF_ENABLED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_REPLAY:
		if (_world->Undo())
		{
			while(_world->Undo());
			_timerPlay.Set(800);
			EnableMenuItem(_hmenu, ID_GAME_STOP, MF_ENABLED);
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_ENABLED);
			UpdateAndPaint();
		}
		return true;

	case ID_GAME_REPLAYHIGH:
		if (_world->LoadHighRecord())
		{
			_timerPlay.Set(800);
			EnableMenuItem(_hmenu, ID_GAME_STOP, MF_ENABLED);
			EnableUndoMenus(MF_GRAYED);
			EnableRedoMenus(MF_ENABLED);
			UpdateAndPaint();
		}
		return true;

	case ID_HELP_INSTRUCTIONS:
		AddLocalPath(szFile, 512, "help.chm");
		ShellExecute(_hwnd, "Open", szFile, "", "", SW_SHOWNORMAL);
		/*
		if (!dlgSimple.Run(GetModuleHandle(NULL), _hwnd, IDD_INSTRUCTIONS))
			SendMessage(_hwnd, WM_COMMAND, ID_FILE_DEMO, 0);
		*/
		return true;

	case ID_HELP_SCHMITZTECH:
		ShellExecute(_hwnd, "Open", "http://www.schmitztech.com/", "", "", SW_SHOWNORMAL);
		return true;

	case ID_HELP_ABOUT:
		dlgSimple.Run(GetModuleHandle(NULL), _hwnd, IDD_ABOUT);
		return true;

	default:
		return false;
	}
}

void CProcedure::UpdateAndPaint()
{
	UpdateCanvas canvas(_hwnd);
	_view.Update(canvas);
	InvalidateRect(_hwnd, NULL, 0);
}

void CProcedure::UpdateNavMenus()
{
	CRoomset* roomset = &_world->_roomset;

	UINT uEnable;

	if (roomset->IsFirst())
		uEnable = MF_GRAYED;
	else
		uEnable = MF_ENABLED;
	EnableMenuItem(_hmenu, ID_GAME_FIRST, uEnable);

	if (roomset->IsLast())
		uEnable = MF_GRAYED;
	else
		uEnable = MF_ENABLED;
	EnableMenuItem(_hmenu, ID_GAME_LAST, uEnable);

	if (roomset->IsNext())
		uEnable = MF_ENABLED;
	else
		uEnable = MF_GRAYED;
	EnableMenuItem(_hmenu, ID_GAME_NEXT, uEnable);

	if (roomset->IsPrev())
		uEnable = MF_ENABLED;
	else
		uEnable = MF_GRAYED;
	EnableMenuItem(_hmenu, ID_GAME_PREV, uEnable);

	if (roomset->Current()->GetHighpush() != 0)
		uEnable = MF_ENABLED;
	else
		uEnable = MF_GRAYED;
	EnableMenuItem(_hmenu, ID_GAME_REPLAYHIGH, uEnable);

}

void CProcedure::UpdateUndoRedoMenus()
{
	if (_world->_undo.IsEmpty())
		EnableUndoMenus(MF_GRAYED);
	else
		EnableUndoMenus(MF_ENABLED);

	if (_world->_redo.IsEmpty())
		EnableRedoMenus(MF_GRAYED);
	else
		EnableRedoMenus(MF_ENABLED);
}

void CProcedure::EnableSaveMenus(UINT uEnable)
{
	EnableMenuItem(_hmenu, IDM_FILE_SAVE, uEnable);
	EnableMenuItem(_hmenu, IDM_FILE_SAVEAS, uEnable);
	EnableMenuItem(_hmenu, IDM_FILE_SAVECOPY, uEnable);
}

void CProcedure::EnableGameMenus(UINT uEnable)
{
	EnableMenuItem(_hmenu, ID_GAME_FIRST, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_LAST, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_NEXT, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_PREV, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_RESTART, uEnable);
}

void CProcedure::EnableUndoMenus(UINT uEnable)
{
	EnableMenuItem(_hmenu, ID_GAME_UNDO, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_UNDOPUSH, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_UNDOALL, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_REPLAY, uEnable);
}

void CProcedure::EnableRedoMenus(UINT uEnable)
{
	EnableMenuItem(_hmenu, ID_GAME_REDO, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_REDOPUSH, uEnable);
	EnableMenuItem(_hmenu, ID_GAME_REDOALL, uEnable);
}