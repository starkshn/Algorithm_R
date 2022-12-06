#pragma once
#include "pch.h"

class Board;

struct NodeB
{
	bool operator < (const NodeB& other)  const { return f < other.f; }
	bool operator > (const NodeB& other)  const { return f > other.f; }

	int32 f;
	int32 g;
	Pos pos;
};

class AstarTest
{
public:
	void AstarFunc(Pos start, Pos dest, int32 size, Board* board);

	bool CangoTo(Pos pos);

public:
	Board* p_board;
	vector<Pos> _path;
};

