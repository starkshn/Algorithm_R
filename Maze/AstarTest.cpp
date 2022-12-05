#include "pch.h"
#include "AstarTest.h"
#include "Board.h"


bool AstarTest::CangoTo(Pos pos)
{
	TILE_TYPE tileType = p_board->GetTileType(pos);
	return tileType == TILE_TYPE::EMPTY;
}


void AstarTest::AstarFunc(Pos _start, Pos _dest, int32 _size, Board* board)
{
	p_board = board;
	Pos start = _start;
	Pos dest = _dest;
	
	enum
	{
		DIR_COUNT = 4,
	};
	
	Pos front[] =
	{
		Pos {0, -1},
		Pos {-1, 0},
		Pos {0, 1},
		Pos {1, 0},

		Pos {-1, -1},
		Pos {1, -1},
		Pos {1, 1},
		Pos {-1, 1}
	};

	int32 cost[] =
	{
		10, 10, 10, 10,
		14, 14, 14, 14
	};

	const int32 size = _size;

	vector<vector<bool>> closed(size, vector<bool>(size, false));
	vector<vector<int32>> best(size, vector<int>(size, INT32_MAX));
	map<Pos, Pos> parent;

	priority_queue<NodeB, std::vector<NodeB>, std::greater<NodeB>> pq;

	// �ʱⰪ
	int32 g = 0;
	int32 h = 10 * (std::abs(dest.y - start.y) + std::abs(dest.x - start.x));
	pq.push(NodeB{g + h, g, start});
	best[start.y][start.x] = g + h;
	parent[start] = start;

	while (!pq.empty())
	{
		NodeB node = pq.top();
		pq.pop();

		if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;
		
		closed[node.pos.y][node.pos.x] = true;
		
		if (node.pos == dest)
			break;
		
		for (int32 dir = 0; dir < DIR_COUNT; ++dir)
		{
			Pos nextPos = node.pos + front[dir];
			if (CangoTo(nextPos) == false)
				continue;
			if (closed[nextPos.y][nextPos.x])
				continue;
			
			int32 g = node.g + cost[dir];
			int32 h = 10 * (std::abs(dest.y - nextPos.y) + std::abs(dest.x - nextPos.x));

			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			best[nextPos.y][nextPos.x] = g + h;
			pq.push(NodeB{ g + h, g, nextPos});
			parent[nextPos] = node.pos;
		}
	}

	Pos pos = dest;
	
	_path.reserve(50);

	while (true)
	{
		_path.push_back(pos);

		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());
}

