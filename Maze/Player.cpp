#include "pch.h"
#include "Player.h"
#include "Board.h"
#include "AstarTest.h"
#include <stack>

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	p_board = board;

	// RightHand();
	// BFS();
	Astar();

	/*AstarTest a;
	int32 size = p_board->GetSize();

	Pos dest = p_board->GetExitPos();


	a.AstarFunc(_pos, dest, size, p_board);

	std::for_each(a._path.begin(), a._path.end(), [](Pos& pos) { std::cout << pos.y << pos.x << endl; });*/
}

void Player::Update(uint64 deltaTick)
{
	/*if (_pathIdx >= _path.size())
	{
		p_board->GenerateMap();
		Init(p_board);
		return;
	}

	_sumTick += deltaTick;
	
	if (_sumTick >= MOVE_TICK)
	{
		_sumTick = 0;

		_pos = _path[_pathIdx];
		++_pathIdx;
	}
	*/
}

#pragma region RightHand
void Player::RightHand()
{
	Pos pos = _pos;
	DIR dir = _dir;

	_path.clear();
	_path.push_back(pos);

	Pos dest = p_board->GetExitPos();


	Pos front[4] =
	{
		Pos {0, -1},
		Pos {-1, 0},
		Pos {0, 1},
		Pos {1, 0}
	};

	while (pos != dest)
	{
		DIR newDir = DIR((int32(dir) + int32(DIR::COUNT) - 1) % int32(DIR::COUNT));

		// 1. 현재 바라보는 방향을 기준으로 오른쪽으로 갈 수 있는지 확인.
		if (Cango(pos + front[int32(newDir)]))
		{
			// 오른쪽 방향으로 90도 회전
			dir = newDir;

			// 앞으로 한 보 전진
			pos += front[int32(newDir)];
			_path.push_back(pos);
		}
		// 2. 현재 바라보는 방향을 기준으로 전진할 수 있는지 확인.
		else if (Cango(pos + front[int32(dir)]))
		{
			// 앞으로 한보 전진
			pos += front[int32(dir)];
			_path.push_back(pos);
		}
		else
		{
			// 왼쪽 방향으로 90도 회전
			dir = DIR((int32(dir) + 1) % int32(DIR::COUNT));
		}
	}

	stack<Pos> s;

	for (int i = 0; i < _path.size() - 1; ++i)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
		{
			// 되돌아 가야하는 길
			s.pop();
		}
		else
		{
			s.push(_path[i]);
		}
	}

	// 목적지 도착
	if (_path.empty() == false)
		s.push(_path.back());

	vector<Pos> path;
	while (!s.empty())
	{
		path.push_back(s.top());
		s.pop();
	}

	std::reverse(path.begin(), path.end());

	_path = path;

}
#pragma endregion


#pragma region BFS
void Player::BFS()
{
	Pos pos = _pos;
	DIR dir = _dir;

	Pos dest = p_board->GetExitPos();

	Pos front[4] =
	{
		Pos {0, -1},
		Pos {-1, 0},
		Pos {0, 1},
		Pos {1, 0}
	};

	const int32 size = p_board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));

	map<Pos, Pos> parent;

	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	int count = 0;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		// 방문
		cout << "방문함 : " << pos.y << " , " << pos.x << endl;

		if (pos == dest)
			break;

		for (int32 dir = 0; dir < 4; ++dir)
		{
			Pos nextPos = pos + front[dir];

			// 갈 수 있는 곳인지 아닌지 체크
			if (Cango(nextPos) == false)
				continue;

			// 이미 발견한 지역인지 확인.
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			++count;
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}
	}

	_path.clear();

	// 거꾸로 거슬러 올라간다
	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// 시작점은 자신이 곧 부모이다. (그렇게 설정을 했기 때문에)
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());

	cout << count << endl;
}
#pragma endregion


#pragma region Astar
struct PQNode
{
	bool operator < (const PQNode& other)  const { return f < other.f; }
	bool operator > (const PQNode& other)  const { return f > other.f; }

	int32 f;
	int32 g;
	Pos pos;
};

void Player::Astar()
{
	// F = G + H
	// F = 최종 점수 (작을 수록 좋음, 경로에 따라 달라짐)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을 수록 좋음, 경로에 따라 달라짐)
	// H = 목적지에서 얼마나 가까운지 (작을 수록 좋음, 고정) (공식을 정해주어야함)

	Pos start = _pos;
	Pos dest = p_board->GetExitPos();

	enum
	{
		DIR_COUNT = 4
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

	const int32 size = p_board->GetSize();

	// ClosedList (옵션임)
	// closed[y][x] -> (y, x)에 방문을 했는지 여부 (방문을 끝낸애들)
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// best[y][x] -> 지금까지 (y, x)에 대한 가장 좋은 비용 (작을 수록 좋음)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// 부모 추적 용도
	map<Pos, Pos> parent;

	// OpenList (에약된 애들 관리하는 자료구조)
	// 발견한 상태인 녀석들을 OpenList로 관리할 것임. (절반의 성공 느낌)
	priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

	// 1) 예약(발견) 시스템 구현
	// 2) 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수

	// 초기값
	{
		int32 g = 0; // 시작점 -> 시작점이라 0.
		int32 h = 10 * (std::abs(dest.y - start.y) + std::abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다.
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾아서 
		// 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵. ( 이부분 누락하면 드라군이 왼쪽갔다 오른쪽갔다 와리가리침)

		// [선택] (중복된 부분 선택할 수 있는 방법 두가지)
		if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// 방문
		closed[node.pos.y][node.pos.x] = true;

		// 목적지에 도착했으면 종료
		if (node.pos == dest)
			break;

		for (int32 dir = 0; dir < DIR_COUNT; ++dir)
		{
			Pos nextPos = node.pos + front[dir];
			if (Cango(nextPos) == false)
				continue;
			// [선택] 이미 방문한 곳이라면 스킵
			if (closed[nextPos.y][nextPos.x])
				continue;

			// 비용계산
			int32 g = node.g + cost[dir]; // 이전 좌표에서 현재 좌표로 이동하는데 드는 비용 => 시작점을 기준으로 이동하는데 드는 비용.
			int32 h = 10 * (std::abs(dest.y - nextPos.y) + std::abs(dest.x - nextPos.x)); // 해당 좌표에서 고정적임 => 목적지를 기준으로 얼마만큼 떨어져있느냐?.

			// 다른 경로에서 더 빠른 길을 찾았다면 스킵 (더 빠른 길이 있다면)
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// 거꾸로 거슬러 올라간다
	Pos pos = dest;

	_path.clear();
	_pathIdx = 0;

	while (true)
	{
		_path.push_back(pos);

		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());
}
#pragma endregion




bool Player::Cango(Pos pos)
{
	TILE_TYPE tileType = p_board->GetTileType(pos);
	return tileType == TILE_TYPE::EMPTY;
}
