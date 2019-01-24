#ifndef _H_WORLD
#define _H_WORLD

#include <windows.h>
#include "list.h"
#include "game.h"
#include "roomset.h"

class CWorld
{
public:
	CWorld();

	void Click(int x, int y);
	bool KeyDown(int iVirtKey);

	bool Exists()
	{
		return _roomset.Exists();
	}

	bool FirstRoom()
	{
		if (_roomset.GoFirst())
		{
			_undo.Empty();
			_redo.Empty();
			_roomset.Current()->Copy(_game);
			return true;
		}

		return false;
	}

	bool LastRoom()
	{
		if (_roomset.GoLast())
		{
			_undo.Empty();
			_redo.Empty();
			_roomset.Current()->Copy(_game);
			return true;
		}

		return false;
	}

	bool NextRoom()
	{
		if (_roomset.GoNext())
		{
			_undo.Empty();
			_redo.Empty();
			_roomset.Current()->Copy(_game);
			return true;
		}

		return false;
	}

	bool PrevRoom()
	{
		if (_roomset.GoPrev())
		{
			_undo.Empty();
			_redo.Empty();
			_roomset.Current()->Copy(_game);
			return true;
		}

		return false;
	}

	bool Restart()
	{
		if (Exists())
		{
			_undo.Empty();
			_redo.Empty();
			_roomset.Current()->Copy(_game);
			return true;
		}
		else
			return false;
	}

	bool Highscore();

	char Undo();
	char Redo();
	bool UndoPush();
	bool RedoPush();

	bool SaveState();
	bool SaveHighscore();
	bool LoadHighRecord();

	CGame _game;
	CRoomset _roomset;

	list<char> _undo;
	list<char> _redo;
};

#endif