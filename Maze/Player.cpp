#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	p_board = board;

	// RightHand();
	BFS();
	
}

void Player::Update(uint64 deltaTick)
{
	if (_pathIdx >= _path.size())
		return;

	_sumTick += deltaTick;
	
	if (_sumTick >= MOVE_TICK)
	{
		_sumTick = 0;

		_pos = _path[_pathIdx];
		++_pathIdx;
	}
}

void Player::RightHand()
{
	POS pos = _pos;
	DIR dir = _dir;

	_path.clear();
	_path.push_back(pos);

	POS dest = p_board->GetExitPos();


	POS front[4] =
	{
		POS {0, -1},
		POS {-1, 0},
		POS {0, 1},
		POS {1, 0}
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

	stack<POS> s;

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

	vector<POS> path;
	while (!s.empty())
	{
		path.push_back(s.top());
		s.pop();
	}

	std::reverse(path.begin(), path.end());

	_path = path;

}

void Player::BFS()
{
	POS pos = _pos;
	DIR dir = _dir;

	POS dest = p_board->GetExitPos();

	POS front[4] =
	{
		POS {0, -1},
		POS {-1, 0},
		POS {0, 1},
		POS {1, 0}
	};

	const int32 size = p_board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));

	map<POS, POS> parent;

	queue<POS> q;
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
			POS nextPos = pos + front[dir];

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

		// 시작점은 자신이 곧 부모이다.
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());

	cout << count << endl;
}

bool Player::Cango(POS pos)
{
	TILE_TYPE tileType = p_board->GetTileType(pos);
	return tileType == TILE_TYPE::EMPTY;
}
