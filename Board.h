#ifndef _H_BOARD
#define _H_BOARD

#include <memory.h>
#include <string.h>
#include <stdio.h>

class CBoard
{
public:
	CBoard() : _board(0) {}
	~CBoard() { Free(); }

	void Create(int width, int height);
	void Clear() { memset(_board, 0, sizeof(bool) * _width * _height); }
	void Free();

	bool InBounds(int x, int y) { if (x >= 0 && y >= 0 && x < Width() && y < Height()) return true; else return false; }

	bool Write(int index, char* szString, int size)
	{
		if ((int)(index + size) <= Size())
		{
			memcpy(_board + index, szString, size);
			// _snprintf(_board + index, size, szString);
			return true;
		}
		else
		{
			return false;
		}
	}

	char Get(int x, int y		 ) { return _board[y * _width + x]; }
	char Get(int i)				   { return _board[i]; }
	void Set(int x, int y, char v) { _board[y * _width + x] = v; }

	int Width () { return _width;  }
	int Height() { return _height; }
	int Size  () { return _width * _height; }

	char* _board;

private:
	int _width;
	int _height;
	int _right;
	int _bottom;
};

#endif