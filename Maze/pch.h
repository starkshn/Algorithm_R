#pragma once

#include "Types.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

struct Pos
{
	int32 x = 0;
	int32 y = 0;

	bool operator == (Pos& other)
	{
		return y == other.y && x == other.x;
	}
	bool operator != (Pos& other)
	{
		return !(*this == other);
	}

	bool operator < (const Pos& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	Pos operator + (Pos& other)
	{
		Pos ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	Pos& operator += (Pos& other)
	{
		y += other.y;
		x += other.x;
		return *this;
	}

	
};

enum class DIR
{
	UP		= 0,
	LEFT	= 1,
	DOWN	= 2,
	RIGHT	= 3,

	COUNT	= 4,
};




