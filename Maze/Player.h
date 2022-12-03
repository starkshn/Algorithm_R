#pragma once

class Board;

class Player
{
	enum
	{
		MOVE_TICK = 100,
	};

public:
	void	Init(Board* board);
	void	Update(uint64 deltaTick);

private:
	void	RightHand(); // ¿ì¼ö¹ý
	void	BFS();
	void	Astar();

public:
	void	SetPos(POS pos) { _pos = pos; }
	
public:
	POS		GetPos() { return _pos; }

public:
	bool	Cango(POS pos);

private:
	POS				_pos = {};
	DIR				_dir = DIR::UP;
	Board*			p_board = nullptr;

	vector<POS>		_path;
	int32			_pathIdx = 0;
	uint64			_sumTick = 0;


	// queue<POS>	_path;
};

