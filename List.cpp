#include "linkedlist.h"
#include <assert.h>

void CUndoList::PushState(const char* state)
{
	if (!(_width == 0 && _height == 0))
	{
		char* newstate = new char[_width * _height];
		memcpy(newstate, state, sizeof(char) * _width * _height);
		Push(newstate);
	}
}

void CUndoList::PopState (char** state)
{
	if (!(_width == 0 && _height == 0))
	{
		*state = GetTop();
		Pop();
	}
}