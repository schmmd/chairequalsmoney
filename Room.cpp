#include "room.h"
#include "game.h"

void CRoom::Copy(CGame& game)
{
	game.Free();
	game.Create(Width(), Height());
	game.Write(0, _board, Size());
	game.SetAuthor(_szAuthor);
	game.SetTitle(_szTitle);
	game.SetHighpush(_highpush);
	game.SetHighstep(_highstep);
	game.SetPlayer(_player.x, _player.y);
	game.SetChairs(_chairs);
	game.SetHighscore(_highscore);

	game.SetSteps(0);
	game.SetPushes(0);
}

void CRoom::Copy(CRoom& room)
{
	room.Free();
	room.Create(Width(), Height());
	room.Write(0, _board, Size());
	room.SetAuthor(_szAuthor);
	room.SetTitle(_szTitle);
	room.SetHighpush(_highpush);
	room.SetHighstep(_highstep);
	room.SetPlayer(_player.x, _player.y);
	room.SetChairs(_chairs);
	room.SetHighscore(_highscore);
}