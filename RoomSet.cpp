#include "roomset.h"
#include "game.h"
#include "main.h"

void CRoomset::Free()
{
	node* _nav = _head;
	while (_nav != NULL)
	{
		delete _nav->_data;
		_nav->_data = NULL;
		_nav = _nav->_next;
	}
	Destroy();
}

bool CRoomset::SaveBookmark(list<char>& undo)
{
	if (strcmp(_szFile, "") != 0)
	{
		ofstream fout(_szFile, ios::out);

		if (fout)
		{
			fout << Current()->GetTitle() << endl;

			char szBuffer[256];

			int i = 0, height = Current()->Height(), width = Current()->Width();
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					szBuffer[x] = Current()->Get(i);
					i++;
				}

				fout.write(szBuffer, width);
				fout << endl;
			}

			fout << "Title: " << Current()->GetTitle() << endl
				 << "Author: " << Current()->GetAuthor() << endl
				 // << "Highstep: " << Current()->GetHighstep() << endl
				 // << "Highpush: " << Current()->GetHighpush() << endl
				 << "+-+-" << endl;

			PrintHistory(fout, undo);
		}
		else
			MessageBox(NULL, "The file could not be saved. The Disk is unwritable or the file is read only.", "Chair Equals Money", MB_ICONERROR);

		fout.close();
	}

	return true;
}

bool CRoomset::LoadBookmark(list<char>& redo)
{
	if (strcmp(_szFile, "") != 0)
	{
		Free();
		Create();

		char szLine[256];
		char szRoom[256];

		ifstream fin(_szFile, ios::in | ios::nocreate);

		if (!fin)
			return false;

		fin.getline(szLine, 256);

		// Analyze preroom data
		while (!fin.eof() && (szLine[0] == ';' || strchr(szLine, '#') == 0))
		{
			fin.getline(szLine, 256);
		}

		strcpy(szRoom, szLine);

		// Store the room
		int width = 0, height = 0;
		list<char*> lines;
		while(!fin.eof() && strchr(szRoom, '#') != 0)
		{
			// Check if longer than maxwidth
			int len = strlen(szRoom);
			if (width < len)
				width = len;
			height++;

			char* szTemp = new char[256];
			strcpy(szTemp, szRoom);
			lines.Append(szTemp);

			fin.getline(szRoom, 256);
		}

		if (width <= 0 || height <= 0)
			return false;

		// Create new room
		CRoom* room = new CRoom;
		room->Create(width, height);

		// Copy to room
		int index = 0;
		char* szRoomLine;
		while((szRoomLine = lines.Get(0)) != NULL)
		{
			// Check for chairs and player
			char* tile = szRoomLine;
			for (int i = 0; i < width; i++)
			{
				switch(*tile)
				{
				case '@':
				case '+':
					room->SetPlayer(tile - szRoomLine, index / width);
					break;
				case '$':
					room->AddChair();
					break;
				}
				tile++;
			}

			room->Write(index, szRoomLine, strlen(szRoomLine));
			index += width;

			delete szRoomLine;
			lines.Delete(0);
		}

		strcpy(szLine, szRoom);

		// Analyze postroom data
		while (szLine[0] != '\0' && strcmp(szLine, "+-+-") != 0 && szLine[0] != ' ' && szLine[0] != '\t' && strchr(szLine, '#') == 0)
		{
			if (strncmp(szLine, "Title:", 6) == 0)
				room->SetTitle(szLine + 7);
			else if (strncmp(szLine, "Author:", 7) == 0)
				room->SetAuthor(szLine + 8);
			// else if (strncmp(szLine, "Highstep:", 9) == 0)
				// room->SetHighstep(strtol(szLine + 10, NULL, 10));
			// else if (strncmp(szLine, "Highpush:", 9) == 0)
				// room->SetHighpush(strtol(szLine + 10, NULL, 10));
			fin.getline(szLine, 256);
		}

		if (strcmp(szLine, "+-+-") == 0)
		{
			fin.getline(szLine, 256);

			while(strcmp(szLine, "") != 0)
			{
				if (szLine[0] != ';' && szLine[0] != ' ')
				{
					int i = 0;
					while (szLine[i] != '\0')
						redo.Append(szLine[i++]);
				}

				fin.getline(szLine, 256);
			}
		}

		// Add the room if it exists
		if (width > 0 && height > 0)
			Append(room);

		fin.close();

		_loc = GetNode(0);
		_index = index;
	}

	return true;
}

bool CRoomset::Load()
{
	if (strcmp(_szFile, "") != 0)
	{
		Free();
		Create();

		int index = 0;

		char szLine[256];
		char szRoom[256];
		char szTitleLine[256];
		char szAuthor[256];

		bool bHaveRoom = false;

		ifstream fin(_szFile, ios::in | ios::nocreate);

		fin.getline(szLine, 256);

		if (!fin.eof())
		{
			while(!fin.eof() && strchr(szLine, '#') == 0 && (szLine[0] == ';' || szLine[0] == ' ' || szLine[0] == '\0'))
			{
				// if (strncmp(szLine, "; Index: ", 9) == 0)
				//	index = strtol(szLine + 9, NULL, 10);

				if (strncmp(szLine, "; Copyright: ", 13) == 0)
					strcpy(szAuthor, szLine + 13);

				/*
				if (strncmp(szLine, "; History: ", 11) == 0)
				{
					int i = 11;
					while (szLine[i] != '\0')
						redo.Append(szLine[i++]);
				}
				*/

				sprintf(szTitleLine, szLine);
				fin.getline(szLine, 256);
			}
		}

		while(!fin.eof())
		{
			// Analyze preroom data
			while (!fin.eof() && (szLine[0] == ';' || strchr(szLine, '#') == 0))
			{
				strcpy(szTitleLine, szLine);
				fin.getline(szLine, 256);
			}

			strcpy(szRoom, szLine);

			// Store the room
			int width = 0, height = 0;
			list<char*> lines;
			while(!fin.eof() && strchr(szRoom, '#') != 0)
			{
				// Check if longer than maxwidth
				int len = strlen(szRoom);
				if (width < len)
					width = len;
				height++;

				char* szTemp = new char[256];
				strcpy(szTemp, szRoom);
				lines.Append(szTemp);

				fin.getline(szRoom, 256);
			}

			if (width <= 0 || height <= 0)
				break;

			// Create new room
			CRoom* room = new CRoom;
			room->Create(width, height);

			// Copy to room
			int index = 0;
			char* szRoomLine;
			while((szRoomLine = lines.Get(0)) != NULL)
			{
				// Check for chairs and player
				char* tile = szRoomLine;
				for (int i = 0; i < width; i++)
				{
					switch(*tile)
					{
					case '@':
					case '+':
						room->SetPlayer(tile - szRoomLine, index / width);
						break;
					case '$':
						room->AddChair();
						break;
					}
					tile++;
				}

				room->Write(index, szRoomLine, strlen(szRoomLine));
				index += width;

				delete szRoomLine;
				lines.Delete(0);
			}

			// If it the line before the room may contain a name
			if (strncmp(szTitleLine, "; ", 2) == 0)
			{
				room->SetTitle(szTitleLine + 2);
			}

			bHaveRoom = true;

			strcpy(szLine, szRoom);

			// Analyze postroom data
			while (szLine[0] != '\0' && szLine[0] != ' ' && szLine[0] != '\t' && strchr(szLine, '#') == 0)
			{
				if (strncmp(szLine, "Title:", 6) == 0)
					room->SetTitle(szLine + 7);
				else if (strncmp(szLine, "Author:", 7) == 0)
					room->SetAuthor(szLine + 8);
				// else if (strncmp(szLine, "Highstep:", 9) == 0)
				//	room->SetHighstep(strtol(szLine + 10, NULL, 10));
				// else if (strncmp(szLine, "Highpush:", 9) == 0)
				//	room->SetHighpush(strtol(szLine + 10, NULL, 10));
				fin.getline(szLine, 256);
			}

			if (strcmp(room->GetAuthor(), "Anonymous") == 0 && strcmp(szAuthor, "") != 0)
				room->SetAuthor(szAuthor);

			// Add the room if it exists
			if (width > 0 && height > 0)
				Append(room);
		}

		fin.close();

		_loc = GetNode(index);
		_index = index;

		if (bHaveRoom && _loc != NULL)
			return true;
		else
			return false;
	}

	return true;
}

bool CRoomset::Save(bool bSaveAll)
{
	if (strcmp(_szFile, "") != 0)
	{
		ofstream fout(_szFile, ios::out);

		if (fout)
		{
			list<CRoom*>::node* _oldloc = _loc;

			if (!bSaveAll || (_loc = GetHead()) != NULL)
			{
				/*
				// Save current room info
				if (bSaveAll)
					fout << "; Index: " << _index << endl;
				else
					fout << "; Index: 0" << endl;

				char szBuffer[64];
				list<char>::node* cur = undo.GetTail();
				while(cur != NULL)
				{
					int i = 0;
					while(cur != NULL && i < 63)
					{
						szBuffer[i] = cur->_data;

						cur = cur->_prev;
						i++;
					}
					szBuffer[i] = '\0';

					fout << "; History: " << szBuffer << endl;
				}
				*/

				do
				{
					char szBuffer[256];

					int i = 0, height = Current()->Height(), width = Current()->Width();
					for (int y = 0; y < height; y++)
					{
						for (int x = 0; x < width; x++)
						{
							szBuffer[x] = Current()->Get(i);
							i++;
						}

						fout.write(szBuffer, width);
						fout << endl;
					}

					fout << "Title: " << Current()->GetTitle() << endl
						 << "Author: " << Current()->GetAuthor() << endl
						 // << "Highstep: " << Current()->GetHighstep() << endl
						 // << "Highpush: " << Current()->GetHighpush() << endl
						 << endl;

				} while (bSaveAll && (_loc = _loc->_next) != NULL);
			}

			_loc = _oldloc;
		}
		else
			MessageBox(NULL, "Error: Disk is unwritable or the file is read only.", "Chair Equals Money", MB_ICONERROR);

		fout.close();
	}

	return true;
}

void CRoomset::PrintHistory(ofstream& fout, list<char>& undo)
{
	char szBuffer[64];

	list<char>::node* cur = undo.GetTail();
	while(cur != NULL)
	{
		int i = 0;
		while(cur != NULL && i < 63)
		{
			szBuffer[i] = cur->_data;

			cur = cur->_prev;
			i++;
		}
		szBuffer[i] = '\0';

		fout << szBuffer << endl;
	}
}

bool CRoomset::LoadHighscores(list<char>& redo)
{
	char szFile [512];
	char szLine [512];

	int index = 0;

	sprintf(szLine, "score\\%s", GetFilename());
	AddLocalPath(szFile, 512, szLine);			// Prepend local path
	*strrchr(szFile, '.') = '_';				// Change the . to _

	ifstream fin(szFile, ios::in | ios::nocreate);

	CGame  game;
	CRoom* room = NULL;
	if (fin)
	{
		while(!fin.eof())
		{
			fin.getline(szLine, 512);

			if (szLine[0] == '<')
			{
				if (strcmp(szLine, "<index>") == 0)
				{
					fin.getline(szLine, 512);

					if (strchr(szLine, '['))
						return false;

					index = strtol(szLine, NULL, 10);
				}

				if (strncmp(szLine, "<history>", 9) == 0)
				{
					while(!fin.eof() && fin.peek() != '[')
					{
						char ch;
						fin.get(ch);
						if (ch != -1 && ch != '\n')
							redo.Append(ch);
					}
				}
			}

			if (szLine[0] == '[')
			{
				*(strrchr(szLine, ']')) = '\0';
				long index = strtol(szLine + 1, NULL, 10);

				room = Get(index);
				room->Copy(game);

				while(!fin.eof() && fin.peek() != '[')
				{
					char ch;
					fin.get(ch);
					if (ch != -1 && ch != '\n' && !game.Do(ch))
						return false;
				}

				if (game.IsCompleted())
				{
					room->SetHighpush(game.GetPushes());
					room->SetHighstep(game.GetSteps());
				}
				else
				{
					return false;
				}
			}
		}
	}

	if (!(_loc = GetNode(index)))
		return false;
	
	_index = index;

	fin.close();

	return true;
}

void CRoomset::DeleteHighscores()
{
	char szFile [512];
	char szLine [512];

	sprintf(szLine, "score\\%s", GetFilename());
	AddLocalPath(szFile, 512, szLine);			// Prepend local path
	*strrchr(szFile, '.') = '_';				// Change the . to _

	_unlink(szFile);

	list<CRoom*>::node* loc = GetHead();
	while(loc != NULL)
	{
		loc->_data->SetHighpush(0);
		loc->_data->SetHighstep(0);
		loc = loc->_next;
	}
}