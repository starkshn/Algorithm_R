#include "pch.h"
#include "Board.h"
#include "Player.h"
#include "DisjointSet.h"

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
	ConsoleHelper::ShowConsoleColor(false);

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

	std::vector<CostEdge> edges;

	// edges 후보를 랜덤 cost로 등록한다.
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0) continue;
			
			// 우측 연결하는 간선후보
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.emplace_back(CostEdge{ randValue, Pos{ y, x }, Pos{ y, x + 2 }});
			}

			// 아래로 연결하는 간선후보
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.emplace_back(CostEdge{ randValue, Pos{ y, x }, Pos{ y + 2, x } });
			}
		}
	}

	std::sort(edges.begin(), edges.end());

	DisjointSet sets(_size * _size);

	for (CostEdge& edge : edges)
	{
		int32 u = edge.u.y * _size + edge.u.x;
		int32 v = edge.v.y * _size + edge.v.x;

		// 같은 그룹이라면은 스킵 (안그러면 사이클 발생)
		if (sets.Find(u) == sets.Find(v)) continue;

		// 두 그룹을 합친다.
		sets.Merge(u, v);

		int32 y = (edge.u.y + edge.v.y) / 2;
		int32 x = (edge.u.x + edge.v.x) / 2;
		_tile[y][x] = TILE_TYPE::EMPTY;
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
