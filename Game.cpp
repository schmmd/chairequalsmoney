#include "game.h"

void CGame::Move(int dx, int dy)
{
	char oldtile = Get(_player.x, _player.y);
	char newtile = Get(_player.x + dx, _player.y + dy);

	if (oldtile == '@')
		Set(_player.x, _player.y, ' ');
	else if (oldtile == '+')
		Set(_player.x, _player.y, '.');

	_player.x += dx;
	_player.y += dy;

	if (newtile == ' ')
		Set(_player.x, _player.y, '@');
	else if (newtile = '.')
		Set(_player.x, _player.y, '+');

	_step++;
}

void CGame::Push(int dx, int dy)
{
	POINT newpoint  = { _player.x + dx    , _player.y + dy };
	POINT pushpoint = { _player.x + dx * 2, _player.y + dy * 2};

	char newtile  = Get(newpoint.x, newpoint.y);
	char pushtile = Get(pushpoint.x, pushpoint.y);

	// Replace chair
	// If is chair
	if (newtile == '$')
		Set(newpoint.x, newpoint.y, ' ');
	// If is money
	else if (newtile == '*')
		Set(newpoint.x, newpoint.y, '.');

	// Move chair
	// If is floor
	if (pushtile == ' ')
	{
		Set(pushpoint.x, pushpoint.y, '$');
		if (newtile == '*')
			_chairs++;
	}
	// If is spot
	else if (pushtile == '.')
	{
		Set(pushpoint.x, pushpoint.y, '*');
		if (newtile == '$')
			_chairs--;
	}

	_push++;

	// Move player
	Move(dx, dy);
}

void CGame::Pull(int dx, int dy)
{
	POINT srcpoint = { _player.x - dx, _player.y - dy };
	char srctile  = Get(srcpoint.x, srcpoint.y);
	char desttile = Get(_player.x, _player.y);

	// Replace chair
	// If is chair
	if (srctile == '$')
		Set(srcpoint.x, srcpoint.y, ' ');
	// If is money
	else if (srctile == '*')
		Set(srcpoint.x, srcpoint.y, '.');

	// Move chair
	// If is floor
	if (desttile == '@')
	{
		Set(_player.x, _player.y, '$');
		if (srctile == '*')
			_chairs++;
	}
	// If is spot
	else if (desttile == '+')
	{
		Set(_player.x, _player.y, '*');
		if (srctile == '$')
			_chairs--;
	}

	_push--;
}

char CGame::Turn(int dx, int dy)
{
	char newtile  = Get(_player.x + dx, _player.y + dy);
	char pushtile = Get(_player.x + dx * 2, _player.y + dy * 2);

	char code = '\0';
	if (dx == -1)
		code = 'l';
	else if (dx == +1)
		code = 'r';
	else if (dy == -1)
		code = 'u';
	else if (dy == +1)
		code = 'd';

	// If is clear or is spot
	if (newtile == ' ' || newtile == '.')
	{
		Move(dx, dy);
		return code;
	}
	// If is chair or is money
	else if ((newtile == '$' || newtile == '*') && (pushtile == ' ' || pushtile == '.'))
	{
		Push(dx, dy);
		return code - 32;
	}

	return '\0';
}

bool CGame::Do  (char move)
{
	int dx = 0, dy = 0;

	switch (move)
	{
	case 'l':
	case 'L':
		dx = -1;
		break;
	case 'r':
	case 'R':
		dx = +1;
		break;
	case 'u':
	case 'U':
		dy = -1;
		break;
	case 'd':
	case 'D':
		dy = +1;
		break;
	default:
		return false;
	}

	if (Turn(dx, dy))
		return true;
	else
		return false;
}

bool CGame::Undo(char move)
{
	int dx = 0, dy = 0;

	switch (move)
	{
	case 'l':
	case 'L':
		dx = +1;
		break;
	case 'r':
	case 'R':
		dx = -1;
		break;
	case 'u':
	case 'U':
		dy = +1;
		break;
	case 'd':
	case 'D':
		dy = -1;
		break;
	default:
		return false;
	}

	// 'Move' is uppercase
	if (isupper(move))
	{
		Pull(dx, dy);
	}

	// Move player in opposite direction
	if (Turn(dx, dy))
	{
		_step -= 2;
		return true;
	}
	else
		return false;
}