#pragma once

class Board;

class Player
{
public:
	void	Init(Board* board);
	void	Update(uint64 deltaTime);

public:
	void	SetPos(POS pos) { _pos = pos; }
	
public:
	POS		GetPos() { return _pos; }

private:
	POS		_pos = {};
	DIR		_dir = DIR::UP;
	Board*	p_board = nullptr;
	
	
};

