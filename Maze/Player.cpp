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

		// 1. ���� �ٶ󺸴� ������ �������� ���������� �� �� �ִ��� Ȯ��.
		if (Cango(pos + front[int32(newDir)]))
		{
			// ������ �������� 90�� ȸ��
			dir = newDir;

			// ������ �� �� ����
			pos += front[int32(newDir)];
			_path.push_back(pos);
		}
		// 2. ���� �ٶ󺸴� ������ �������� ������ �� �ִ��� Ȯ��.
		else if (Cango(pos + front[int32(dir)]))
		{
			// ������ �Ѻ� ����
			pos += front[int32(dir)];
			_path.push_back(pos);
		}
		else
		{
			// ���� �������� 90�� ȸ��
			dir = DIR((int32(dir) + 1) % int32(DIR::COUNT));
		}
	}

	stack<Pos> s;

	for (int i = 0; i < _path.size() - 1; ++i)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
		{
			// �ǵ��� �����ϴ� ��
			s.pop();
		}
		else
		{
			s.push(_path[i]);
		}
	}

	// ������ ����
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

		// �湮
		cout << "�湮�� : " << pos.y << " , " << pos.x << endl;

		if (pos == dest)
			break;

		for (int32 dir = 0; dir < 4; ++dir)
		{
			Pos nextPos = pos + front[dir];

			// �� �� �ִ� ������ �ƴ��� üũ
			if (Cango(nextPos) == false)
				continue;

			// �̹� �߰��� �������� Ȯ��.
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			++count;
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}
	}

	_path.clear();

	// �Ųٷ� �Ž��� �ö󰣴�
	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// �������� �ڽ��� �� �θ��̴�. (�׷��� ������ �߱� ������)
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
	// F = ���� ���� (���� ���� ����, ��ο� ���� �޶���)
	// G = ���������� �ش� ��ǥ���� �̵��ϴµ� ��� ��� (���� ���� ����, ��ο� ���� �޶���)
	// H = ���������� �󸶳� ������� (���� ���� ����, ����) (������ �����־����)

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

	// ClosedList (�ɼ���)
	// closed[y][x] -> (y, x)�� �湮�� �ߴ��� ���� (�湮�� �����ֵ�)
	vector<vector<bool>> closed(size, vector<bool>(size, false));

	// best[y][x] -> ���ݱ��� (y, x)�� ���� ���� ���� ��� (���� ���� ����)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

	// �θ� ���� �뵵
	map<Pos, Pos> parent;

	// OpenList (����� �ֵ� �����ϴ� �ڷᱸ��)
	// �߰��� ������ �༮���� OpenList�� ������ ����. (������ ���� ����)
	priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

	// 1) ����(�߰�) �ý��� ����
	// 2) �ڴʰ� �� ���� ��ΰ� �߰ߵ� �� ���� -> ���� ó�� �ʼ�

	// �ʱⰪ
	{
		int32 g = 0; // ������ -> �������̶� 0.
		int32 h = 10 * (std::abs(dest.y - start.y) + std::abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		// ���� ���� �ĺ��� ã�´�.
		PQNode node = pq.top();
		pq.pop();

		// ������ ��ǥ�� ���� ��η� ã�Ƽ� 
		// �� ���� ��η� ���ؼ� �̹� �湮(closed)�� ��� ��ŵ. ( �̺κ� �����ϸ� ����� ���ʰ��� �����ʰ��� �͸�����ħ)

		// [����] (�ߺ��� �κ� ������ �� �ִ� ��� �ΰ���)
		if (closed[node.pos.y][node.pos.x])
			continue;
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// �湮
		closed[node.pos.y][node.pos.x] = true;

		// �������� ���������� ����
		if (node.pos == dest)
			break;

		for (int32 dir = 0; dir < DIR_COUNT; ++dir)
		{
			Pos nextPos = node.pos + front[dir];
			if (Cango(nextPos) == false)
				continue;
			// [����] �̹� �湮�� ���̶�� ��ŵ
			if (closed[nextPos.y][nextPos.x])
				continue;

			// �����
			int32 g = node.g + cost[dir]; // ���� ��ǥ���� ���� ��ǥ�� �̵��ϴµ� ��� ��� => �������� �������� �̵��ϴµ� ��� ���.
			int32 h = 10 * (std::abs(dest.y - nextPos.y) + std::abs(dest.x - nextPos.x)); // �ش� ��ǥ���� �������� => �������� �������� �󸶸�ŭ �������ִ���?.

			// �ٸ� ��ο��� �� ���� ���� ã�Ҵٸ� ��ŵ (�� ���� ���� �ִٸ�)
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// ��������
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// �Ųٷ� �Ž��� �ö󰣴�
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
