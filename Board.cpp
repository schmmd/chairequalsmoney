#include "board.h"

void CBoard::Create(int width, int height)
{
	if (_board != 0)
		Free();

	_width  = width;
	_height = height;
	_right  = width - 1;
	_bottom = height - 1;

	_board = new char[width * height];
	Clear();
}

void CBoard::Free()
{
	if (_board)
	{
		delete _board;
		_board = 0;
	}
}