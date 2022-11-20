#include "pch.h"
#include <iostream>
#include "ConsoleHelper.h"
#include "Board.h"
#include "Player.h"

Board	g_board;
Player	g_player;

int main()
{
	::srand(static_cast<unsigned int>(time(nullptr)));

	g_board.Init(25, &g_player);
	g_player.Init(&g_board);

	uint64 lastTick = 0;
	while (true)
	{
#pragma region
		const uint64 curTick = ::GetTickCount64();
		const uint64 deltaTick = curTick - lastTick;
		lastTick = curTick;
#pragma endregion

		// 입력

		// 로직
		g_player.Update(deltaTick);
		
		// 렌더링
		g_board.Render();
	}
}