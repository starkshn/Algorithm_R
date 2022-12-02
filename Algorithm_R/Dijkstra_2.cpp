#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>

std::vector<std::vector<int>> adjacent;

void CreateGraph()
{
	adjacent = std::vector<std::vector<int>>(6, std::vector<int>(6, -1));

	adjacent[0][1] = 15;
	adjacent[0][3] = 35;

	adjacent[1][0] = 15;
	adjacent[1][2] = 5;
	adjacent[1][3] = 10;

	adjacent[3][4] = 5;
	adjacent[5][4] = 5;
}

void Dijkstra(int here)
{
	std::pair<int, int> vertexCost;
	std::vector<std::pair<int, int>> discovered;
	std::vector<int> best(6, INT32_MAX);
	std::vector<int> parent(6, -1);
	
	discovered.emplace_back(here, 0);

	best[here] = 0;
	parent[here] = here;

	int i = 0;

	while (discovered.empty() == false)
	{
		std::vector<std::pair<int, int>>::iterator bestIter;
		int bestCost = INT32_MAX;

		for (auto iter = discovered.begin(); iter != discovered.end(); ++iter)
		{
			int vertex = iter->first;
			int cost = iter->second;
			
			if (cost < bestCost)
			{
				bestCost = cost;
				bestIter = iter;
			}
		}

		int cost = bestIter->second;
		here = bestIter->first;
		discovered.erase(bestIter);

		std::cout << "가장 좋은 후보 cost : " << cost << std::endl;

		if (best[here] < cost)
			continue;

		for (int there = 0; there < 6; ++there)
		{
			if (adjacent[here][there] == -1)
				continue;
			
			int nextCost = best[here] + adjacent[here][there];
			if (nextCost > best[there])
				continue;

			best[there] = nextCost;
			parent[there] = here;

			std::cout << "best[there] : " << nextCost << std::endl;

			discovered.emplace_back(there, nextCost);

			
		}

		std::cout << i + 1 << "번 수행후 size() : " << discovered.size() << std::endl;
		++i;
	}
}

void main()
{
	CreateGraph();
	Dijkstra(0);

	return;
}