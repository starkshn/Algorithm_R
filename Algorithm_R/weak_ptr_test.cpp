#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

class K
{
public:
	K() { cout << "123" << endl; }
	K(const K& other) {}
	~K() {}
	void operator = (const K& other) {}

public:
	void Attack()
	{
		if (_target.expired() == false)
		{
			shared_ptr<K> k1 = _target.lock();
			k1->Attack();
		}
	}


private:
	weak_ptr<K> _target;
};

void main()
{
	K a = {};
	return;
}