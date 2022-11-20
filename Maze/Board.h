#pragma once
#include "ConsoleHelper.h"

class Player;

enum
{
	BOARD_MAX_SIZE = 100,
};


enum class TILE_TYPE
{
	NONE = 0,
	EMPTY,
	WALL,
	
};

class Board
{
public:
	Board();
	~Board();

public:
	void			Init(INT32 size, Player* player);
	void			Render();

	void			GenerateMap();

public:
	TILE_TYPE		GetTileType(POS pos);
	ConsoleColor	GetTileColor(POS pos);
	POS				GetEnterPos()	{ return POS{ 1, 1 }; };
	POS				GetExitPos()	{ return POS{ _size - 2, _size - 2 }; };

private:
	TILE_TYPE		_tile[BOARD_MAX_SIZE][BOARD_MAX_SIZE] = {};
	INT32			_size = 0;
	Player*			p_player;
	
};

