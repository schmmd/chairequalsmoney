#ifndef _H_GAME
#define _H_GAME

#include "room.h"

#include <memory.h>

class CGame : public CRoom
{
public:
	char Turn(int dx, int dy);
	void Push(int dx, int dy);
	void Pull(int dx, int dy);
	void Move(int dx, int dy);

	bool Do  (char move);
	bool Undo(char move);

	bool IsCompleted()
	{
		return _chairs == 0;
	}

	long GetSteps()
	{
		return _step;
	}

	long GetPushes()
	{
		return _push;
	}

	void SetSteps(long steps)
	{
		_step = steps;
	}

	void SetPushes(long pushes)
	{
		_push = pushes;
	}

private:
	long _step, _push;
};

#endif