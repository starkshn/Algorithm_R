#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>

std::vector<bool> discovered;
std::vector<std::vector<int>> adjacent;

void CreateGraph()
{
	discovered.resize(6);
	adjacent = std::vector<std::vector<int>>(6);

	adjacent[0].push_back(1);
	adjacent[0].push_back(3);
	adjacent[1].push_back(0);
	adjacent[1].push_back(2);
	adjacent[1].push_back(3);
	adjacent[3].push_back(4);
	adjacent[5].push_back(4);
}

void BFS(int here)
{
	std::vector<int> parent(6, -1);
	std::vector<int> distance(6, -1);

	std::queue<int> q;
	q.push(here);
	discovered[here] = true;

	parent[here] = here;
	distance[here] = 0;

	while (q.empty() == false)
	{
		here = q.front();
		q.pop();

		std::cout << "Visitied : " << here << std::endl;

		for (int there : adjacent[here])
		{
			if (discovered[there])
				continue;

			q.push(there);
			discovered[there] = true;

			parent[there] = here;
			distance[there] = distance[here] + 1;
		}
	}
}

void main()
{
	CreateGraph();

	BFS(0);

	return;
}
