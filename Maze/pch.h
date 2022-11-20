#pragma once

#include "Types.h"
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

struct POS
{
	int32 y = 0;
	int32 x = 0;

	bool operator == (POS& other)
	{
		return y == other.y && x == other.x;
	}
	bool operator != (POS& other)
	{
		return !(*this == other);
	}

	POS operator + (POS& other)
	{
		POS ret;
		ret.y = other.y;
		ret.x = other.x;
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




