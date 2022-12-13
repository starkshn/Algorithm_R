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

	// edges 후보를 랜덤 cost로 등록한다.
	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0) continue;

			// 우측 연결하는 간선후보
			if (x < _size - 2)
			{
				++Xcount;
				const int32 randValue = ::rand() % 100;
				edges.emplace_back(CostEdge{ randValue, Pos{ y, x }, Pos{ y, x + 2 } });
			}

			// 아래로 연결하는 간선후보
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

		// 같은 그룹이라면은 스킵 (안그러면 사이클 발생)
		if (sets.Find(u) == sets.Find(v)) continue;

		// 두 그룹을 합친다.
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
		// 우선순위 큐 빌드할 때 const, const없으면 빌드 안됨.
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

	// edges[u] : u 정점과 연결된 vertex 목록
	std::map<Pos, std::vector<CostEdge>> edges;

	for (int32 y = 0; y < _size; ++y)
	{
		for (int32 x = 0; x < _size; ++x)
		{
			if (x % 2 == 0 || y % 2 == 0) continue;

			// 우측 연결하는 간선후보
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{y, x};
				Pos v = Pos{y, x + 2};
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}

			// 아래로 연결하는 간선후보
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

	std::map<Pos, bool>		added;	// 해당 정점이 트리에 포함되어 있나?
	std::map<Pos, Pos>		parent; // 어떤 정점이 누구에 의해 연결되어있는지?
	std::map<Pos, int>		best;	// 만들고 있는 트리에 인접한 간선 중, 해당 정점에 닿는 최소 간선의 정보

	// 다익스트라랑 거의 유사함
	// - 다익스트라에서는 best가 [시작점]을 기준으로 한 cost
	// - Prim에서는 best가 [현재 트리]를 기준으로 한 간선 cost
	
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

		// 새로 연결된 정점
		Pos vertex = bestEdge.vertex;

		// 이미 추가되었다면 스킵
		if (added[vertex]) continue;

		added[vertex] = true;

		// 맵에 적용
		{
			int y = (parent[vertex].y + vertex.y) / 2;
			int x = (parent[vertex].x + vertex.x) / 2;
			_tile[y][x] = TILE_TYPE::EMPTY;
		}
		
		// 방금 추가한 정점에 인접한 간선들을 검사한다.
		for (CostEdge& edge : edges[vertex])
		{
			// 이미 추가되었다면 스킵
			if (added[edge.vertex]) continue;

			// 다른 경로로 더 좋은 후보가 발견 되었으면 스킵
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
