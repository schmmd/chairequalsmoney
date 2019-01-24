#ifndef _H_ROOMSET
#define _H_ROOMSET

#include <windows.h>
#include <fstream.h>

#include "room.h"
#include "list.h"

class CGame;

class CRoomset : public list<CRoom*>
{
public:
	CRoomset()
		: _loc(0)
	{	
		strcpy(_szFile, "");
	}
	~CRoomset()
	{
		Free();
	}

	bool IsFirst()
	{
		return _loc == _head;
	}

	bool IsLast()
	{
		return _loc == _tail;
	}

	bool IsNext()
	{
		return _loc != NULL && _loc->_next != NULL;
	}

	bool IsPrev()
	{
		return _loc != NULL && _loc->_prev != NULL;
	}

	bool GoFirst()
	{
		if (_head != NULL)
		{
			_index = 0;
			_loc = _head;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool GoLast()
	{
		if (_tail != NULL)
		{
			_index = _len - 1;
			_loc = _tail;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool GoNext()
	{
		if (_loc == NULL || _loc->_next == NULL)
		{
			return false;
		}
		else
		{
			_index++;
			_loc = _loc->_next;
			return true;
		}
	}

	bool GoPrev()
	{
		if (_loc == NULL || _loc->_prev == NULL)
		{
			return false;
		}
		else
		{
			_index--;
			_loc = _loc->_prev;
			return true;
		}
	}

	bool Exists()
	{
		return _loc != NULL;
	}

	CRoom* Current()
	{
		return _loc->_data;
	}

	int GetIndex()
	{
		return _index;
	}

	bool SaveBookmark(list<char>& undo);
	bool LoadBookmark(list<char>& redo);

	bool Load();
	bool Save(bool bSaveAll = true);
	bool LoadHighscores(list<char>& redo);
	void DeleteHighscores();
	void PrintHistory(ofstream& fout, list<char>& undo);
	void Free();

	void SetFile(char* szFile)
	{
		strcpy(_szFile, szFile);
	}

	char* GetFile()
	{
		return _szFile;
	}

	void SetFilename(char* szFilename)
	{
		strcpy(_szFilename, szFilename);
	}

	char* GetFilename()
	{
		return _szFilename;
	}

	void SetFilterindex(int iFilterIndex)
	{
		_iFilterIndex = iFilterIndex;
	}

	int GetFilterindex()
	{
		return _iFilterIndex;
	}

protected:
	node* _loc;
	int	  _index;

	char _szFile[512];
	char _szFilename[256];
	int _iFilterIndex;
};

#endif