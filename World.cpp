#include "world.h"
#include "main.h"

#include <assert.h>
#include <stdio.h>

CWorld::CWorld()
{
}

void CWorld::Click(int x, int y)
{
	// _game.Move(x, y);
}

bool CWorld::KeyDown(int iVirtKey)
{
	if (!_game.IsCompleted())
	{
		int dx = 0, dy = 0;

		switch (iVirtKey)
		{
		case VK_LEFT:
			dx = -1;
			break;
		case VK_RIGHT:
			dx = +1;
			break;
		case VK_UP:
			dy = -1;
			break;
		case VK_DOWN:
			dy = +1;
			break;
		default:
			return false;
		}

		char code = _game.Turn(dx, dy);
		if (code != '\0')
		{
			_undo.Push(code);
			_redo.Empty();
		}

		return true;
	}

	return false;
}

char CWorld::Undo()
{
	char last = 0;
	if (_undo.Pop(last))
	{
		_game.Undo(last);
		_redo.Push(last);
	}

	return last;
}

char CWorld::Redo()
{
	char next = 0;
	if (_redo.Pop(next))
	{
		_game.Do(next);
		_undo.Push(next);
	}

	return next;
}

bool CWorld::UndoPush()
{
	char undo;
	bool undone = false;

	while (undo = Undo())
	{
		undone = true;

		if (isupper(undo))
			break;
	}

	return undone;
}

bool CWorld::RedoPush()
{
	char redo;
	bool undone = false;

	while (redo = Redo())
	{
		undone = true;

		if (isupper(redo))
			break;
	}

	return undone;
}

bool CWorld::Highscore()
{
	bool bReturn = false;

	if (_game.Highpush(_game.GetPushes()))
	{
		_roomset.Current()->Highpush(_game.GetPushes());
		bReturn = true;
	}
	if (_game.Highstep(_game.GetSteps()))
	{
		_roomset.Current()->Highstep(_game.GetSteps());
		bReturn = true;
	}

	if (bReturn)
		_game.Highscore();

	return bReturn;
}

bool CWorld::SaveState()
{
	char szFile [512];
	char szLine [512];
	char szIndex[64];

	int index = _roomset.GetIndex();

	sprintf(szLine, "score\\%s", _roomset.GetFilename());
	AddLocalPath(szFile, 512, szLine);			// Prepend local path
	*strrchr(szFile, '.') = '_';				// Change the . to _

	sprintf(szIndex, "[%i]", index);

	_unlink("temp.tmp");
	rename(szFile, "temp.tmp");

	ofstream fout(szFile, ios::out);
	ifstream fin("temp.tmp", ios::in | ios::nocreate);

	if (!fout)
		return false;

	fin.getline(szLine, 512);

	// Skip old index (if it exists)
	while(fin && !fin.eof() && fin.peek() != '[')
	{
		fin.getline(szLine, 512);
	}

	fout << "<index>\n" << index << endl
		 << "<history>\n";
	_roomset.PrintHistory(fout, _undo);

	while(fin && !fin.eof())
	{
		fin.getline(szLine, 512);

		if (szLine[0] != '\0')
			fout << szLine << endl;
	}

	fin.close();
	fout.close();

	_unlink("temp.tmp");

	return true;
}

bool CWorld::SaveHighscore()
{
	char szFile [512];
	char szLine [512];
	char szIndex[64];

	int index = _roomset.GetIndex();

	sprintf(szLine, "score\\%s", _roomset.GetFilename());
	AddLocalPath(szFile, 512, szLine);			// Prepend local path
	*strrchr(szFile, '.') = '_';				// Change the . to _

	sprintf(szIndex, "[%i]", index);

	_unlink("temp.tmp");
	rename(szFile, "temp.tmp");

	ofstream fout(szFile, ios::out);
	ifstream fin("temp.tmp", ios::in | ios::nocreate);

	if (!fout)
		return false;

	bool bHaveWrittenScore = false;
	while(fin && !fin.eof())
	{
		fin.getline(szLine, 512);

		if (szLine[0] != '\0')
			fout << szLine << endl;

		if (strcmp(szLine, szIndex) == 0)
		{
			_roomset.PrintHistory(fout, _undo);
			bHaveWrittenScore = true;

			// Skip old score
			do
			{
				fin.getline(szLine, 512);
			} while (!fin.eof() && strchr(szLine, '[') == 0);
			if (szLine[0] != '\0')
				fout << szLine << endl;
		}
	}

	if (!bHaveWrittenScore)
	{
		fout << szIndex << endl;
		_roomset.PrintHistory(fout, _undo);
	}

	fin.close();
	fout.close();

	_unlink("temp.tmp");

	return true;
}

bool CWorld::LoadHighRecord()
{
	char szFile [512];
	char szLine [512];
	char szIndex[64];

	int index = _roomset.GetIndex();

	sprintf(szLine, "score\\%s", _roomset.GetFilename());
	AddLocalPath(szFile, 512, szLine);	// Prepend local path
	*strrchr(szFile, '.') = '_';		// Change the . to _

	sprintf(szIndex, "[%i]", index);

	ifstream fin(szFile, ios::in | ios::nocreate);

	if (fin)
	{
		while(!fin.eof())
		{
			fin.getline(szLine, 512);

			if (strcmp(szLine, szIndex) == 0)
			{
				// Clear undo, redo, and reset game
				Restart();

				while(!fin.eof() && fin.peek() != '[')
				{
					char ch;
					fin.get(ch);
					if (ch != -1 && ch != '\n')
						_redo.Append(ch);
				}
			}
		}
	}

	fin.close();

	return true;
}