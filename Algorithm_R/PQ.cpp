#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>
using namespace std;

template <typename T, typename Container = std::vector<T>, typename Predicate = std::less<T>>
class PQ
{
public:
	void push(const T& data) 
	{
		_heap.push_back(data);
		
		int now = static_cast<int>(_heap.size()) - 1;

		while (now > 0)
		{
			int next = (now - 1) / 2; // parent idx
			
			if (_predicate(_heap[now], _heap[next]))
				break;

			std::swap(_heap[now], _heap[next]);
			now = next;
		}
	}

	void pop() 
	{
		_heap[0] = _heap.back();
		_heap.pop_back();

		int now = 0;

		while (true)
		{
			int left = 2 * now + 1;
			int right = 2 * now + 2;

			if (left >= (int)_heap.size())
				break;

			int next = now;

			if (_predicate(_heap[next], _heap[left]))
				next = left;

			if (right < (int)_heap.size() && _predicate(_heap[next], _heap[right]))
				next = right;

			if (next == now)
				break;

			std::swap(_heap[now], _heap[next]);
			now = next;
		}
	}

	T& top() { return _heap[0]; }
	bool empty() { return _heap.empty(); }
	
private:
	Container _heap = {};
	Predicate _predicate = {};
};

struct Pos
{
	int y;
	int x;
};

struct Node
{
	bool operator < (const Node& other) const { return v2 < other.v2; }
	bool operator > (const Node& other) const { return v2 > other.v2; }

	int v1;
	int v2;
	Pos pos;
};

void main()
{
	priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

	pq.push(Node{ 10, 20, Pos{1, 1} });
	pq.push(Node{ 100, 200, Pos{10, 10} });
	pq.push(Node{ 30, 90, Pos{3, 4} });
	pq.push(Node{ 50, 300, Pos{5, 6} });
	pq.push(Node{ 70, 10, Pos{7, 8} });

	while (pq.empty() == false)
	{
		Node node = pq.top();
		int v1 = node.v1;
		int v2 = node.v2;
		Pos pos = node.pos;
		
		cout << "v1 : " << v1 << " " << "v2 : " << v2 <<  " " << "Pos.y : " << pos.y << " " << "Pos.x : " << pos.x << endl;
		pq.pop();
	}

	

	return;
}