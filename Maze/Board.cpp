#include "pch.h"
#include "Board.h"
#include "Player.h"
#include "DisjointSet.h"

const char* tile = "��";

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

	// GenerateMap_Kruskal();
	GenerateMap_Prim();
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

void Board::GenerateMap_Kruskal()
{
	struct CostEdge
	{
		int cost;
		Pos u;
		Pos v;

		bool operator < (CostEdge& other)
		{
			return cost < other.cost;
		}
	};

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

	int Xcount = 0;
	int Ycount = 0;

	// edges �ĺ��� ���� cost�� ����Ѵ�.
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0) continue;

			// ���� �����ϴ� �����ĺ�
			if (x < _size - 2)
			{
				++Xcount;
				const int32 randValue = ::rand() % 100;
				edges.emplace_back(CostEdge{ randValue, Pos{ y, x }, Pos{ y, x + 2 } });
			}

			// �Ʒ��� �����ϴ� �����ĺ�
			if (y < _size - 2)
			{
				++Ycount;
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

		// ���� �׷��̶���� ��ŵ (�ȱ׷��� ����Ŭ �߻�)
		if (sets.Find(u) == sets.Find(v)) continue;

		// �� �׷��� ��ģ��.
		sets.Merge(u, v);

		int32 y = (edge.u.y + edge.v.y) / 2;
		int32 x = (edge.u.x + edge.v.x) / 2;
		_tile[y][x] = TILE_TYPE::EMPTY;
	}
}

void Board::GenerateMap_Prim()
{
	struct CostEdge
	{
		int cost;
		Pos vertex;

		bool operator < (const CostEdge& other) const
		{
			return cost < other.cost;
		}
		// �켱���� ť ������ �� const, const������ ���� �ȵ�.
	};

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

	// edges[u] : u ������ ����� vertex ���
	std::map<Pos, std::vector<CostEdge>> edges;

	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0) continue;

			// ���� �����ϴ� �����ĺ�
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{y, x};
				Pos v = Pos{y, x + 2};
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}

			// �Ʒ��� �����ϴ� �����ĺ�
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{ y, x };
				Pos v = Pos{ y + 2, x};
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}
		}
	}

	std::map<Pos, bool>		added;	// �ش� ������ Ʈ���� ���ԵǾ� �ֳ�?
	std::map<Pos, Pos>		parent; // � ������ ������ ���� ����Ǿ��ִ���?
	std::map<Pos, int>		best;	// ����� �ִ� Ʈ���� ������ ���� ��, �ش� ������ ��� �ּ� ������ ����

	// ���ͽ�Ʈ��� ���� ������
	// - ���ͽ�Ʈ�󿡼��� best�� [������]�� �������� �� cost
	// - Prim������ best�� [���� Ʈ��]�� �������� �� ���� cost
	
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			best[Pos{ y, x }] = INT32_MAX;
			added[Pos{ y, x }] = false;
		}
	}

	std::priority_queue<CostEdge> pq;
	const Pos startPos = Pos{1, 1};
	pq.push(CostEdge{ 0, startPos });
	best[startPos] = 0;
	parent[startPos] = startPos;
	
	while (pq.empty() == false)
	{
		CostEdge bestEdge = pq.top();
		pq.pop();

		// ���� ����� ����
		Pos vertex = bestEdge.vertex;

		// �̹� �߰��Ǿ��ٸ� ��ŵ
		if (added[vertex]) continue;

		added[vertex] = true;

		// �ʿ� ����
		{
			int y = (parent[vertex].y + vertex.y) / 2;
			int x = (parent[vertex].x + vertex.x) / 2;
			_tile[y][x] = TILE_TYPE::EMPTY;
		}
		
		// ��� �߰��� ������ ������ �������� �˻��Ѵ�.
		for (CostEdge& edge : edges[vertex])
		{
			// �̹� �߰��Ǿ��ٸ� ��ŵ
			if (added[edge.vertex]) continue;

			// �ٸ� ��η� �� ���� �ĺ��� �߰� �Ǿ����� ��ŵ
			if (edge.cost > best[edge.vertex]) continue;

			best[edge.vertex] = edge.cost;
			parent[edge.vertex] = vertex;
			pq.push(edge);
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
