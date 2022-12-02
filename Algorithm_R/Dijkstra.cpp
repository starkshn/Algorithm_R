#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>

std::vector<std::vector<int>>	adjacent;

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
	std::pair<int, int>				vertexCost;
	std::list<std::pair<int, int>>	discovered;			// �߰� ���
	std::vector<int>				best(6, INT32_MAX);	// �� �������� ���ݱ��� �߰��� �ּҰŸ�
	std::vector<int>				parent(6, -1);

	// discovered.push_back(std::make_pair(here, 0));
	discovered.emplace_back(here, 0);

	best[here] = 0;
	parent[here] = here;

	while (discovered.empty() == false)
	{
		// ���� ���� �ĺ��� ã�´�.
		std::list<std::pair<int, int>>::iterator bestIter;
		int bestCost = INT32_MAX;
		
		for (auto iter = discovered.begin(); iter != discovered.end(); ++iter)
		{
			const int vertex = iter->first;
			const int cost = iter->second;

			if (cost < bestCost)
			{
				bestCost = cost;
				bestIter = iter;
			}
		}

		int cost = bestIter->second;	
		here = bestIter->first;			
		discovered.erase(bestIter);

		// �湮? �� ª�� ��θ� �ڴʰ� ã�Ҵٸ� ��ŵ.
		if (best[here] < cost)
			continue;

		// �湮! (������ �ֵ� �� ����� ���־�Ѵ�)
		for (int there = 0; there < 6; ++there)
		{
			// ������� �ʾҴٸ� ��ŵ
			if (adjacent[here][there] == -1)
				continue;

			// �� ���� ��θ� ���ſ� ã�Ҵٸ� ��ŵ.
			int nextCost = best[here] + adjacent[here][there];
			if (nextCost >= best[there])
				continue;

			best[there] = nextCost;
			parent[there] = here;

			discovered.emplace_back(there, nextCost);
		}
	}
}


int main()
{
	CreateGraph();
	Dijkstra(0);

	return 0;
}