#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

vector<bool> visited;
vector<vector<int>> adjacent;

void CreateGraph()
{
	adjacent = vector<vector<int>>(6);
	visited.resize(6);

	adjacent[0].push_back(1);
	adjacent[0].push_back(3);
	adjacent[1].push_back(0);
	adjacent[1].push_back(2);
	adjacent[1].push_back(3);
	adjacent[3].push_back(4);
	adjacent[5].push_back(4);
}

void DFS(int here)
{
	visited[here] = true;
	cout << "Visited : " << here << endl;
	
	for (int i = 0; i < adjacent[here].size(); ++i)
	{
		int there = adjacent[here][i];
		if (visited[there] == false)
			DFS(there);
	}
}

void DFS_ALL()
{
	for (int i = 0; i < adjacent.size(); ++i)
	{
		if (visited[i] == false)
			DFS(i);
	}
}

void main()
{
	CreateGraph();

	DFS_ALL();


	return;
}