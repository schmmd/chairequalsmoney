#ifndef _H_ROOM
#define _H_ROOM

#include "board.h"

#include <string.h>
#include <windows.h>

class CGame;

class CRoom : public CBoard
{
public:
	CRoom()
		: _highscore(false), _highpush(0), _highstep(0), _chairs(0), _step(0), _push(0)
	{
		_player.x = -1;
		_player.y = -1;

		strcpy(_szTitle,  "Unknown");
		strcpy(_szAuthor, "Anonymous");
	}

	void Copy(CGame& game);
	void Copy(CRoom& room);

	void SetTitle(const char* szTitle)
	{
		if (strlen(szTitle) < 64)
			strcpy(_szTitle, szTitle);
	}

	void SetAuthor(const char* szAuthor)
	{
		if (strlen(szAuthor) < 64)
			strcpy(_szAuthor, szAuthor);
	}

	void SetHighstep(long newstep)
	{
		_highstep = newstep;
	}

	void SetHighpush(long newpush)
	{
		_highpush = newpush;
	}

	bool Highstep(long newstep)
	{
		if (newstep < _highstep || _highstep == 0 && newstep > 0)
		{
			_highstep = newstep;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Highpush(long newpush)
	{
		if (newpush < _highpush || _highpush == 0 && newpush > 0)
		{
			_highpush = newpush;
			return true;
		}
		else
		{
			return false;
		}
	}

	char* GetTitle()
	{
		return _szTitle;
	}

	char* GetAuthor()
	{
		return _szAuthor;
	}

	long GetHighstep()
	{
		return _highstep;
	}

	long GetHighpush()
	{
		return _highpush;
	}

	void AddChair()
	{
		_chairs++;
	}

	void SetPlayer(int x, int y)
	{
		_player.x = x;
		_player.y = y;
	}

	long GetChairs()
	{
		return _chairs;
	}

	void SetChairs(int chaircount)
	{
		_chairs = chaircount;
	}

	void Highscore()
	{
		_highscore = true;
	}

	void SetHighscore(bool bHighscore)
	{
		_highscore = bHighscore;
	}

	bool IsHighscore()
	{
		return _highscore;
	}

	POINT _player;

protected:
	char _szTitle[64];
	char _szAuthor[64];
	long _highpush, _highstep;
	long _step, _push;
	long _chairs;
	bool _highscore;
};

#endif