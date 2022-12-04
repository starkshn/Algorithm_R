#include "pch.h"
#include "Board.h"
#include "Player.h"

const char* tile = "■";

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init(int32 size, Player* player)
{
	_size = size;
	p_player = player;

	GenerateMap();
}

void Board::Render()
{
	// Renderings
	ConsoleHelper::SetCursorPosition(0, 0);
	ConsoleHelper::ShowConsoleCursor(false);

	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			ConsoleColor color = GetTileColor(Pos{ y, x });
			ConsoleHelper::SetCursorColor(color);

			cout << tile;
		}
		cout << endl;
	}
}

// Binary Tree 미로 생성 Algorithm
void Board::GenerateMap()
{
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x) 
		{
			if (x % 2 == 0 || y % 2 == 0)
				_tile[y][x] = TILE_TYPE::WALL;
			else
				_tile[y][x] = TILE_TYPE::EMPTY;
		}
	}

	// 랜덤으로 우측 혹은 아래로 길을 뚫는 작업
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (x == _size - 2 && y == _size - 2)
				continue;

			if (y == _size - 2)
			{
				_tile[y][x + 1] = TILE_TYPE::EMPTY;
				continue;
			}

			if (x == _size - 2)
			{
				_tile[y + 1][x] = TILE_TYPE::EMPTY;
				continue;
			}
			
			const int ranValue = ::rand() % 2;
			if (ranValue == 0)
			{
				_tile[y][x + 1] = TILE_TYPE::EMPTY;
			}
			else
			{
				_tile[y + 1][x] = TILE_TYPE::EMPTY;
			}
		}
	}
}

TILE_TYPE Board::GetTileType(Pos pos)
{
	if (pos.x < 0 || pos.x >= _size)
		return TILE_TYPE::NONE;

	if (pos.y < 0 || pos.y >= _size)
		return TILE_TYPE::NONE;

	return _tile[pos.y][pos.x];

}

ConsoleColor Board::GetTileColor(Pos pos)
{
	if (p_player != nullptr && p_player->GetPos() == pos)
		return ConsoleColor::YELLOW;
	
	if (GetExitPos() == pos)
		return ConsoleColor::BLUE;

	TILE_TYPE tileType = GetTileType(pos);

	switch (tileType)
	{
	case TILE_TYPE::NONE:
		{

		}
		break;
	case TILE_TYPE::EMPTY:
		{
			return ConsoleColor::GREEN;
		}
		break;
	case TILE_TYPE::WALL:
		{
			return ConsoleColor::RED;
		}
		break;
	default:
		{
			return ConsoleColor::WHITE;
		}
		break;
	}
}
