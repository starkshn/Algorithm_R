#pragma once

#include "Types.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
using namespace std;

struct POS
{
	int32 x = 0;
	int32 y = 0;

	bool operator == (POS& other)
	{
		return y == other.y && x == other.x;
	}
	bool operator != (POS& other)
	{
		return !(*this == other);
	}

	bool operator < (const POS& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	POS operator + (POS& other)
	{
		POS ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	POS& operator += (POS& other)
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




