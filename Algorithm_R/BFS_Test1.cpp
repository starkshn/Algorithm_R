#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
using namespace std;

vector<bool>		discovered;
vector<vector<int>> adjacent;

void CreateGraph()
{
	adjacent = vector<vector<int>>(6);
	discovered.resize(6);

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
	vector<int> parent(6, -1);
	vector<int> distance(6, -1);

	queue<int> q;
	q.push(here);
	discovered[here] = true;

	parent[here] = here;
	distance[here] = 0;

	while (q.empty() == false)
	{
		here = q.front();
		q.pop();

		cout << "Visited : " << here << endl;
		
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

void BFS_ALL()
{
	for (int i = 0; i < 6; ++i)
		if (discovered[i] == false)
			BFS(i);
}

struct POS
{
	int x = 0;
	int y = 0;

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

void main()
{
	CreateGraph();

	BFS(0);

	/*map<int, string> testMap;

	testMap.insert(pair<int, string>(1, "map1"));
	testMap[2] = "map2";

	map<int, string>::iterator iter;
	iter = testMap.find(2);
	if (iter != testMap.end())
	{
		cout << iter->first << "\n";
		cout << iter->second << "\n";
	}

	cout << "################" << endl;

	for (auto iter = testMap.begin(); iter != testMap.end(); iter++)
	{
		cout << iter->first << "\n";
		cout << iter->second << "\n";
	}*/

	return;
}